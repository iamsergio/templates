import QtQuick

Rectangle {
    color: "blue"
    anchors.fill: parent
    width: 500
    height: 500
    Rectangle {
        width: 50
        height: 20
        color: "white"
        anchors.centerIn: parent

        // NumberAnimation on rotation {
        //     from: 0
        //     to: 360
        //     duration: 10
        //     loops: Animation.Infinite
        //     running: true
        // }
    }
}
