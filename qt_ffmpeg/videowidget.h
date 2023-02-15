#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H

#include <QOpenGlWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>

//顶点Shader
static const char vertexShaderSource[] =
"attribute vec4 position;           \n"
"attribute vec2 texcoord;           \n"
"varying vec2 v_texcoord;           \n"
"void main()                        \n"
"{                                  \n"
"   gl_Position = position;         \n"
"   v_texcoord = texcoord.xy;       \n"
"}                                  \n";

//片元Shader
static const char yuvFragmentShaderSource[] =
"precision mediump float;                              \n"
"varying vec2 v_texcoord;                              \n"
"uniform sampler2D s_texture_y;                        \n"
"uniform sampler2D s_texture_u;                        \n"
"uniform sampler2D s_texture_v;                        \n"
"void main()                                           \n"
"{                                                     \n"
"   vec3 yuv;                                          \n"
"   vec3 rgb;                                          \n"
"   yuv.r = texture2D(s_texture_y, v_texcoord).r;      \n"
"   yuv.g = texture2D(s_texture_u, v_texcoord).r - 0.5;\n"
"   yuv.b = texture2D(s_texture_v, v_texcoord).r - 0.5;\n"
"   rgb = mat3(1.0, 1.0, 1.0,                          \n"
"              0.0, -0.39465, 2.03211,                 \n"
"              1.13983, -0.58060, 0.0)*yuv;            \n"
"   gl_FragColor = vec4(rgb,1.0);                      \n"
"}                                                     \n";

//Vertex coordinates
static const GLfloat verCoords[] = {
    -1.0f, -1.0f,
     1.0f, -1.0f,
    -1.0f,  1.0f,
     1.0f,  1.0f
};

//Material coordinates
static const GLfloat texCoords[] = {
    0.0f, 1.0f,
    1.0f, 1.0f,
    0.0f, 0.0f,
    1.0f, 0.0f
};

class VideoWidget : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

public:
    explicit VideoWidget(QWidget *parent = 0);
    ~VideoWidget() override;
    GLuint VideoWidget::InitShader(GLenum type, const GLchar *code);

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

private:
    int viewPortWidth_ = 0;
    int viewPortHeight_ = 0;
    GLuint program;
    std::array<GLuint, 3> textures;

};

#endif // VIDEOWIDGET_H
