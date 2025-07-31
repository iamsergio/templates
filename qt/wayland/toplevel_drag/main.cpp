#include <QtWidgets>
#include <QGuiApplication>
#include <qpa/qplatformnativeinterface.h>


wl_display *getWLDisplay()
{
    QPlatformNativeInterface *nativeInterface = QGuiApplication::platformNativeInterface();
    if (!nativeInterface) {
        qDebug() << "Not running on a native platform interface or Wayland plugin not loaded.";
        return nullptr;
    }
    
    void *wlDisplayPtr = nativeInterface->nativeResourceForIntegration("wl_display");
    if (!wlDisplayPtr) {
        qDebug() << "No wl_display found. Not a Wayland session or Wayland plugin issue.";
        return nullptr;
    }
    return static_cast<struct wl_display *>(wlDisplayPtr);
}

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    QWidget window;
    QVBoxLayout *layout = new QVBoxLayout(&window);
    QPushButton *button = new QPushButton("Click me", &window);
    layout->addWidget(button);
    QObject::connect(button, &QPushButton::pressed, [&window](){
        
        qDebug() << "Start";
        QDrag drag(&window);
        
        auto mimeData = new QMimeData();
        auto qwindow = window.windowHandle();
    
        auto serialize = [](const auto &object) {
            QByteArray data;
            QDataStream dataStream(&data, QIODevice::WriteOnly);
            dataStream << object;
            return data;
        };
        mimeData->setData("application/x-qt-mainwindowdrag-window",
                          serialize(reinterpret_cast<qintptr>(qwindow)));
        mimeData->setData("application/x-qt-mainwindowdrag-position", serialize(QCursor::pos()));
        drag.setMimeData(mimeData);
        
        drag.exec();
        qDebug() << "Done";
        
        
    });
    window.show();

    
    return app.exec();
}
