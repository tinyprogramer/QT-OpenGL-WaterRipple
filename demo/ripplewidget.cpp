#include "ripplewidget.h"

#include <QOpenGLShaderProgram>

#include <QDebug>
#include <QFile>
#include <QFileInfo>

//"precision highp float;"is not supported by some old version of GLSL
//pay attention to the openGL version in your computer
//it may lead to shader link errors when runing(rahter than building) the program 

//the algorithm used by update_program is based on wave equation
//for detail interpret of shaders,you could go to my blog https://blog.csdn.net/qq_41961619/article/details/114074630

static GLfloat vertArray[]={//vertexes of square
    -1.0,1.0,
    -1.0,-1.0,
    1.0,-1.0,
    1.0,1.0,
    -1.0,1.0,
    1.0,-1.0
};

static const char* globVert=//vertex shader for drop_program and update_program
        "attribute vec2 vertex;\n"
        "varying vec2 coord;\n"
        "void main() {\n"
        "	coord = vertex * 0.5 + 0.5;\n"//convert coordinate from drawing space [-1,1] to texture space [0,1]
        "	gl_Position = vec4(vertex, 0.0, 1.0);\n"
        "}\n";

static const char* renderVert=//vertex shader for render_program
        "precision highp float;\n"
        "attribute vec2 vertex;\n"
        "varying vec2 ripplesCoord;\n"//in this program ripplesCoord has the same value as backgroundCoord
        "varying vec2 backgroundCoord;\n"
        "void main() {\n"
        "	backgroundCoord=vertex*0.5+0.5;\n"
        "	ripplesCoord=backgroundCoord;\n"//for convinience,I hold both of them
        "	gl_Position = vec4(vertex.x, vertex.y, 0.0, 1.0);\n"
        "}\n";

static const char* renderFrag=//fragment shader for render_program
        "precision highp float;\n"
        "uniform sampler2D samplerBackground;\n"//background image
        "uniform sampler2D samplerRipples;\n"//height of water,from framebuffer
        "uniform vec2 delta;\n"//distance between sample points
        "uniform float perturbance;\n"//you can regard this value as average height of water surface
        "varying vec2 ripplesCoord;\n"
        "varying vec2 backgroundCoord;\n"
        "void main() {\n"
        "	float height = texture2D(samplerRipples, ripplesCoord).r;\n"
        "	float heightX = texture2D(samplerRipples, vec2(ripplesCoord.x + delta.x, ripplesCoord.y)).r;\n"
        "	float heightY = texture2D(samplerRipples, vec2(ripplesCoord.x, ripplesCoord.y + delta.y)).r;\n"
        "	vec3 dx = vec3(delta.x, heightX - height, 0.0);\n"
        "	vec3 dy = vec3(0.0, heightY - height, delta.y);\n"
        "	vec2 offset = -normalize(cross(dy, dx)).xz;\n"//here use "cross" to calculate the normals of water surface 
		//"offset" is an approximant of real offset result in light refract 
        "	float specular = pow(max(0.0, dot(offset, normalize(vec2(-0.6, 1.0)))), 4.0);\n"//here simulate light reflect
		//you can see some white highlight on the ripple by adding this value
        "	gl_FragColor = texture2D(samplerBackground, backgroundCoord + offset * perturbance) + specular;\n"
        "}\n";

static const char* updateFrag=//fragment shader for update_program
        "precision highp float;\n"
        "uniform sampler2D texture;\n"//water height from current reading framebuffer
        "uniform vec2 delta;\n"
        "uniform float damping;\n"
        "varying vec2 coord;\n"
        "void main() {\n"
        "	vec4 info = texture2D(texture, coord);\n"
        "	vec2 dx = vec2(delta.x, 0.0);\n"
        "	vec2 dy = vec2(0.0, delta.y);\n"
        "	float average = (\n"
        "		texture2D(texture, coord - dx).r +\n"
        "		texture2D(texture, coord - dy).r +\n"
        "		texture2D(texture, coord + dx).r +\n"
        "		texture2D(texture, coord + dy).r\n"
        "	) * 0.25;\n"
        "	info.g += (average - info.r) * 2.0;\n"//here calculate the velocity of water surface in vertical direction for next frame
        "	info.g *= damping;\n"//multiply with damping so that ripple won't last forever
        "	info.r += info.g;\n"//water height for next frame 
        "	gl_FragColor = info;\n"//info.r save height,info.g save velocity
        "}\n";

