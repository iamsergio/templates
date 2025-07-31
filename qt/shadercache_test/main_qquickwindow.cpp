#include <QGuiApplication>
#include <QQuickView>

#include <QDebug>
#include <QDirIterator>
#include <QQuickGraphicsConfiguration>
#include <QTimer>

#include <QCommandLineParser>
#include <QtQuick/private/qquickwindow_p.h>
#ifdef SUPPORTS_RHI
#include <rhi/qrhi.h>
#endif

#include "utils.h"

#define SHADER_CACHE_LOCATION "/tmp/qtshadercache_test"

void print_qrc_recursively(const QString &path = ":/") {
  QDirIterator it(path, QDirIterator::Subdirectories);
  while (it.hasNext()) {
    QString filePath = it.next();
    qDebug() << filePath;
  }
}

void checkCacheExists() {
  QFile cacheFile(SHADER_CACHE_LOCATION);
  if (cacheFile.exists()) {
    qDebug() << SHADER_CACHE_LOCATION << "exists";
  } else {
    qDebug() << SHADER_CACHE_LOCATION << "does not exist";
  }
}

#ifdef SUPPORTS_RHI
QRhi *rhiForWindow(QQuickWindow *window) {
  auto priv = QQuickWindowPrivate::get(window);
  return priv->rhi;
}
#endif

void printInfo(QQuickWindow &window) {
  auto ri = window.rendererInterface();

  // auto rhi = rhiForWindow(&window);

  // 3 - opengl ; 5 - vulkan
  qDebug() << "Graphics API:" << ri->graphicsApi()
           << "Shader Compilation Type:" << ri->shaderCompilationType()
           << "Shader Type:" << ri->shaderType();
}

int main(int argc, char **argv) {
  setenv("XDG_CACHE_HOME", SHADER_CACHE_LOCATION, 1); // Qt5 style
  QGuiApplication app(argc, argv);

  QCommandLineParser parser;
  parser.setApplicationDescription("QQuickWindow cache test application");
  parser.addHelpOption();
  parser.addVersionOption();

  QCommandLineOption timeoutOption("delete-view-after-timeout",
                                   "Delete window after a timeout");
  parser.addOption(timeoutOption);
  QCommandLineOption qt5CacheStyleOption("qt5-cache-style",
                                         "Use Qt5 cache style");
  parser.addOption(qt5CacheStyleOption);

  parser.process(app);

  const bool deleteView = parser.isSet(timeoutOption);
  const bool isQt5cachestyle = parser.isSet(qt5CacheStyleOption);

  auto view = new QQuickView();

  system(QString("rm -rf %1").arg(SHADER_CACHE_LOCATION).toStdString().c_str());
  checkCacheExists();

  QQuickGraphicsConfiguration config = view->graphicsConfiguration();

  if (isQt5cachestyle) {
    config.setAutomaticPipelineCache(false);
  } else {
    config.setPipelineCacheSaveFile(QString(SHADER_CACHE_LOCATION));
    config.setPipelineCacheLoadFile(QString(SHADER_CACHE_LOCATION));
  }

  view->setGraphicsConfiguration(config);

  view->setSource(QUrl("qrc:/qt/qml/MyTest/MyTest/main.qml"));
  view->setResizeMode(QQuickView::SizeRootObjectToView);
  view->resize(500, 500);

  QObject::connect(
      view, &QQuickWindow::beforeRendering, view,
      [&view] {
        static bool done = false;
        if (done)
          return;
        done = true;
        printShaderCacheInfos(QOpenGLContext::currentContext());
      },
      Qt::DirectConnection);

  view->show();

  checkCacheExists();

  printInfo(*view);
  // print_qrc_recursively();

  if (deleteView) {
    QTimer::singleShot(3000, [view]() {
      delete view;
      qDebug() << "view was deleted";
      checkCacheExists();
    });
  }

  app.exec();
}
