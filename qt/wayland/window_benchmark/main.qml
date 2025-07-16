import QtQuick
import QtQuick.Window
import QtWayland.Compositor
import QtWayland.Compositor.XdgShell
import QtWayland.Compositor.WlShell
import QtWayland.Compositor.QtShell
import QtWayland.Compositor.IviApplication

WaylandCompositor {
    WaylandOutput {
        sizeFollowsWindow: true
        window: Window {
            width: 1024
            height: 768
            visible: true
            visibility: _fullscreen ? Window.FullScreen : Window.Windowed
            Repeater {
                model: _noSurface ? 0 : shellSurfaces
                ShellSurfaceItem {
                    shellSurface: modelData
                    onSurfaceDestroyed: shellSurfaces.remove(index)
                }
            }
        }
    }

    XdgShell {
        onToplevelCreated: (toplevel, xdgSurface) => shellSurfaces.append({shellSurface: xdgSurface});
    }

    QtShell {
        onQtShellSurfaceCreated: function(qtShellSurface) {
            shellSurfaces.append({shellSurface: qtShellSurface});
        }
    }
    
    WlShell {
        onWlShellSurfaceCreated: function(surface) {
            shellSurfaces.append({shellSurface: surface});
        }
    }
    
    ListModel { id: shellSurfaces }
}
