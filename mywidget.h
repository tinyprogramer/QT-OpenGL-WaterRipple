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

QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)

class MyWidget : public QOpenGLWidget,protected QOpenGLFunctions_3_3_Core
{

    Q_OBJECT

public:
    MyWidget(QWidget* parent=0);
    ~MyWidget();
    void printcnt();

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;
    virtual void mouseMoveEvent(QMouseEvent* ev);

private:
    QOpenGLShaderProgram* program,* light_program,* frame_program;
    QOpenGLVertexArrayObject m_vao,m_lightvao,m_framevao;
    QOpenGLFramebufferObject* m_fbo,* tmp_fbo;
    QOpenGLBuffer m_vbo,m_ebo,ins_vbo,m_framevbo;
    QOpenGLTexture* m_texture;
    QOpenGLTexture* m_texture2;
    QOpenGLFramebufferObjectFormat fmt;
    int m_attr;
    int m_color;
    bool ready;
    QTimer m_timer;
    int cnt;

};

#endif // MYWIDGET_H
