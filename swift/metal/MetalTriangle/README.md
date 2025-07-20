# Metal Triangle Example

This is a minimal Metal example that demonstrates how to draw a triangle using Apple's Metal graphics API.

## Project Structure

- `main.swift`: The entry point of the application that sets up the window and Metal view
- `Renderer.swift`: Contains the Metal rendering code that draws the triangle
- `build.sh`: A build script to compile and run the application

## Requirements

- macOS with Metal support
- Xcode command line tools installed

## How to Build and Run

1. Open Terminal and navigate to the project directory
2. Make the build script executable:
   ```
   chmod +x build.sh
   ```
3. Build and run the application:
   ```
   ./build.sh run
   ```

## How It Works

1. The application creates a Metal device and a Metal view
2. The renderer sets up a render pipeline with vertex and fragment shaders
3. A triangle is defined with three vertices, each having a position and color
4. In the draw method, a render pass is created to draw the triangle
5. The triangle is rendered to the screen using the Metal pipeline

## Customization

You can modify the following to experiment with the example:
- Change the triangle's vertices in the `createVertexBuffer` method
- Modify the shaders in the `createPipelineState` method
- Change the background color in the `draw` method