#ifndef RIPPLEWIDGET_H
#define RIPPLEWIDGET_H

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

class RippleWidget : public QOpenGLWidget,protected QOpenGLFunctions
{

    Q_OBJECT

public:
    RippleWidget(QWidget* parent=0,bool insfilter=true);
    ~RippleWidget();
    void printcnt();
    void accEvent(QEvent* ev);
    void drop(int x,int y,int radius,float strength);

    void setRadius(int radius);
    void setStrength(GLfloat strength);
    void setResolution(GLfloat resolution);
    void setDamping(GLfloat damping);
    void setBackgroundImage(QString filename);

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;
    virtual void mouseMoveEvent(QMouseEvent* ev);
    virtual void mousePressEvent(QMouseEvent* ev);
    void swapFrameBuffer();
    void render();
    void updateFrame();
    void initProgram(QString vert,QString frag,QOpenGLShaderProgram* pro);
    void initProgram(const char* vert,const char* frag,QOpenGLShaderProgram* pro);
    bool eventFilter(QObject *watched, QEvent *event);


private:
    QOpenGLShaderProgram *drop_program,*render_program,*update_program;
    QOpenGLVertexArrayObject m_globVAO;

    std::vector<unsigned int> m_FrameBuffers,m_Textures;
    QOpenGLBuffer m_globVBO;
    QOpenGLTexture* m_texture;
    int m_texIndex;
    int m_radius;
    GLfloat m_deltx,m_delty,m_strength,m_resolution,m_aspectratio,m_damping;
    QString m_backgroundImg;

};

#endif // RIPPLEWIDGET_H
