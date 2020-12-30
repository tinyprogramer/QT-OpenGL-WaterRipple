#include "mywidget.h"

#include <QOpenGLShaderProgram>

#include <QDebug>
#include <QFile>

static GLfloat vertArray[]={
    -1.0,1.0,
    -1.0,-1.0,
    1.0,-1.0,
    1.0,1.0,
    -1.0,1.0,
    1.0,-1.0
};


MyWidget::MyWidget(QWidget* parent)
    :QOpenGLWidget(parent),m_texture(nullptr)
{

    //this->setFixedSize(1024,1024);
    this->setMouseTracking(true);

    m_texIndex=0;
    m_radius=20;
    m_strength=0.01;
    m_resolution=2.0;
    m_damping=0.995;
    m_backgroundImg=":/img/bg3.jpg";
}

MyWidget::~MyWidget()
{
    makeCurrent();
    if(m_texture)
    {
        m_texture->destroy();
        delete m_texture;
    }
    m_globVAO.destroy();
    m_globVBO.destroy();
    doneCurrent();
}

void MyWidget::swapFrameBuffer()
{
    m_texIndex=1-m_texIndex;
}

void MyWidget::initializeGL()
{

    initializeOpenGLFunctions();

    glClearColor(0.0f,0.0f,0.0f,0.0f);

    m_globVAO.create();
    m_globVAO.bind();

    m_globVBO.create();
    m_globVBO.bind();

    m_globVBO.allocate(vertArray,sizeof(vertArray));

    drop_program=new QOpenGLShaderProgram;
    initProgram(":/glob.vert",":/drop.frag",drop_program);
    drop_program->setAttributeBuffer("vertex",GL_FLOAT,0,2,2*sizeof(GL_FLOAT));
    drop_program->enableAttributeArray("vertex");

    render_program=new QOpenGLShaderProgram;
    initProgram(":/render.vert",":/render.frag",render_program);
    render_program->setAttributeBuffer("vertex",GL_FLOAT,0,2,2*sizeof(GL_FLOAT));
    render_program->enableAttributeArray("vertex");


    update_program=new QOpenGLShaderProgram;
    initProgram(":/glob.vert",":/update.frag",update_program);
    update_program->setAttributeBuffer("vertex",GL_FLOAT,0,2,2*sizeof(GL_FLOAT));
    update_program->enableAttributeArray("vertex");

    m_globVAO.release();
    m_globVBO.release();

    m_texture=new QOpenGLTexture(QImage(m_backgroundImg).mirrored());
    m_texture->setMagnificationFilter(QOpenGLTexture::Linear);

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

void MyWidget::paintGL()
{
    this->updateFrame();
    this->render();

    update();
}

void MyWidget::resizeGL(int width, int height)
{
    //qDebug()<<this->size();
    glDeleteFramebuffers(2,m_FrameBuffers.data());
    glDeleteTextures(2,m_Textures.data());
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

void MyWidget::initProgram(QString vert,QString frag,QOpenGLShaderProgram* pro){
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

void MyWidget::mouseMoveEvent(QMouseEvent* ev)
{
    this->drop(ev->x(),ev->y(),m_radius,m_strength);
}

void MyWidget::mousePressEvent(QMouseEvent *ev)
{
    this->drop(ev->x(),ev->y(),1.5*m_radius,8*m_strength);
}

void MyWidget::drop(int x,int y,int radius,float strength)
{

    makeCurrent();
    drop_program->bind();

    glBindFramebuffer(GL_FRAMEBUFFER,m_FrameBuffers[1-m_texIndex]);
    m_globVAO.bind();
    float px=(float)(2*x-this->width())/this->width(),py=-(float)(2*y-this->height())/this->height();
    float ra=(float)radius/this->width();
    //GLfloat rate=(GLfloat)this->height()/(GLfloat)this->width();

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

void MyWidget::updateFrame()
{
    makeCurrent();

    glBindFramebuffer(GL_FRAMEBUFFER,m_FrameBuffers[1-m_texIndex]);
    m_globVAO.bind();
    update_program->bind();
    //GLfloat dx=2.0/this->width(),dy=2.0/this->height();

    update_program->setUniformValue("delta",m_deltx,m_delty);
    update_program->setUniformValue("damping",m_damping);

    //GLfloat dx=1.0/512.0,dy=1.0/512.0;
    //update_program->setUniformValue("delta",dx,dy);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,m_Textures[m_texIndex]);

    glDrawArrays(GL_TRIANGLES,0,6);
    update_program->release();

    glBindFramebuffer(GL_FRAMEBUFFER,defaultFramebufferObject());
    this->swapFrameBuffer();
}

void MyWidget::render()
{

    glBindFramebuffer(GL_FRAMEBUFFER,defaultFramebufferObject());
    render_program->bind();
    render_program->setUniformValue("samplerBackground",0);
    render_program->setUniformValue("samplerRipples",1);
    render_program->setUniformValue("perturbance",(GLfloat)0.04);
    //GLfloat deltx=1.0/512.0,delty=1.0/512.0;
    //GLfloat deltx=0.0025,delty=0.0025*this->width()/this->height();
    render_program->setUniformValue("delta",m_deltx,m_delty);
    //render_program->setUniformValue("delta",deltx,delty);
    m_globVAO.bind();

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D,m_Textures[m_texIndex]);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,m_texture->textureId());

    glDrawArrays(GL_TRIANGLES,0,6);
    render_program->release();

}

void MyWidget::setRadius(int radius)
{
    m_radius=radius;
}

void MyWidget::setStrength(GLfloat strength)
{
    if(strength>0&&strength<1)
    {
        m_strength=strength;
    }
}

void MyWidget::setResolution(GLfloat resolution)
{
    if(resolution>0)
    {
        m_resolution=resolution;
    }
}

void MyWidget::setDamping(GLfloat damping)
{
    if(damping>=1||damping<0)
    {
        return;
    }
    m_damping=damping;
}

void MyWidget::setBackgroundImage(QString filename)
{
    makeCurrent();
    if(QFile::exists(filename))
    {
        m_backgroundImg=filename;
        if(m_texture)
        {
            m_texture->destroy();
            delete m_texture;
            m_texture=new QOpenGLTexture(QImage(filename).mirrored());

        }

    }
}
