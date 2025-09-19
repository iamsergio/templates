#include <QtCore>

void create_timers(int n)
{
    for (int i = 0; i < n; ++i) {
        auto timer = new QTimer();
        timer->setInterval(5000);
        timer->callOnTimeout([] {
            static int count = 0;
            count++;
            qDebug() << "Called" << count;
        });
        timer->start();
    }

    qDebug() << "Finished creating timers" << n;
}


int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);

    if (argc < 2) {
        qDebug() << "Usage:" << argv[0] << "<number_of_timers>";
        return 1;
    }

    int n = QString(argv[1]).toInt();
    create_timers(n);

    return app.exec();
}
