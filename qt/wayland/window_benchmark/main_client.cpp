#include <QtQuick>


int NUM_WINDOWS = 0;


QQuickWindow* createWindow() {
    auto view = new QQuickView;
    view->setSource(QUrl(QStringLiteral("qrc:/main_client.qml")));
    view->setResizeMode(QQuickView::SizeViewToRootObject);
    return view;
}

void createWindows(int n) {
    if (n == 0) {
        qGuiApp->exit();
        return;   
    }
    
    auto timer = new QElapsedTimer();
    timer->start();
    auto view = createWindow();

    auto ctx = new QObject();
    QObject::connect(view, &QQuickWindow::frameSwapped, ctx, [view, timer, n, ctx] {
        qDebug() << "View" << n << "; frameSwapped (ms):" << timer->elapsed();
        delete ctx;
        QTimer::singleShot(1000, [n] {
            createWindows(n-1);
        });
    });
    
    view->show();
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
    
    auto warmUpView = createWindow();
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
