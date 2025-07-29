// main.cpp
#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QElapsedTimer>
#include <QMainWindow>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLWidget>

#include "utils.h"

class MyOpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
public:
  MyOpenGLWidget(QWidget *parent = nullptr) : QOpenGLWidget(parent) {}
  ~MyOpenGLWidget() override {
    makeCurrent();
    m_vbo.destroy();
    delete m_program;
    doneCurrent();
  }

protected:
  void initializeGL() override {
    QElapsedTimer timer;
    timer.start();

    initializeOpenGLFunctions();

    qDebug() << "OpenGL Version:" << (const char *)glGetString(GL_VERSION);
    qDebug() << "GLSL Version:"
             << (const char *)glGetString(GL_SHADING_LANGUAGE_VERSION);
    qDebug() << "Vendor:" << (const char *)glGetString(GL_VENDOR);
    qDebug() << "Renderer:" << (const char *)glGetString(GL_RENDERER);

    const char *vShaderSource =
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        "    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\n";

    const char *fShaderSource =
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f); // Orange color\n"
        "}\n";

    m_program = new QOpenGLShaderProgram(this);
    if (!m_program->addCacheableShaderFromSourceCode(QOpenGLShader::Vertex,
                                                     vShaderSource)) {
      qCritical() << "Vertex shader compilation failed:" << m_program->log();
      return;
    }

    if (!m_program->addCacheableShaderFromSourceCode(QOpenGLShader::Fragment,
                                                     fShaderSource)) {
      qCritical() << "Fragment shader compilation failed:" << m_program->log();
      return;
    }

    if (!m_program->link()) {
      qCritical() << "Shader program linking failed:" << m_program->log();
      return;
    }

    GLfloat vertices[] = {
        -0.5f, -0.5f, 0.0f, // Bottom-left
        0.5f,  -0.5f, 0.0f, // Bottom-right
        0.0f,  0.5f,  0.0f  // Top
    };

    m_vbo.create();
    m_vbo.bind();
    m_vbo.allocate(vertices, sizeof(vertices));

    m_program->enableAttributeArray(0);
    m_program->setAttributeBuffer(0, GL_FLOAT, 0, 3, 3 * sizeof(GLfloat));

    printShaderCacheInfos(QOpenGLContext::currentContext());

    qDebug() << "Shader initialization time:" << timer.elapsed() << "ms";
  }

  void resizeGL(int w, int h) override { glViewport(0, 0, w, h); }

  void paintGL() override {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // Dark teal background
    glClear(GL_COLOR_BUFFER_BIT);

    m_program->bind();

    m_vbo.bind();

    glDrawArrays(GL_TRIANGLES, 0, 3);

    m_vbo.release();
    m_program->release();
  }

private:
  QOpenGLShaderProgram *m_program = nullptr;
  QOpenGLBuffer m_vbo;
};

int main(int argc, char *argv[]) {

  system("rm -rf /tmp/qtshadercache_tes/");
  setenv("XDG_CACHE_HOME", "/tmp/qtshadercache_test", 1);

  QSurfaceFormat format;
  format.setDepthBufferSize(24);
  format.setVersion(3, 3);
  format.setProfile(QSurfaceFormat::CoreProfile);
  QSurfaceFormat::setDefaultFormat(format);

  QApplication a(argc, argv);

  QMainWindow window;
  window.setWindowTitle("Qt Shader Cache Test");
  window.resize(600, 400);

  MyOpenGLWidget *glWidget = new MyOpenGLWidget(&window);
  window.setCentralWidget(glWidget);

  window.show();

  int fileCount = 0;
  FILE *fp = popen("find /tmp/qtshadercache_test -type f | wc -l", "r");
  if (fp) {
    fscanf(fp, "%d", &fileCount);
    pclose(fp);
  }

  if (fileCount == 0) {
    qDebug() << "/tmp/qtshadercache_test has no files. Caching FAILED!";
    return -1;
  } else {
    qDebug() << "/tmp/qtshadercache_test has files:" << fileCount;
    return 0;
  }

  a.exec();
}
