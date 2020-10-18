#include "mywidget.h"

#include <QOpenGLShaderProgram>

//#include <QOpenGLFunctions_3_2_Core>
#include <QDebug>
#include <QDateTime>

static GLfloat vertices[]={
    -1.0f,1.0f,0.0f,0.0f,1.0f,
    -1.0f,-1.0f,0.0f,0.0f,0.0f,
    1.0f,-1.0f,0.0f,1.0f,0.0f,
    1.0f,1.0f,0.0f,1.0f,1.0f,
    -1.0f,1.0f,0.0f,0.0f,1.0f,
    1.0f,-1.0f,0.0f,1.0f,0.0f,
};

static GLfloat vertArray[]={
    -1.0,1.0,
    -1.0,-1.0,
    1.0,-1.0,
    1.0,1.0,
    -1.0,1.0,
    1.0,-1.0
};

//static GLfloat vertices[] = {
//    // 位置              // 颜色
//     0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // 右下
//    -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // 左下
//     0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // 顶部
//};

static float s=1.0f;

//static GLfloat vertices[] = {
////     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
//        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
//         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
//         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
//         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
//        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
//        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
//
//        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
//         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
//         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
//         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
//        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
//        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
//
//        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
//        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
//        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
//        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
//        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
//        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
//
//         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
//         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
//         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
//         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
//         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
//         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
//
//        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
//         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
//         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
//         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
//        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
//        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
//
//        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
//         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
//         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
//         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
//        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
//        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
//};

static GLfloat frameBuffer[]={
    -1.0f,1.0f,0.0f,0.0f,1.0f,
    -1.0f,-1.0f,0.0f,0.0f,0.0f,
    1.0f,-1.0f,0.0f,1.0f,0.0f,
    1.0f,1.0f,0.0f,1.0f,1.0f,
    -1.0f,1.0f,0.0f,0.0f,1.0f,
    1.0f,-1.0f,0.0f,1.0f,0.0f,
};

static GLfloat texCoords[] = {
    0.0f, 0.0f, // 左下角
    1.0f, 0.0f, // 右下角
    0.5f, 1.0f // 上中
};

static GLfloat ins[]={-2.0f,-2.0f,0.0f,-2.0f,2.0f,0.0f,2.0f,2.0f,0.0f,2.0f,-2.0f,0.0f};

static unsigned int indices[] = { // 注意索引从0开始!
    0, 1, 3, // 第一个三角形
    1, 2, 3,  // 第二个三角形
    4,5,7,
    5,6,7,
                                  4,5,0,
                                  5,1,0,
                                  3,2,7,
                                  2,6,7,
                                  4,0,7,
                                  0,3,7,
                                  5,1,6,
                                  1,2,6
};

MyWidget::MyWidget(QWidget* parent)
    :QOpenGLWidget(parent),ready(false),cnt(0),m_texture(nullptr),m_texture2(nullptr)
{
    QObject::connect(&m_timer,&QTimer::timeout,this,&MyWidget::printcnt);
    //this->resize(1024,1024);
    this->setFixedSize(1024,1024);
    this->setMouseTracking(true);
    m_timer.setInterval(1000);
    m_timer.start();
    m_texIndex=0;
    m_radius=20;
}

MyWidget::~MyWidget()
{
    qDebug()<<"in destroy";
    makeCurrent();
    if(m_texture)
    {
        delete m_texture;
    }
    if(m_texture2)
    {
        delete m_texture2;
    }
    m_vao.destroy();
    m_vbo.destroy();
    m_lightvao.destroy();
    m_ebo.destroy();
    doneCurrent();
    qDebug()<<"finish";
}

void MyWidget::swapFrameBuffer()
{
    m_texIndex=1-m_texIndex;
}

void MyWidget::printcnt()
{
    qDebug()<<cnt;
}

