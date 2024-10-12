/// SPDX-FileCopyrightText: 2024 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
/// SPDX-License-Identifier: GPL-3.0-or-later

#include "webviewqwidget.h"

#include <QDebug>
#include <QEvent>
#include <QWidget>

#if __has_feature(objc_arc)
// print here
#endif

WebViewQWidget::WebViewQWidget(QWidget* parent)
    : QWidget(parent)
{
    auto containerView = reinterpret_cast<NSView*>(winId());

    m_webviewConfig = [[WKWebViewConfiguration alloc] init];

    m_webview = [[WKWebView alloc] initWithFrame:containerView.bounds
                                   configuration:m_webviewConfig];

    [m_webview retain];

    [containerView addSubview:m_webview];
}

WebViewQWidget::~WebViewQWidget()
{
    [m_webview removeFromSuperview];
    [m_webview dealloc];
    [m_webviewConfig dealloc];
}

void WebViewQWidget::load(const QString& path)
{
    NSURL* url = [NSURL URLWithString:path.toNSString()];
    NSURLRequest* request = [NSURLRequest requestWithURL:url];
    [m_webview loadRequest:request];
}

void WebViewQWidget::resizeEvent(QResizeEvent* ev)
{
    QWidget::resizeEvent(ev);

    NSRect newFrame = m_webview.frame;
    newFrame.size.width = static_cast<CGFloat>(width());
    newFrame.size.height = static_cast<CGFloat>(height());
    m_webview.frame = newFrame;
}
