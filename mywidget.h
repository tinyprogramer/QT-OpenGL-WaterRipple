#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLFramebufferObject>
#include <QOpenGLFramebufferObjectFormat>
#include <QOpenGLTexture>
#include <QMouseEvent>
#include <QOpenGLShaderProgram>
#include <vector>
#include <QString>

class MyWidget : public QOpenGLWidget,protected QOpenGLFunctions
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
    QOpenGLShaderProgram *drop_program,*render_program,*update_program;
    QOpenGLVertexArrayObject m_globVAO;

    std::vector<unsigned int> m_FrameBuffers,m_Textures;
    QOpenGLBuffer m_globVBO;
    QOpenGLTexture* m_texture;
    int m_texIndex;
    int m_radius;

};

#endif // MYWIDGET_H
