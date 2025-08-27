import QtQuick 2.9

import MyTest

Rectangle {
    id: root
    MyRect {
        color: _color
    }

    Text {
        anchors.centerIn: parent
        text: root.width + " x " + root.height
    }

    color: _color
}
