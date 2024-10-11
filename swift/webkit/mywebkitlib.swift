import Cocoa
import WebKit

@_cdecl("createWebView")
public func createWebView(_ parentView: NSView) -> WKWebView {
    let webView = WKWebView(frame: parentView.bounds)

    return webView
}
