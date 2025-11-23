#pragma once

#include <QObject>
#include <QQmlEngine>
#include <QElapsedTimer>

class Controller : public QObject {
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON
    Q_PROPERTY(int fps READ fps NOTIFY fpsChanged)
public:
    explicit Controller(QObject *parent = nullptr);
    int fps() const;

signals:
    void fpsChanged();

private:
    void onFrameSwapped();

    int m_fps = 0;
    int m_frameCount = 0;
    QElapsedTimer m_timer;
};
