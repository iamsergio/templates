/// SPDX-FileCopyrightText: 2024 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
/// SPDX-License-Identifier: GPL-3.0-or-later

import Cocoa
import WebKit

@_cdecl("createWebView")
public func createWebView(_ parentView: NSView) -> WKWebView {
    let webView = WKWebView(frame: parentView.bounds)

    return webView
}
