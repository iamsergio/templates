#include <QGuiApplication>
#include <QQuickView>

#include <QDebug>
#include <QDirIterator>
#include <QQuickGraphicsConfiguration>
#include <QTimer>

#include <QtQuick/private/qquickwindow_p.h>
#include <rhi/qrhi.h>

#include "utils.h"

void print_qrc_recursively(const QString &path = ":/") {
  QDirIterator it(path, QDirIterator::Subdirectories);
  while (it.hasNext()) {
    QString filePath = it.next();
    qDebug() << filePath;
  }
}

void checkCacheExists() {
  QFile cacheFile("/tmp/qquickwindow-cache-test");
  if (cacheFile.exists()) {
    qDebug() << "/tmp/qquickwindow-cache-test exists";
  } else {
    qDebug() << "/tmp/qquickwindow-cache-test does not exist";
  }
}

QRhi *rhiForWindow(QQuickWindow *window) {
  auto priv = QQuickWindowPrivate::get(window);
  return priv->rhi;
}

void printInfo(QQuickWindow &window) {
  auto ri = window.rendererInterface();

  // auto rhi = rhiForWindow(&window);

  // 3 - opengl ; 5 - vulkan
  qDebug() << "Graphics API:" << ri->graphicsApi()
           << "Shader Compilation Type:" << ri->shaderCompilationType()
           << "Shader Type:" << ri->shaderType();
}

int main(int argc, char **argv) {
  QGuiApplication app(argc, argv);

  auto view = new QQuickView();

  system("rm -rf /tmp/qquickwindow-cache-test");
  checkCacheExists();

  QQuickGraphicsConfiguration config = view->graphicsConfiguration();
  config.setPipelineCacheSaveFile(QString("/tmp/qquickwindow-cache-test"));
  config.setPipelineCacheLoadFile(QString("/tmp/qquickwindow-cache-test"));
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

  // QTimer::singleShot(3000, [view]() {
  //   delete view;
  //   qDebug() << "view was deleted";
  //   checkCacheExists();
  // });

  app.exec();
}
