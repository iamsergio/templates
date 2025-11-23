#include "Controller.h"
#include <QQuickWindow>
#include <QGuiApplication>
#include <QtGui/qguiapplication.h>

Controller::Controller(QObject *parent) : QObject(parent) {
    const auto windows = qGuiApp->allWindows();
    for (auto *window : windows) {
        if (auto *quickWindow = qobject_cast<QQuickWindow *>(window)) {
            connect(quickWindow, &QQuickWindow::frameSwapped, this, &Controller::onFrameSwapped);
            break;
        }
    }
    m_timer.start();
}

int Controller::fps() const
{
    return m_fps;
}

void Controller::onFrameSwapped()
{
    m_frameCount++;
    if (m_timer.elapsed() >= 1000) {
        m_fps = m_frameCount;
        m_frameCount = 0;
        m_timer.restart();
        emit fpsChanged();
    }
}