static const char* dropFrag=//fragment shader for drop_program
        "precision highp float;\n"
        "const float PI = 3.141592653589793;\n"
        "uniform sampler2D texture;\n"//water height from current reading framebuffer
        "uniform vec2 center;\n"//parameters for ripple
        "uniform float radius;\n"//
        "uniform float strength;\n"//
        "uniform float ratio;\n"//aspect ratio of widget,we need this to create circular rather than oval shaped ripples
        "varying vec2 coord;\n"
        "void main() {\n"
        "	vec4 info = texture2D(texture, coord);\n"
        "	float x=center.x * 0.5 + 0.5 - coord.x;\n"
        "	float y=(center.y * 0.5 + 0.5 - coord.y)*ratio;\n"
        "	float drop = max(0.0, 1.0 - length(vec2(x,y)) / radius);\n"//initial height of the ripple
        "	drop = 0.5 - cos(drop * PI) * 0.5;\n"//value out of the ripple circle is 0
        "	info.r += drop * strength;\n"//add ripple
        "	gl_FragColor = info;\n"
        "}\n";

RippleWidget::RippleWidget(QWidget* parent,bool insfilter)
    :QOpenGLWidget(parent),m_texture(nullptr)
{

    this->setMouseTracking(true);

    if(parent&&insfilter)
    {
        parent->installEventFilter(this);//supervision mouse events of parent widget
    }
    m_texIndex=0;
    m_radius=20;
    m_strength=0.01;
    m_resolution=2.0;
    m_damping=0.995;
    m_backgroundImg=":/img/bg3.jpg";
}

RippleWidget::~RippleWidget()//destroy sources about OpenGL
{
    makeCurrent();
    if(m_texture)
    {
        m_texture->destroy();
        delete m_texture;
    }
    glDeleteFramebuffers(m_FrameBuffers.size(),m_FrameBuffers.data());
    glDeleteTextures(m_Textures.size(),m_Textures.data());
    m_globVAO.destroy();
    m_globVBO.destroy();
    doneCurrent();
}

void RippleWidget::swapFrameBuffer()//swap current reading and writting framebuffer
{
    m_texIndex=1-m_texIndex;//only swap the indexes infact 
}//we should make sure that shaders get latest water height from framebuffer

