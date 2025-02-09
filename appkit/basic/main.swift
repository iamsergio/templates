import AppKit

class AppDelegate: NSObject, NSApplicationDelegate {
    let window: NSWindow

    override init() {
        print("NSDelegate CTOR START")
        window = NSWindow(
            contentRect: NSRect(x: 0, y: 0, width: 200, height: 100),
            styleMask: [.titled, .closable, .resizable],
            backing: .buffered,
            defer: false)

        let button = NSButton(frame: NSRect(x: 0, y: 0, width: 100, height: 30))
        button.title = "Click Me"
        button.bezelStyle = .rounded

        window.contentView?.addSubview(button)

        button.translatesAutoresizingMaskIntoConstraints = false
        NSLayoutConstraint.activate([
            button.centerXAnchor.constraint(equalTo: window.contentView!.centerXAnchor),
            button.centerYAnchor.constraint(equalTo: window.contentView!.centerYAnchor),
        ])

        super.init()

        button.target = self
        button.action = #selector(delegate.showHello)

        window.center()
        window.makeKeyAndOrderFront(nil)
        print("NSDelegate CTOR END")
    }

    func applicationDidFinishLaunching(_ aNotification: Notification) {
        print("applicationDidFinishLaunching")
    }

    @objc func showHello() {
        print("Alert!")
        let alert = NSAlert()
        alert.messageText = "Hello World"
        alert.alertStyle = .informational
        alert.addButton(withTitle: "OK")
        alert.runModal()
    }
}

let app = NSApplication.shared
let delegate = AppDelegate()
app.delegate = delegate

app.setActivationPolicy(.regular)
app.activate(ignoringOtherApps: true)
app.run()