void MyWidget::initializeGL()
{

    initializeOpenGLFunctions();

    //fmt.setSamples(16);
    fmt.setTextureTarget(GL_TEXTURE_2D);
    //fmt.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);

    glClearColor(0.0f,0.0f,0.0f,0.0f);


    //glEnable(GL_DEPTH_TEST);
    //glEnable(GL_STENCIL_TEST);
    //glEnable(GL_MULTISAMPLE);
    //glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_FRONT);
    // 创建顶点着色器
    //QOpenGLShader *vshader = new QOpenGLShader(QOpenGLShader::Vertex, this);
    //const char *vsrc =
    //    "#version 330 core\n"
    //    "layout (location = 0) in vec3 aPos;\n"
    //    "void main()\n"
    //    "{\n"
    //    "    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    //    "};\n";
    //vshader->compileSourceCode(vsrc);
    //// 创建片段着色器
    //QOpenGLShader *fshader = new QOpenGLShader(QOpenGLShader::Fragment, this);
    //const char *fsrc =
    //        "void main() {                              \n"
    //        "   gl_FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    //        "}                                          \n";
    //fshader->compileSourceCode(fsrc);

    // 创建着色器程序
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
    qDebug()<<"vert location "<<render_program->attributeLocation("vert");
    render_program->setAttributeBuffer("vertex",GL_FLOAT,0,2,2*sizeof(GL_FLOAT));
    render_program->enableAttributeArray("vertex");
    //render_program->setUniformValue("samplerBackground",0);
    //render_program->setUniformValue("samplerRipples",1);
    //render_program->setUniformValue("perturbance",(GLfloat)0.04);
    //GLfloat deltx=1.0/this->width(),delty=1.0/this->height();
    //qDebug()<<deltx<<" "<<delty;
    //render_program->setUniformValue("delta",deltx,delty);

    update_program=new QOpenGLShaderProgram;
    initProgram(":/glob.vert",":/update.frag",update_program);
    update_program->setAttributeBuffer("vertex",GL_FLOAT,0,2,2*sizeof(GL_FLOAT));
    update_program->enableAttributeArray("vertex");

    m_globVAO.release();
    m_globVBO.release();

    program = new QOpenGLShaderProgram;
    if(!program->addShaderFromSourceFile(QOpenGLShader::Vertex,":/tgl.vert"))
    {
        qDebug()<< (program->log());
        return;
    }
    if(!program->addShaderFromSourceFile(QOpenGLShader::Fragment,":/tgl.frag"))
    {
        qDebug()<< (program->log());
        return;
    }
    if(!program->link())
    {
        qDebug()<< (program->log());
        return;
    }

    program->bind();
    //if (!m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vsrc)) {
    //    emit error(m_program->log());
    //    return;
    //}
    //if (!m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fsrc)) {
    //    emit error(m_program->log());
    //    return;
    //}
    //if (!m_program->link()) {
    //    emit error(m_program->log());
    //    return;
    //}

    m_texture=new QOpenGLTexture(QImage(":/img/bg3.jpg").mirrored());
    m_texture->setMagnificationFilter(QOpenGLTexture::Linear);
    m_texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    m_texture2=new QOpenGLTexture(QImage(":/img/t2.jpg").mirrored());
    //
    program->setUniformValue("screenTexture",0);
    program->setUniformValue("texture2",1);

    m_vao.create();
    m_vao.bind();

    m_vbo.create();
    m_vbo.bind();
    m_vbo.allocate(vertices, sizeof(vertices));

    //m_ebo= QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    //m_ebo.create();
    //m_ebo.bind();
    //m_ebo.allocate(indices,sizeof(indices));
    //m_logoVbo.bind();
    //QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    //f->glEnableVertexAttribArray(0);
    //f->glEnableVertexAttribArray(1);
    //f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
    //f->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<void *>(3 * sizeof(GLfloat)));
    //m_logoVbo.release();
    m_attr=program->attributeLocation("aPos");
    qDebug()<<"m_attr is "<<m_attr;
    m_color=program->attributeLocation("aColor");

    program->setAttributeBuffer(m_attr,GL_FLOAT, 0, 3,5*sizeof(GLfloat));
    program->enableAttributeArray(m_attr);
    //program->setAttributeBuffer(3,GL_FLOAT,3*sizeof(GLfloat),3,6*sizeof(GLfloat));
    //program->enableAttributeArray(3);
    //program->setAttributeBuffer(m_color,GL_FLOAT,3*sizeof(GLfloat),3,8*sizeof(GLfloat));
    //program->enableAttributeArray(m_color);
    program->setAttributeBuffer(2,GL_FLOAT,3*sizeof(GLfloat),2,5*sizeof(GLfloat));
    program->enableAttributeArray(2);
    //glVertexAttribPointer(m_attr,3,GL_FLOAT,GL_FALSE,3*sizeof(float),0);
    //glEnableVertexAttribArray(m_attr);
    //m_vbo.release();

    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0);

    m_vbo.release();
    ins_vbo.create();
    ins_vbo.bind();
    ins_vbo.allocate(ins,sizeof(ins));
    program->setAttributeArray(4,GL_FLOAT,0,3,3*sizeof(GLfloat));
    program->enableAttributeArray(4);
    glVertexAttribDivisor(4,1);
    ins_vbo.release();

    program->release();

    m_vao.release();

    //m_lightvao.create();
    //m_lightvao.bind();
    //
    //m_vbo.bind();
    ////m_ebo.bind();
    //
    //
    //light_program = new QOpenGLShaderProgram;
    //if(!light_program->addShaderFromSourceFile(QOpenGLShader::Vertex,":/tgl.vert"))
    //{
    //    qDebug()<< (light_program->log());
    //    return;
    //}
    //if(!light_program->addShaderFromSourceFile(QOpenGLShader::Fragment,":/light.frag"))
    //{
    //    qDebug()<< (light_program->log());
    //    return;
    //}
    //if(!light_program->link())
    //{
    //    qDebug()<< (light_program->log());
    //    return;
    //}
    //
    ////light_program->setAttributeBuffer(m_attr,GL_FLOAT, 0, 3,6*sizeof(GLfloat));
    ////light_program->enableAttributeArray(m_attr);
    ////light_program->setAttributeBuffer(m_color,GL_FLOAT,3*sizeof(GLfloat),3,8*sizeof(GLfloat));
    ////light_program->enableAttributeArray(m_color);
    ////light_program->setAttributeBuffer(2,GL_FLOAT,6*sizeof(GLfloat),2,8*sizeof(GLfloat));
    ////light_program->enableAttributeArray(2);
    //
    //m_lightvao.release();
    //m_vbo.release();
    ////m_ebo.release();
    //light_program->release();

    frame_program=new QOpenGLShaderProgram;
    if(!frame_program->addShaderFromSourceFile(QOpenGLShader::Vertex,":/fbo.vert"))
    {
        qDebug()<< (frame_program->log());
        return;
    }
    if(!frame_program->addShaderFromSourceFile(QOpenGLShader::Fragment,":/fbo.frag"))
    {
        qDebug()<< (frame_program->log());
        return;
    }
    if(!frame_program->link())
    {
        qDebug()<< (frame_program->log());
        return;
    }
    frame_program->bind();
    m_framevao.create();
    m_framevao.bind();
    m_framevbo.create();
    m_framevbo.bind();
    m_framevbo.allocate(frameBuffer,sizeof(frameBuffer));
    frame_program->setAttributeBuffer(0,GL_FLOAT,0,2,5*sizeof(GL_FLOAT));
    frame_program->enableAttributeArray(0);
    frame_program->setAttributeBuffer(1,GL_FLOAT,3*sizeof(GL_FLOAT),2,5*sizeof(GL_FLOAT));
    frame_program->enableAttributeArray(1);

    frame_program->setUniformValue("screenTexture1",0);
    frame_program->setUniformValue("texture23",1);
    m_framevao.release();
    m_framevbo.release();


    m_frameBuffers.push_back(new QOpenGLFramebufferObject(this->size(),fmt));
    //int a[this->width()][this->height()]={0};
    m_frameBuffers[0]->bind();
    unsigned int texture1;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->width(),this->height(), 0, GL_RGBA, GL_FLOAT, NULL);
    qDebug()<<GL_FLOAT;
    m_frameBuffers[0]->release();
    m_frameBuffers.push_back(new QOpenGLFramebufferObject(this->size(),fmt));
    m_frameBuffers[1]->bind();
    unsigned int texture2;
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->width(),this->height(), 0, GL_RGBA, GL_FLOAT, NULL);
    m_frameBuffers[1]->release();

    m_fbo=new QOpenGLFramebufferObject(this->size(),fmt);
    tmp_fbo=new QOpenGLFramebufferObject(this->size());
    if(m_fbo->isValid())
    {
        qDebug()<<"isValid";
    }
    //m_fbo->setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);

}