void RippleWidget::initializeGL()
{

    initializeOpenGLFunctions();

    glClearColor(0.0f,0.0f,0.0f,0.0f);

    m_globVAO.create();
    m_globVAO.bind();

    m_globVBO.create();
    m_globVBO.bind();

    m_globVBO.allocate(vertArray,sizeof(vertArray));

    drop_program=new QOpenGLShaderProgram;
    //initProgram(":/glob.vert",":/drop.frag",drop_program);
    initProgram(globVert,dropFrag,drop_program);
    drop_program->setAttributeBuffer("vertex",GL_FLOAT,0,2,2*sizeof(GL_FLOAT));
    drop_program->enableAttributeArray("vertex");

    render_program=new QOpenGLShaderProgram;
    //initProgram(":/render.vert",":/render.frag",render_program);
    initProgram(renderVert,renderFrag,render_program);
    render_program->setAttributeBuffer("vertex",GL_FLOAT,0,2,2*sizeof(GL_FLOAT));
    render_program->enableAttributeArray("vertex");


    update_program=new QOpenGLShaderProgram;
    //initProgram(":/glob.vert",":/update.frag",update_program);
    initProgram(globVert,updateFrag,update_program);
    update_program->setAttributeBuffer("vertex",GL_FLOAT,0,2,2*sizeof(GL_FLOAT));
    update_program->enableAttributeArray("vertex");

    m_globVAO.release();
    m_globVBO.release();

    m_texture=new QOpenGLTexture(QImage(m_backgroundImg).mirrored());//set background image texture
    m_texture->setMagnificationFilter(QOpenGLTexture::Linear);

    unsigned int texture1,texture2,fb1,fb2;//create reading and writting framebuffers and their texture attachments
    glGenFramebuffers(1,&fb1);
    glBindFramebuffer(GL_FRAMEBUFFER,fb1);
    glGenTextures(1, &texture1);

    glBindTexture(GL_TEXTURE_2D, texture1);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);//this setting let ripple bound from edge
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F_ARB, this->width(),this->height(), 0, GL_RGBA, GL_FLOAT, NULL);
	//here we use type GL_FLOAT to save the water height and velocity

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture1, 0);

    m_FrameBuffers.push_back(fb1);
    m_Textures.push_back(texture1);

    glGenFramebuffers(1,&fb2);
    glBindFramebuffer(GL_FRAMEBUFFER,fb2);
    glGenTextures(1, &texture2);

    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F_ARB, this->width(),this->height(), 0, GL_RGBA, GL_FLOAT, NULL);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture2, 0);

    m_FrameBuffers.push_back(fb2);
    m_Textures.push_back(texture2);


    m_deltx=m_resolution/this->width();
    m_delty=m_resolution/this->height();
    m_aspectratio=(GLfloat)this->height()/(GLfloat)this->width();

}

void RippleWidget::paintGL()
{
    this->updateFrame();
    this->render();

    update();
}

void RippleWidget::resizeGL(int width, int height)//when the size of widget changed,delete old framebuffers/textures and create new ones
{
    glDeleteFramebuffers(m_FrameBuffers.size(),m_FrameBuffers.data());
    glDeleteTextures(m_Textures.size(),m_Textures.data());
    m_FrameBuffers.clear();
    m_Textures.clear();
    unsigned int texture1,texture2,fb1,fb2;
    glGenFramebuffers(1,&fb1);
    glBindFramebuffer(GL_FRAMEBUFFER,fb1);
    glGenTextures(1, &texture1);

    glBindTexture(GL_TEXTURE_2D, texture1);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F_ARB, this->width(),this->height(), 0, GL_RGBA, GL_FLOAT, NULL);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture1, 0);

    m_FrameBuffers.push_back(fb1);
    m_Textures.push_back(texture1);

    glGenFramebuffers(1,&fb2);
    glBindFramebuffer(GL_FRAMEBUFFER,fb2);
    glGenTextures(1, &texture2);

    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F_ARB, this->width(),this->height(), 0, GL_RGBA, GL_FLOAT, NULL);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture2, 0);

    m_FrameBuffers.push_back(fb2);
    m_Textures.push_back(texture2);

    m_deltx=m_resolution/this->width();
    m_delty=m_resolution/this->height();
    m_aspectratio=(GLfloat)this->height()/(GLfloat)this->width();
}

void RippleWidget::initProgram(QString vert,QString frag,QOpenGLShaderProgram* pro){
    if(!pro->addShaderFromSourceFile(QOpenGLShader::Vertex,vert))
    {
        qDebug()<< vert<<(pro->log());
        return;
    }
    if(!pro->addShaderFromSourceFile(QOpenGLShader::Fragment,frag))
    {
        qDebug()<< vert<<(pro->log());
        return;
    }
    if(!pro->link())
    {
        qDebug()<<vert<< (pro->log());
        return;
    }
}

void RippleWidget::initProgram(const char *vert, const char *frag, QOpenGLShaderProgram *pro)
{
    if(!pro->addShaderFromSourceCode(QOpenGLShader::Vertex,vert))
    {
        qDebug()<< (pro->log());
        return;
    }
    if(!pro->addShaderFromSourceCode(QOpenGLShader::Fragment,frag))
    {
        qDebug()<< (pro->log());
        return;
    }
    if(!pro->link())
    {
        qDebug()<< (pro->log());
        return;
    }
}

