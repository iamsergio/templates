#include <QtQuick>


int NUM_WINDOWS = 0;


QQuickWindow* createWindow(int n) {
    auto view = new QQuickView;

    QQuickGraphicsConfiguration config = view->graphicsConfiguration();
    config.setPipelineCacheSaveFile(QString("/tmp/cache-%1").arg(n));
    config.setPipelineCacheLoadFile(QString("/tmp/cache-%1").arg(n));
    view->setGraphicsConfiguration(config);
    
    view->setSource(QUrl(QStringLiteral("qrc:/main_client.qml")));
    view->setResizeMode(QQuickView::SizeViewToRootObject);
    return view;
}

void createWindows(int n) {
    if (n == 0) {
        
        for (auto window : qGuiApp->topLevelWindows()) {
            qDebug() << "deleting" << window;
            delete window;
        }
        
        qGuiApp->exit();
        return;   
    }
    
    auto timer = new QElapsedTimer();
    timer->start();
    auto view = createWindow(n);
    qDebug() << "View" << n << "window created" << timer->elapsed();

    auto ctx = new QObject();
    QObject::connect(view, &QQuickWindow::frameSwapped, ctx, [view, timer, n, ctx] {
        qDebug() << "View" << n << "; frameSwapped (ms):" << timer->elapsed();
        delete ctx;
        QTimer::singleShot(1000, [n] {
            createWindows(n-1);
        });
    });
    
    QObject::connect(view, &QQuickWindow::beforeFrameBegin, ctx, [view, timer, n, ctx] {
        qDebug() << "View" << n << "; beforeFrameBegin (ms):" << timer->elapsed();
    });
    
    QObject::connect(view, &QQuickWindow::afterSynchronizing, ctx, [view, timer, n, ctx] {
        qDebug() << "View" << n << "; afterSynchronizing (ms):" << timer->elapsed();
    });
    
    view->show();
    qDebug() << "View" << n << "show()" << timer->elapsed();
}


int main(int argc, char**argv) {
    
    QGuiApplication app(argc, argv);

    QCommandLineParser parser;
    parser.addHelpOption();
    QCommandLineOption numWindowsOption(QStringList() << "n" << "numWindows",
                                        "Number of windows to create.",
                                        "N", "1");
    
    QCommandLineOption sleepSecsOption(QStringList() << "s" << "sleepSecs",
                                        "Number of seconds to sleep between creating windows.",
                                        "S", "-1");
    parser.addOption(sleepSecsOption);
    
    parser.addOption(numWindowsOption);
    
    parser.process(app);
    
    auto warmUpView = createWindow(-1);
    warmUpView->show();
    auto ctx = new QObject();
    QObject::connect(warmUpView, &QQuickWindow::frameSwapped, ctx, [ctx, warmUpView, &parser, &numWindowsOption, &sleepSecsOption] {
        delete ctx;
        warmUpView->deleteLater();
        const int numWindows = parser.value(numWindowsOption).toInt();
        NUM_WINDOWS = numWindows;
        
        const int sleepSecs = parser.value(sleepSecsOption).toInt();
       
         // delay 1 second so it's easier to filter in hotspot
         QTimer::singleShot(1000, [numWindows, sleepSecs] { createWindows(numWindows); });
    });

    
    return app.exec();
}