void MyWidget::paintGL()
{

    //glViewport(0,0,this->width(),this->height());
    //m_fbo->bind();
    ////glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    ////m_fbo->addColorAttachment(this->size());
    //
    ////m_fbo->addColorAttachment(width(),height());
    //
    ////int w=(float)(m_fbo->width())/(float)(this->width())*m_fbo->width();
    ////int h=(float)(m_fbo->height())/(float)(this->height())*m_fbo->height();
    ////qDebug()<<w<<"  "<<h;
    ////glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->width(),this->height(), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    ////glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_fbo->width(),m_fbo->height(), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    ////glBindTexture(GL_TEXTURE_2D,m_fbo->texture());
    //
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
    //m_vao.bind();
    //
    //program->bind();
    //m_texture->bind(0);
    //m_texture2->bind(1);
    ////glBindTexture(GL_TEXTURE_2D,0);
    //glDrawArrays(GL_TRIANGLES,0,6);
    //
    //m_vao.release();
    //program->release();
    //
    ////QOpenGLFramebufferObject::bindDefault();
    //if(m_fbo->release())
    //{
    //    //qDebug()<<"success";
    //}
    ////tmp_fbo->addColorAttachment(this->size());
    //
    ////QRect r1(0,0,this->width(),this->height()),r2(0,0,this->width(),this->height());
    ////tmp_fbo=new QOpenGLFramebufferObject(this->size());
    ////QRect r1(0,0,tmp_fbo->width(),tmp_fbo->height()),r2(0,0,m_fbo->width(),m_fbo->height());
    //QOpenGLFramebufferObject::blitFramebuffer(tmp_fbo,m_fbo);
    ////tmp_fbo->release();
    ////qDebug()<<m_fbo->size();
    ////qDebug()<<tmp_fbo->size();
 // //返回默认
    ////glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
    //
    //frame_program->bind();
    //m_framevao.bind();
    //
    ////frame_program->setUniformValue("screenTexture",m_fbo->texture());
    ////glBindTexture(GL_TEXTURE_2D,m_fbo->texture());
    //
    ////m_texture2->bind(0);
    ////frame_program->setUniformValue("screenTexture",tmp_fbo->texture());
    //glActiveTexture(GL_TEXTURE0);
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->width(),this->height(), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    ////glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_fbo->width(),m_fbo->height(), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    //glBindTexture(GL_TEXTURE_2D,tmp_fbo->texture());
    ////m_texture2->bind(1);
    ////glBindTexture(GL_TEXTURE_2D,1);
    ////glBindTexture(GL_TEXTURE_2D,0);
    //
    //
    ////glBindTexture(GL_TEXTURE_2D,0);
    ////glBindFramebuffer(GL_FRAMEBUFFER,0);
    //glDrawArrays(GL_TRIANGLES,0,6);
    //frame_program->release();
    ++cnt;
    //this->drop(500,500,40,0.15);
    if(cnt%10==0)
    {
        this->updateFrame();
    }
    this->render();


    update();
}

