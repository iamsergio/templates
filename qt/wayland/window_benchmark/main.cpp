

#include <QUrl>
#include <QDebug>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QQmlContext>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts, true);
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine appEngine;

    QCommandLineParser parser;
    parser.setApplicationDescription("Application description");
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption noSurfaceOption("no-surface", "Disable surface rendering");
    parser.addOption(noSurfaceOption);
    
    QCommandLineOption waylandSocketNameOption("wayland-socket-name", "Specify the Wayland socket name", "name");
    parser.addOption(waylandSocketNameOption);

    QCommandLineOption fullScreenOption("fullscreen", "If fullscreen or not");
    parser.addOption(fullScreenOption);
       
    parser.process(app);

    const bool noSurface = parser.isSet(noSurfaceOption);

    appEngine.rootContext()->setContextProperty("_noSurface", noSurface);
    appEngine.rootContext()->setContextProperty("_fullscreen", parser.isSet(fullScreenOption));
    appEngine.load(QUrl("qrc:///main.qml"));

    return app.exec();
}