void RippleWidget::mouseMoveEvent(QMouseEvent* ev)
{
    this->drop(ev->x(),ev->y(),m_radius,m_strength);
}

void RippleWidget::mousePressEvent(QMouseEvent *ev)
{
    this->drop(ev->x(),ev->y(),1.5*m_radius,8*m_strength);
}

void RippleWidget::drop(int x,int y,int radius,float strength)//draw with drop_program,add ripples
{

    makeCurrent();
    drop_program->bind();

    glBindFramebuffer(GL_FRAMEBUFFER,m_FrameBuffers[1-m_texIndex]);
    m_globVAO.bind();
    float px=(float)(2*x-this->width())/this->width(),py=-(float)(2*y-this->height())/this->height();
    float ra=(float)radius/this->width();

    drop_program->setUniformValue("center",px,py);
    drop_program->setUniformValue("radius",ra);
    drop_program->setUniformValue("strength",strength);
    drop_program->setUniformValue("ratio",m_aspectratio);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,m_Textures[m_texIndex]);

    glDrawArrays(GL_TRIANGLES,0,6);

    drop_program->release();

    this->swapFrameBuffer();

}

void RippleWidget::updateFrame()//draw with update_program,update water height in framebuffers
{
    makeCurrent();

    glBindFramebuffer(GL_FRAMEBUFFER,m_FrameBuffers[1-m_texIndex]);
    m_globVAO.bind();
    update_program->bind();

    update_program->setUniformValue("delta",m_deltx,m_delty);
    update_program->setUniformValue("damping",m_damping);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,m_Textures[m_texIndex]);

    glDrawArrays(GL_TRIANGLES,0,6);
    update_program->release();

    glBindFramebuffer(GL_FRAMEBUFFER,defaultFramebufferObject());
    this->swapFrameBuffer();
}

void RippleWidget::render()//draw with render_program,render the final effect on the screen
{

    glBindFramebuffer(GL_FRAMEBUFFER,defaultFramebufferObject());//bind default framebuffer in QT
    render_program->bind();
    render_program->setUniformValue("samplerBackground",0);
    render_program->setUniformValue("samplerRipples",1);
    render_program->setUniformValue("perturbance",(GLfloat)0.04);
    render_program->setUniformValue("delta",m_deltx,m_delty);

    m_globVAO.bind();

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D,m_Textures[m_texIndex]);//water height

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,m_texture->textureId());//background image

    glDrawArrays(GL_TRIANGLES,0,6);
    render_program->release();

}

void RippleWidget::setRadius(int radius)
{
    m_radius=radius;
}

void RippleWidget::setStrength(GLfloat strength)
{
    if(strength>0&&strength<1)
    {
        m_strength=strength;
    }
}

void RippleWidget::setResolution(GLfloat resolution)
{
    if(resolution>0)
    {
        m_resolution=resolution;
    }
}

void RippleWidget::setDamping(GLfloat damping)
{
    if(damping>=1||damping<0)
    {
        return;
    }
    m_damping=damping;
}

void RippleWidget::setBackgroundImage(QString filename)
{
    makeCurrent();
    if(QFile::exists(filename))
    {
        QFileInfo fi(filename);
        m_backgroundImg=fi.absoluteFilePath();
        if(m_texture)
        {
            m_texture->destroy();
            delete m_texture;
            m_texture=new QOpenGLTexture(QImage(filename).mirrored());

        }

    }
}

void RippleWidget::accEvent(QEvent *ev)
{
    this->event(ev);
}

bool RippleWidget::eventFilter(QObject *watched, QEvent *event)
{
    if(event->type()==QEvent::MouseButtonPress||event->type()==QEvent::MouseMove)
    {
        this->event(event);
    }
    if(event->type()==QEvent::Resize)
    {
        this->resize(this->parentWidget()->size());
    }
    return QOpenGLWidget::eventFilter(watched,event);
}
