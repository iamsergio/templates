import QtQuick 2.9

import MyTest

Rectangle {
    id: root
    MyRect {
        color: "red"
    }

    color: "red"
    
    MouseArea {
        anchors.fill: parent
        onPressed: {
            Qt.createQmlObject('import QtQuick 2.9; Rectangle { width: 30; height: 30; color: "blue"; x: 0; y: 0; z: 1 }', parent)      
        }
    }
}
