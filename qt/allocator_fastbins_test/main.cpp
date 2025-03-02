#include <QtWidgets>

#include "malloc.h"

int main(int argc, char **argv) {
  QApplication app(argc, argv);

  const int numObjects = (argc > 1) ? atoi(argv[1]) : 1000000;

  qDebug() << "Creating numObjects" << numObjects;

  QVector<QObject *> objects;
  objects.reserve(numObjects);
  for (int i = 0; i < numObjects; i++) {
    objects.push_back(new QObject());
  }

  QWidget window;
  auto layout = new QVBoxLayout(&window);
  auto button = new QPushButton("Delete Objects");
  auto evenButton = new QPushButton("Delete Even Objects");
  layout->addWidget(button);
  layout->addWidget(evenButton);
  window.show();

  QObject::connect(button, &QPushButton::clicked, [&objects]() {
    qDeleteAll(objects);
    objects.clear();
    objects.squeeze();
    // malloc_trim(0);
  });

  QObject::connect(evenButton, &QPushButton::clicked, [&objects]() {
    for (int i = 0; i < objects.size(); i++) {
      if (i % 2 == 0) {
        delete objects[i];
        objects[i] = nullptr;
      }
    }
    objects.removeAll(nullptr);
  });

  return app.exec();
}
