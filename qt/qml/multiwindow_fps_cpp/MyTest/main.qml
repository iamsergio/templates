import QtQuick
import QtQuick.Controls
import MyTest

Item {
    Rectangle {
        width: 100
        height: 100
        color: "blue"
        anchors.centerIn: parent

        NumberAnimation on rotation {
            from: 0
            to: 360
            duration: 2000
            loops: Animation.Infinite
        }
    }

    Column {
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.margins: 10
        spacing: 10

        Text {
            text: "FPS: " + Controller.fps
            color: "black"
            font.pixelSize: 20
        }

        Button {
            text: "Open Second Window"
            onClicked: {
                console.log("STARTING TO OPEN SECOND WINDOW");
                var component = Qt.createComponent("SecondWindow.qml");
                if (component.status === Component.Ready) {
                    var win = component.createObject(parent);
                    win.show();
                } else {
                    console.error("Error loading component:", component.errorString());
                }
            }
        }
    }
}
