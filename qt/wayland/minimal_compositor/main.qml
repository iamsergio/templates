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
            Repeater {
                model: shellSurfaces
                ShellSurfaceItem {
                    x: index * 60
                    y: index * 60
                    shellSurface: modelData
                    onSurfaceDestroyed: shellSurfaces.remove(index)
                }
            }
        }
    }

    XdgShell {
        onToplevelCreated: (toplevel, xdgSurface) => shellSurfaces.append({
                shellSurface: xdgSurface
            })
    }

    QtShell {
        onQtShellSurfaceCreated: function (qtShellSurface) {
            shellSurfaces.append({
                shellSurface: qtShellSurface
            });
        }
    }

    XdgDecorationManagerV1 {
        preferredMode: XdgToplevel.ServerSideDecoration
    }

    ListModel {
        id: shellSurfaces
    }
}
