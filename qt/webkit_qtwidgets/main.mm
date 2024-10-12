/// SPDX-FileCopyrightText: 2024 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
/// SPDX-License-Identifier: GPL-3.0-or-later

#include "webviewqwidget.h"

#include <AppKit/AppKit.h>
#include <QApplication>
#include <QPushButton>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>
#include <QWindow>

#include <QPushButton>
#include <QThread>
#include <WebKit/WebKit.h>
#include <Webkit/WebKit.h>

extern "C" WKWebView* createWebView(NSView*);

void testMemLeaks()
{
    // Check if there's a bunch of webview processes hanging after widget
    // went out of scope
    for (int i = 0; i < 10; ++i) {
        WebViewQWidget webview;
        webview.load("https://www.apple.com");
    }
}

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    QWidget window;
    window.resize(1000, 1000);
    window.show();

    WebViewQWidget container;
    container.load("https://www.apple.com");

    auto lay = new QVBoxLayout(&window);
    auto but = new QPushButton("toggle visibility");
    lay->addWidget(&container);
    lay->addWidget(but);

    but->connect(but, &QPushButton::clicked, &app,
        [&container] { container.setVisible(!container.isVisible()); });

    if (app.arguments().contains("--quit")) {
        // to test under ASAN
        QTimer::singleShot(3000, [] {
            qApp->exit();
        });
    } else if (app.arguments().contains("--test-leaks")) {
        testMemLeaks();
    }

    return app.exec();
}
