#pragma once

#include <QObject>
#include <QQmlEngine>

class MyObj : public QObject {
    Q_OBJECT
    QML_ELEMENT
};