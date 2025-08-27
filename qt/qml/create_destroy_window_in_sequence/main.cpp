#include <QQuickView>
#include <QGuiApplication>
#include <QTimer>
#include <QCommandLineParser>


#include <QDirIterator>
#include <QDebug>

void printQrcResourcesRecursively(const QString &path = ":/")
{
    QDirIterator it(path, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        QString resource = it.next();
        qDebug() << "Resource:" << resource;
    }
}

QQuickView *createWindow()
{
    qDebug() << "Creating window";
    auto win = new QQuickView();
    win->setSource(QUrl("qrc:/qt/qml/MyTest/MyTest/main.qml"));
    win->setResizeMode(QQuickView::SizeRootObjectToView);
    win->resize(500, 500);
    win->show();

    QTimer::singleShot(1000, [win] {
        delete win;
        QTimer::singleShot(1000, [] {
            createWindow();
        });
    });

    return win;
}

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);

    QCommandLineParser parser;
    parser.setApplicationDescription("Window test");
    parser.addHelpOption();
    parser.addVersionOption();

    createWindow();
    printQrcResourcesRecursively();

    app.exec();
}
