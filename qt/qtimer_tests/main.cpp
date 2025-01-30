#include <QtCore>

#include <chrono>

using namespace std::chrono_literals;

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);


    QTimer t;
    QElapsedTimer et;
    t.setInterval(100ms);
    et.start();
    t.start();
    const int N = 5;
    int timeout_count = 0;
    t.connect(&t, &QTimer::timeout, [&]{
        qDebug() << "timeout. elasped=" << et.elapsed();
        timeout_count++;

        QThread::sleep(1s);

        if (timeout_count == N) {
            t.stop();
            app.quit();
        }
    });


    return app.exec();
}
