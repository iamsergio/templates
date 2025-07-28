// main.cpp
#include <QApplication>
#include <QDebug>
#include <QElapsedTimer>
#include <QMainWindow>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLWidget>

// MyOpenGLWidget class definition
class MyOpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
public:
  MyOpenGLWidget(QWidget *parent = nullptr) : QOpenGLWidget(parent) {}
  ~MyOpenGLWidget() override {
    // Ensure cleanup happens in the correct context
    makeCurrent();
    m_vbo.destroy();
    delete m_program;
    doneCurrent();
  }

protected:
  void initializeGL() override {
    // Start timer to measure initialization time
    QElapsedTimer timer;
    timer.start();

    // Initialize OpenGL functions for the current context
    initializeOpenGLFunctions();

    qDebug() << "OpenGL Version:" << (const char *)glGetString(GL_VERSION);
    qDebug() << "GLSL Version:"
             << (const char *)glGetString(GL_SHADING_LANGUAGE_VERSION);
    qDebug() << "Vendor:" << (const char *)glGetString(GL_VENDOR);
    qDebug() << "Renderer:" << (const char *)glGetString(GL_RENDERER);

    // Define simple vertex and fragment shaders
    // These shaders are intentionally simple to focus on the caching mechanism.
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

    // Create and compile shader program
    m_program = new QOpenGLShaderProgram(this);
    if (!m_program->addShaderFromSourceCode(QOpenGLShader::Vertex,
                                            vShaderSource)) {
      qCritical() << "Vertex shader compilation failed:" << m_program->log();
      return;
    }
    if (!m_program->addShaderFromSourceCode(QOpenGLShader::Fragment,
                                            fShaderSource)) {
      qCritical() << "Fragment shader compilation failed:" << m_program->log();
      return;
    }
    if (!m_program->link()) {
      qCritical() << "Shader program linking failed:" << m_program->log();
      return;
    }

    // --- Shader Caching Explanation ---
    // Qt's QOpenGLShaderProgram, in conjunction with the underlying OpenGL
    // driver, often caches the compiled shader binaries. On the first run, the
    // shaders are compiled from the source code (vShaderSource, fShaderSource).
    // This compilation process can be time-consuming.
    //
    // On subsequent runs (if the shader source code has not changed), the
    // driver/Qt will attempt to load the pre-compiled binary from a cache
    // location (e.g., a file on disk). This loading process is significantly
    // faster than recompilation, demonstrating the effectiveness of the shader
    // cache.
    //
    // The exact cache location and mechanism are typically driver-specific and
    // managed internally by Qt/OpenGL, not directly exposed via a simple API.
    // The most observable effect is the reduced initialization time.

    // Define vertex data for a simple triangle
    GLfloat vertices[] = {
        -0.5f, -0.5f, 0.0f, // Bottom-left
        0.5f,  -0.5f, 0.0f, // Bottom-right
        0.0f,  0.5f,  0.0f  // Top
    };

    // Create and bind Vertex Buffer Object (VBO)
    m_vbo.create();
    m_vbo.bind();
    m_vbo.allocate(vertices, sizeof(vertices));

    // Set up vertex attribute pointers
    // The '0' corresponds to 'layout (location = 0)' in the vertex shader
    m_program->enableAttributeArray(0);
    m_program->setAttributeBuffer(0, GL_FLOAT, 0, 3, 3 * sizeof(GLfloat));

    // Output the time taken for initialization
    qDebug() << "Shader initialization time:" << timer.elapsed() << "ms";
  }

  void resizeGL(int w, int h) override {
    // Adjust viewport to new widget size
    glViewport(0, 0, w, h);
  }

  void paintGL() override {
    // Clear the color buffer
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // Dark teal background
    glClear(GL_COLOR_BUFFER_BIT);

    // Use the shader program
    m_program->bind();

    // Bind the VBO
    m_vbo.bind();

    // Draw the triangle
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // Release resources
    m_vbo.release();
    m_program->release();
  }

private:
  QOpenGLShaderProgram *m_program = nullptr;
  QOpenGLBuffer m_vbo;
};

// main function
int main(int argc, char *argv[]) {
  // Set OpenGL format for compatibility
  QSurfaceFormat format;
  format.setDepthBufferSize(24);
  format.setVersion(3, 3); // Request OpenGL 3.3 Core Profile
  format.setProfile(QSurfaceFormat::CoreProfile);
  QSurfaceFormat::setDefaultFormat(format);

  QApplication a(argc, argv);

  QMainWindow window;
  window.setWindowTitle("Qt Shader Cache Test");
  window.resize(600, 400);

  MyOpenGLWidget *glWidget = new MyOpenGLWidget(&window);
  window.setCentralWidget(glWidget);

  window.show();

  return a.exec();
}
