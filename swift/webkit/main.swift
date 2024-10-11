import mywebkitlib

import Cocoa

@preconcurrency
import WebKit

class AppDelegate: NSObject, NSApplicationDelegate, WKNavigationDelegate {
    var window: NSWindow!
    var webView: WKWebView!
    var requestedUrl: String

    init(message: String) {
        self.requestedUrl = message;
        super.init()
    }

    func applicationDidFinishLaunching(_ notification: Notification) {
        let windowSize = NSRect(x: 0, y: 0, width: 800, height: 600)
        window = NSWindow(contentRect: windowSize, styleMask: [.titled, .closable, .resizable, .miniaturizable], backing: .buffered, defer: false)
        window.center()
        window.title = "Minimal WebKit Example"
        window.makeKeyAndOrderFront(nil)

        webView = createWebView(window.contentView!)
        webView.navigationDelegate = self
        window.contentView?.addSubview(webView)

        let fileManager = FileManager.default
        let localHTMLPath = requestedUrl

        if fileManager.fileExists(atPath: localHTMLPath) {
            let fileURL = URL(fileURLWithPath: localHTMLPath)
            let directoryURL = fileURL.deletingLastPathComponent()
            webView.loadFileURL(fileURL, allowingReadAccessTo: directoryURL)
        } else {
            let url = URL(string: requestedUrl)!
            let request = URLRequest(url: url)
            webView.load(request)
        }

        NSApplication.shared.activate()
    }

    func webView(_ webView: WKWebView, decidePolicyFor navigationAction: WKNavigationAction, decisionHandler: @escaping (WKNavigationActionPolicy) -> Void) {
        let url = navigationAction.request.url

        if let url = url, url.pathExtension.lowercased() == "pdf" {
            NSWorkspace.shared.open(url)
            print("Opening PDF \(url.absoluteString)")
            decisionHandler(.cancel)
        } else {
            decisionHandler(.allow)
        }
    }

    func applicationWillTerminate(_ notification: Notification) {
        print("applicationWillTerminate:")
    }
}

if CommandLine.argc <= 1 {
    print("Pass the url to open!")
    exit(-1)
}

let app = NSApplication.shared
let delegate = AppDelegate(message: CommandLine.arguments[1])
app.delegate = delegate
app.setActivationPolicy(.regular)

app.run()
