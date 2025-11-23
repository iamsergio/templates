#include <QQuickView>
#include <QGuiApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);

    QCommandLineParser parser;
    parser.addHelpOption();
    QCommandLineOption timingsOption("timings", "Enable QSG render timings");
    parser.addOption(timingsOption);
    parser.process(app);

    if (parser.isSet(timingsOption)) {
        qputenv("QSG_RENDER_TIMING", "1");
    }

    QQuickView view;
    view.setSource(QUrl("qrc:/qt/qml/MyTest/main.qml"));
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.resize(500, 500);
    view.show();

    app.exec();
}