void MyWidget::resizeGL(int width, int height)
{
    //qDebug()<<this->size();
    //glViewport(0,0,width*2,height*2);
    if(m_fbo)
    {
        //delete m_fbo;
        //m_fbo=new QOpenGLFramebufferObject(this->size(),fmt);
        //m_fbo->addColorAttachment(width,height);
    }
    if(tmp_fbo)
    {
        //delete tmp_fbo;
        //tmp_fbo=new QOpenGLFramebufferObject(this->size());
        //tmp_fbo->addColorAttachment(width,height);
    }
    //delete m_fbo;
    //m_fbo=new QOpenGLFramebufferObject(this->size(),fmt);
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

}

void MyWidget::mousePressEvent(QMouseEvent *ev)
{

    qDebug()<<"mouse pressed";
    //qDebug()<<m_texIndex;
    this->drop(ev->x(),ev->y(),40,0.05);
    //this->render();
}

void MyWidget::drop(int x,int y,int radius,float strength)
{

    makeCurrent();
    drop_program->bind();


    m_frameBuffers[1-m_texIndex]->bind();
    m_globVAO.bind();
    float px=(float)(2*x-this->width())/this->width(),py=-(float)(2*y-this->height())/this->height();
    float ra=(float)radius/this->width();
    //qDebug()<<px;
    //qDebug()<<py;
    drop_program->setUniformValue("center",px,py);
    drop_program->setUniformValue("radius",ra);
    drop_program->setUniformValue("strength",strength);
    //drop_program->setUniformValue("texture",0);
    //m_texture2->bind();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,m_frameBuffers[m_texIndex]->texture());
    //glBindTexture(GL_TEXTURE_2D,m_texture2->textureId());
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->width(),this->height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glDrawArrays(GL_TRIANGLES,0,6);
    m_frameBuffers[1-m_texIndex]->release();
    drop_program->release();

    this->swapFrameBuffer();

}

