# Basic Window Test

This is a simplified version of the application that creates a basic window using Cocoa without any Metal components. This is useful for troubleshooting whether the issue is with Metal specifically or with the basic window management.

## What This Test Does

1. Creates a basic `NSWindow` with a blue background
2. Adds debug print statements to track the application's lifecycle
3. Removes all Metal-related code

## How to Build and Run

1. Open Terminal and navigate to the project directory
2. Make the build script executable:
   ```
   chmod +x build_basic.sh
   ```
3. Build and run the application:
   ```
   ./build_basic.sh run
   ```

## Expected Result

If this basic version works, you should see:
- A blue window appears on your screen
- Debug messages printed to the console

## Troubleshooting

If the window still doesn't appear:
1. Check the console output to see if the application is reaching the "Window should be visible now" message
2. Verify that the NSApp.activate() call is working properly
3. Check Activity Monitor to confirm the application is running

If this basic version works but the Metal version doesn't, the issue is likely related to:
- Metal device compatibility
- Metal view configuration
- Renderer implementation