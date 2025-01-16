/// SPDX-FileCopyrightText: 2024 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
/// SPDX-License-Identifier: GPL-3.0-or-later

#include <AppKit/AppKit.h>
#include <QApplication>
#include <QPushButton>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>
#include <QWindow>

#include <QPushButton>
#include <QThread>

#import <Cocoa/Cocoa.h>
#include <QtWidgets/qpushbutton.h>

void showAlert() {
    NSAlert *alert = [[NSAlert alloc] init];
    alert.messageText = @"Information";
    alert.informativeText = @"This is an informational message. <a href=\"http://www.apple.com\"> </a>";
    alert.alertStyle = NSAlertStyleInformational;

    [alert addButtonWithTitle:@"OK"];
    [alert addButtonWithTitle:@"Cancel"];

    NSModalResponse response = [alert runModal];
    if (response == NSAlertFirstButtonReturn) {
        NSLog(@"User clicked OK");
    } else if (response == NSAlertSecondButtonReturn) {
        NSLog(@"User clicked Cancel");
    }
}

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    QWidget window;
    window.resize(1000, 1000);
    window.show();

    auto lay = new QVBoxLayout(&window);
    auto but = new QPushButton("toggle visibility");
    lay->addWidget(but);
    QObject::connect(but, &QPushButton::clicked, [] {
        showAlert();
    });



    return app.exec();
}
