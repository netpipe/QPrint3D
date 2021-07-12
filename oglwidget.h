#ifndef OGLWIDGET_H
#define OGLWIDGET_H
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>

QT_BEGIN_NAMESPACE
class QOpenGLShaderProgram;
class QOpenGLTexture;
QT_END_NAMESPACE

class OGLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT
public:
    explicit OGLWidget(QWidget* parent = 0);
    ~OGLWidget();

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;
    void rotateBy(int xAngle, int yAngle, int zAngle);

    void setClearColor(const QColor& color);

signals:
    void clicked();

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;

    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    void makeObject();

    QColor clearColor;
    QPoint lastPos;
    int xRot, yRot, zRot;

    QOpenGLTexture* textures[6];
    QOpenGLShaderProgram* program;
    QOpenGLBuffer vbo;
};

#endif // OGLWIDGET_H
