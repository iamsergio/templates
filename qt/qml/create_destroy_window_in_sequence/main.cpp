#include <QQuickView>
#include <QGuiApplication>
#include <QTimer>
#include <QCommandLineParser>


#include <QDirIterator>
#include <QDebug>

static bool s_singleWindow = false;

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

    if (!s_singleWindow) {
        QTimer::singleShot(1000, [win] {
            win->deleteLater();
            QTimer::singleShot(2000, [] {
                createWindow();
            });
        });
    }

    return win;
}

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);

    QCommandLineParser parser;
    parser.setApplicationDescription("Window test");
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption singleWindowOption("single-window", "Run with a single window only, without recreating it");
    parser.addOption(singleWindowOption);

    parser.process(app);

    s_singleWindow = parser.isSet(singleWindowOption);

    createWindow();
    // printQrcResourcesRecursively();

    app.exec();
}
