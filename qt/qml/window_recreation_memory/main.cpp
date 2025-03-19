#include <QQuickView>
#include <QGuiApplication>
#include <QTimer>
#include <QCommandLineParser>

#include <malloc.h>
#ifdef ENABLE_JEMALLOC
#include <jemalloc/jemalloc.h>
#endif

// ./build-dev/test -q -n 0
//     6.5 heap

// ./build-dev/test -q -n 1
//     12.5 heap (6MB per window). Probably QQmlEngine overhead

// ./build-dev/test -n 100
//    306.3 heap (3MB per window)

// ./build-dev/test -n 50
// smem reports 630532

// ./build-dev/test -n 50 --malloc-trim
// smem reports 37000

#ifdef ENABLE_JEMALLOC
void dump_jemallocinfo()
{
    unsigned narenas;
    size_t sz = sizeof(narenas);

    if (mallctl("arenas.narenas", &narenas, &sz, NULL, 0) == 0) {
        printf("Total number of arenas: %u\n", narenas);
    } else {
        printf("Failed to query number of arenas\n");
    }
}
#endif

QQuickView *createWindow()
{
    auto win = new QQuickView();
    win->setSource(QUrl("qrc:/qt/qml/MyTest/main.qml"));
    win->setResizeMode(QQuickView::SizeRootObjectToView);
    win->resize(500, 500);
    win->show();
    return win;
}

int main(int argc, char **argv)
{
#ifdef ENABLE_JEMALLOC
    dump_jemallocinfo();
#endif

    QGuiApplication app(argc, argv);

    QCommandLineParser parser;
    parser.setApplicationDescription("Window test");
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption numWindowsOption(QStringList() << "n" << "num-windows",
                                        "Number of windows to create", "number", QString::number(50));
    parser.addOption(numWindowsOption);

    QCommandLineOption quitOption(QStringList() << "q" << "quit",
                                  "Quit after 5 seconds");
    parser.addOption(quitOption);

    QCommandLineOption mallocTrimOption(QStringList() << "malloc-trim",
                                        "Enable malloc trim");
    parser.addOption(mallocTrimOption);

    parser.process(app);

    int numWindows = parser.value(numWindowsOption).toInt();
    if (numWindows < 0 || numWindows > 100) {
        numWindows = 50;
        qDebug() << "Defaulting to 50 windows";
    }

    static int window_count = 0;

    const bool infiniteWithTimer = !parser.isSet(quitOption);
    QTimer timer;
    QVector<QQuickView *> windows;
    QVector<QObject *> annoyingObjects;
    windows.reserve(numWindows);

    if (infiniteWithTimer) {
        QObject::connect(&timer, &QTimer::timeout, [&] {
            for (auto window : windows) {
                window->deleteLater();
            }
            // qDeleteAll(annoyingObjects);
            // annoyingObjects.clear();
            windows.clear();

            auto leak = new QObject(); // annoy the allocator and leak this

            for (int i = 0; i < numWindows; i++) {
                window_count++;
                QQuickView *win = createWindow();
                // annoyingObjects.append(new QObject());
                windows.append(win);
            }

            if (parser.isSet(mallocTrimOption)) {
                malloc_trim(0);
            }

            qDebug() << "Window count:" << window_count;
        });

        timer.start(1000);

        // Stop after 30s
        QTimer::singleShot(30000, &app, [&] {
            timer.stop();
            qDeleteAll(windows);
            windows.clear();
        });
    } else {
        for (int i = 0; i < numWindows; i++) {
            createWindow();
        }

        if (parser.isSet(quitOption)) {
            QTimer::singleShot(5000, &app, [&] {
                QGuiApplication::quit();
            });
        }
    }


    app.exec();
}
