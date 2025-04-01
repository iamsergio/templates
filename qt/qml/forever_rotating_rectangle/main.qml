import QtQuick

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
