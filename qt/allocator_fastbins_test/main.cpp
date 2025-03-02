#include <QtWidgets>

int main(int argc, char **argv) {
  QApplication app(argc, argv);

  QVector<QObject *> objects;
  objects.reserve(1000000);
  for (int i = 0; i < 1000000; i++) {
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
