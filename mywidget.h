#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLBuffer>
#include <QOpenGLFramebufferObject>
#include <QOpenGLFramebufferObjectFormat>
#include <QOpenGLTexture>
#include <QTimer>
#include <QMouseEvent>
#include <QOpenGLShaderProgram>
#include <vector>
#include <QString>

class MyWidget : public QOpenGLWidget,protected QOpenGLFunctions_3_3_Core
{

    Q_OBJECT

public:
    MyWidget(QWidget* parent=0);
    ~MyWidget();
    void printcnt();
    void drop(int x,int y,int radius,float strength);
    void render();
    void updateFrame();

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;
    virtual void mouseMoveEvent(QMouseEvent* ev);
    virtual void mousePressEvent(QMouseEvent* ev);
    void swapFrameBuffer();
    void initProgram(QString vert,QString frag,QOpenGLShaderProgram* pro);

private:
    QOpenGLShaderProgram* program,* light_program,* frame_program,*drop_program,*render_program,*update_program;
    QOpenGLVertexArrayObject m_vao,m_lightvao,m_framevao,m_globVAO;
    QOpenGLFramebufferObject* m_fbo,* tmp_fbo;
    std::vector<QOpenGLFramebufferObject*> m_frameBuffers;
    QOpenGLBuffer m_vbo,m_ebo,ins_vbo,m_framevbo,m_globVBO;
    QOpenGLTexture* m_texture;
    QOpenGLTexture* m_texture2;
    QOpenGLFramebufferObjectFormat fmt;
    int m_attr;
    int m_color;
    bool ready;
    QTimer m_timer;
    int cnt;
    int m_texIndex;
    int m_radius;

};

#endif // MYWIDGET_H
