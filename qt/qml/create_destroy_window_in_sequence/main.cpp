#include <QQuickView>
#include <QGuiApplication>
#include <QTimer>
#include <QCommandLineParser>


#include <QDirIterator>
#include <QDebug>
#include <QQmlContext>
#include <QQmlEngine>

static bool s_singleWindow = false;
static int s_windowIndex = 0;

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
    s_windowIndex++;

    qDebug() << "Creating window";
    auto win = new QQuickView();
    QColor c = QColor::fromHsl(s_windowIndex * 60 % 360, 200, 150);
    win->rootContext()->setContextProperty("_color", c);
    win->setSource(QUrl("qrc:/qt/qml/MyTest/MyTest/main.qml"));
    win->setResizeMode(QQuickView::SizeRootObjectToView);
    win->resize(500, 500);
    win->show();

    if (!s_singleWindow) {
        QTimer::singleShot(2000, [win] {
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

    QCommandLineOption dontDestroyOption("dont-destroy", "Dont destroy window after timeout");
    parser.addOption(dontDestroyOption);

    QCommandLineOption numWindowsOption("num-windows", "Specify the number of windows to create", "count", "1");
    parser.addOption(numWindowsOption);

    parser.process(app);

    s_singleWindow = parser.isSet(dontDestroyOption);

    int numWindows = parser.value(numWindowsOption).toInt();
    for (int i = 0; i < numWindows; ++i) {
        createWindow();
    }

    // printQrcResourcesRecursively();
    //
    qDebug() << "Primary screen size:" << QGuiApplication::primaryScreen()->size();

    app.exec();
}
