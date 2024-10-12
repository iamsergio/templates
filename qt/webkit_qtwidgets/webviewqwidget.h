/// SPDX-FileCopyrightText: 2024 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
/// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QWidget>
#include <QtGui/qevent.h>
#include <WebKit/WebKit.h>

class WebViewQWidget : public QWidget
{
public:
    explicit WebViewQWidget(QWidget *parent = nullptr);
    ~WebViewQWidget() override;

    void load(const QString &path);

protected:
    void resizeEvent(QResizeEvent *) override;

private:
    WKWebView *m_webview = nullptr;
    WKWebViewConfiguration *m_webviewConfig = nullptr;
};
