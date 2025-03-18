#include <QtCore>

QTimer *allocate_every(int n_seconds)
{
    auto timer = new QTimer();
    timer->setInterval(std::chrono::seconds(n_seconds));

    timer->callOnTimeout([] {
        // 10mb
        void *ptr = malloc(10 * 1024 * 1024);
    });

    timer->start();
    return timer;
}


int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);

    QCommandLineParser parser;
    parser.setApplicationDescription("Allocate memory every n seconds");
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption durationOption(QStringList() << "d" << "duration", "Duration in seconds to run before stopping", "duration", "0");
    parser.addOption(durationOption);

    parser.process(app);

    auto timer = allocate_every(1);

    int duration = parser.value(durationOption).toInt();
    if (duration <= 0) {
        duration = 30;
    }

    qDebug() << "Running for" << duration << "seconds.";

    QTimer::singleShot(duration * 1000, &app, &QCoreApplication::quit);

    app.exec();

    delete timer;
}