void MyWidget::updateFrame()
{
    makeCurrent();
    m_frameBuffers[1-m_texIndex]->bind();
    m_globVAO.bind();
    update_program->bind();
    GLfloat dx=1.0/this->width(),dy=1.0/this->height();
    //qDebug()<<dx<<" "<<dy;
    update_program->setUniformValue("delta",dx,dy);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->width(),this->height(), 0, GL_RGBA, GL_FLOAT, NULL);
    glActiveTexture(GL_TEXTURE0);
    //glBindTexture(GL_TEXTURE_2D,m_frameBuffers[m_texIndex]->texture());
    glBindTexture(GL_TEXTURE_2D,m_frameBuffers[m_texIndex]->texture());

    glDrawArrays(GL_TRIANGLES,0,6);
    update_program->release();
    m_frameBuffers[1-m_texIndex]->release();
    this->swapFrameBuffer();
}

void MyWidget::render()
{
    //m_frameBuffers[1-m_texIndex]->release();
    //glBindFramebuffer(GL_FRAMEBUFFER,NULL);
    glBindFramebuffer(GL_FRAMEBUFFER,defaultFramebufferObject());
    render_program->bind();
    render_program->setUniformValue("samplerBackground",0);
    render_program->setUniformValue("samplerRipples",1);
    render_program->setUniformValue("perturbance",(GLfloat)0.04);
    GLfloat deltx=1.0/this->width(),delty=1.0/this->height();
    render_program->setUniformValue("delta",deltx,delty);
    m_globVAO.bind();
    //glClearColor(1.0f, 0.3f, 0.3f, 1.0f);
    //glClear(GL_COLOR_BUFFER_BIT);
    glActiveTexture(GL_TEXTURE1);
    //glBindTexture(GL_TEXTURE_2D,m_texture->textureId());
    //m_texture->bind(1);
    glBindTexture(GL_TEXTURE_2D,m_frameBuffers[m_texIndex]->texture());
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,m_texture->textureId());
    //m_texture2->bind(0);
    glDrawArrays(GL_TRIANGLES,0,6);
    render_program->release();

  //  program->bind();
  //  m_vao.bind();
  //  m_texture->bind();
  //  glDrawArrays(GL_TRIANGLES,0,6);
  //  program->release();
}
