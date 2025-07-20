import Cocoa
import MetalKit

class Renderer: NSObject, MTKViewDelegate {
    let device: MTLDevice
    let commandQueue: MTLCommandQueue
    var pipelineState: MTLRenderPipelineState!
    var vertexBuffer: MTLBuffer!

    init(device: MTLDevice) {
        self.device = device
        self.commandQueue = device.makeCommandQueue()!
        super.init()

        createPipelineState()
        createVertexBuffer()
    }

    func createPipelineState() {
        let shaderSource = """
            #include <metal_stdlib>
            using namespace metal;

            vertex float4 vertexShader(uint vertexID [[vertex_id]],
                                     constant float3 *positions [[buffer(0)]]) {
                return float4(positions[vertexID].xy, 0, 1);
            }

            fragment float4 fragmentShader() {
                return float4(1, 0, 0, 1); // Red color
            }
            """

        let library: MTLLibrary
        do {
            library = try device.makeLibrary(source: shaderSource, options: nil)
        } catch {
            fatalError("Failed to create Metal library: \(error)")
        }

        let vertexFunction = library.makeFunction(name: "vertexShader")
        let fragmentFunction = library.makeFunction(name: "fragmentShader")

        let pipelineDescriptor = MTLRenderPipelineDescriptor()
        pipelineDescriptor.vertexFunction = vertexFunction
        pipelineDescriptor.fragmentFunction = fragmentFunction
        pipelineDescriptor.colorAttachments[0].pixelFormat = .bgra8Unorm

        do {
            pipelineState = try device.makeRenderPipelineState(descriptor: pipelineDescriptor)
        } catch {
            fatalError("Failed to create pipeline state: \(error)")
        }
    }

    func createVertexBuffer() {
        // Define a simple triangle - just x,y coords and a placeholder for z
        let vertices: [Float] = [
            0.0, 0.5, 0.0,
            -0.5, -0.5, 0.0,
            0.5, -0.5, 0.0,
        ]

        vertexBuffer = device.makeBuffer(
            bytes: vertices,
            length: vertices.count * MemoryLayout<Float>.size,
            options: [])
    }

    func mtkView(_ view: MTKView, drawableSizeWillChange size: CGSize) {
        // Handle window resizing if needed
    }

    func draw(in view: MTKView) {
        guard let drawable = view.currentDrawable,
            let renderPassDescriptor = view.currentRenderPassDescriptor
        else {
            return
        }

        guard let commandBuffer = commandQueue.makeCommandBuffer(),
            let renderEncoder = commandBuffer.makeRenderCommandEncoder(
                descriptor: renderPassDescriptor)
        else {
            return
        }

        renderEncoder.setRenderPipelineState(pipelineState)
        renderEncoder.setVertexBuffer(vertexBuffer, offset: 0, index: 0)
        renderEncoder.drawPrimitives(type: .triangle, vertexStart: 0, vertexCount: 3)
        renderEncoder.endEncoding()

        commandBuffer.present(drawable)
        commandBuffer.commit()
    }
}

class AppDelegate: NSObject, NSApplicationDelegate {
    var window: NSWindow!
    var metalView: MTKView!
    var renderer: Renderer!

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
        window.title = "Simple Metal Triangle"
        window.center()

        // Create the Metal view
        metalView = MTKView(frame: window.contentRect(forFrameRect: window.frame), device: device)
        metalView.clearColor = MTLClearColor(red: 0.2, green: 0.2, blue: 0.3, alpha: 1.0)
        metalView.colorPixelFormat = .bgra8Unorm
        window.contentView = metalView

        // Create and assign the renderer
        renderer = Renderer(device: device)
        metalView.delegate = renderer

        // Show the window
        window.makeKeyAndOrderFront(nil)
    }

    func applicationShouldTerminateAfterLastWindowClosed(_ sender: NSApplication) -> Bool {
        return true
    }
}

// Create the application and delegate
let app = NSApplication.shared
let delegate = AppDelegate()
app.delegate = delegate

// Run the application
app.run()
