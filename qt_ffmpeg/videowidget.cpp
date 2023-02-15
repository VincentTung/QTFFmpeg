#include "videowidget.h"

void VideoWidget::initializeGL() {
    initializeOpenGLFunctions();

    //初始化等相关加载工作
    // ......
    // ......

    QTimer *time = new QTimer(this);
    connect(time, SIGNAL(timeout()), this, SLOT(update()));
    time->start(15);
}
void VideoWidget::resizeGL(int w, int h) {
    LOG(INFO) << "resizeGL: " << w << "," << h;
    viewPortWidth_ = w;
    viewPortHeight_ = h;
}
GLuint VideoWidget::InitShader(GLenum type, const GLchar *code) {
    GLuint shader = glCreateShader(type);
    if (shader == 0 || shader == GL_INVALID_ENUM) {
        LOG(ERROR) << "Failed to create shader";
        return 0;
    }

    glShaderSource(shader, 1, &code, NULL);
    glCompileShader(shader);

#ifdef DEBUG
    GLint logLength;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
        GLchar *log = (GLchar *)malloc(logLength);
        glGetShaderInfoLog(shader, logLength, &logLength, log);
        LOG(INFO) << "Shader compile log : " << log;
        free(log);
    }
#endif

    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        glDeleteShader(shader);
        LOG(ERROR) << "Failed to compile shader";
        return 0;
    }

    return shader;
}


void VideoWidget::paintGL() {
    //需要获取frame
    //......
    //......
    auto frame = mediaSamples[0]->GetFrame();

    glViewport(0, 0, viewPortWidth_, viewPortHeight_);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenTextures(3, textures.data());
    int width = frame->width;
    int height = frame->height;
    int widths[3] = { width, width >> 1, width >> 1 };
    int heights[3] = { height, height >> 1, height >> 1 };
    //支持的是yuv格式数据
    for (int i = 0; i < 3; ++i) {
        glPixelStorei(GL_UNPACK_ROW_LENGTH, frame->linesize[i]);
        glBindTexture(GL_TEXTURE_2D, textures[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, widths[i], heights[i],
            0, GL_LUMINANCE, GL_UNSIGNED_BYTE, frame->data[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }
    glBindTexture(GL_TEXTURE_2D, 0);

    program = glCreateProgram();
    GLuint vShader = InitShader(GL_VERTEX_SHADER, vertexShaderSource);
    if (!vShader) {
        glDeleteShader(vShader);
        CHECK(false) << "CreateShader GL_VERTEX_SHADER failed";
    }
    GLuint fShader = InitShader(GL_FRAGMENT_SHADER, yuvFragmentShaderSource);
    if (!fShader) {
        glDeleteShader(fShader);
        CHECK(false) << "CreateShader GL_VERTEX_SHADER failed";
    }
    glAttachShader(program, vShader);
    glAttachShader(program, fShader);
    glBindAttribLocation(program, 0, "position");
    glBindAttribLocation(program, 1, "texcoord");
    glLinkProgram(program);
    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
        glDeleteProgram(program);
        CHECK(false) << "failed to link program : " << program;
    }
    glValidateProgram(program);
#ifdef DEBUG
    GLint logLength;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
        GLchar *log = (GLchar *)malloc(logLength);
        glGetProgramInfoLog(program, logLength, &logLength, log);
        LOG(INFO) << "program validate log : " << log;
        free(log);
    }
#endif
    glGetProgramiv(program, GL_VALIDATE_STATUS, &status);
    if (status == GL_FALSE) {
        LOG(INFO) << "ValidateProgram failed";
        exit(1);
    }
    glUseProgram(program);
    glUniform1i(glGetUniformLocation(program, "s_texture_y"), 0);
    glUniform1i(glGetUniformLocation(program, "s_texture_u"), 1);
    glUniform1i(glGetUniformLocation(program, "s_texture_v"), 2);

    for (int i = 0; i < 3; ++i) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, textures[i]);
    }
    glVertexAttribPointer((GLuint)ATTRIBUTE_COORD::ATTRIBUTE_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, verCoords);
    glEnableVertexAttribArray((GLuint)ATTRIBUTE_COORD::ATTRIBUTE_VERTEX);
    glVertexAttribPointer((GLuint)ATTRIBUTE_COORD::ATTRIBUTE_TEXCOORD, 2, GL_FLOAT, GL_FALSE, 0, texCoords);
    glEnableVertexAttribArray((GLuint)ATTRIBUTE_COORD::ATTRIBUTE_TEXCOORD);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}
