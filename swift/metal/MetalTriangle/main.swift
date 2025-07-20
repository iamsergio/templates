import Cocoa
import MetalKit

// Global variables to retain strong references
var window: NSWindow!
var metalView: MTKView!
var renderer: Renderer!

class AppDelegate: NSObject, NSApplicationDelegate {
    func applicationDidFinishLaunching(_ aNotification: Notification) {
        // Create the Metal device
        guard let device = MTLCreateSystemDefaultDevice() else {
            fatalError("Metal is not supported on this device")
        }

        // Create the window
        window = NSWindow(
            contentRect: NSRect(x: 0, y: 0, width: 800, height: 600),
            styleMask: [.titled, .closable, .miniaturizable, .resizable],
            backing: .buffered,
            defer: false)
        window.title = "Metal Triangle"
        window.center()

        // Create the Metal view
        metalView = MTKView(frame: window.contentRect(forFrameRect: window.frame), device: device)
        metalView.clearColor = MTLClearColor(red: 0.2, green: 0.2, blue: 0.3, alpha: 1.0)
        metalView.colorPixelFormat = .bgra8Unorm
        window.contentView = metalView

        // Create and assign the renderer
        renderer = Renderer(device: device)
        metalView.delegate = renderer

        // Show the window and ensure it's in front
        NSApp.activate(ignoringOtherApps: true)
        window.makeKeyAndOrderFront(nil)
        window.orderFrontRegardless()

        // If needed, try raising the window level
        window.level = .floating
    }

    func applicationShouldTerminateAfterLastWindowClosed(_ sender: NSApplication) -> Bool {
        return true
    }
}

let app = NSApplication.shared
let delegate = AppDelegate()
app.delegate = delegate

app.run()
