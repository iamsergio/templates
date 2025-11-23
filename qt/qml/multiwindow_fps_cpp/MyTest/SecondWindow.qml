import QtQuick
import QtQuick.Controls

Window {
    width: 400
    height: 300
    title: "Second Window"
    visible: true

    ScrollView {
        anchors.fill: parent
        contentWidth: availableWidth

        Column {
            width: parent.width
            spacing: 20
            padding: 20

            Text {
                text: "Complex Window Example"
                font.pixelSize: 24
                font.bold: true
            }

            Text {
                text: "This is a more complex window with various controls and scrolling capability. Resize the window to see the scrollbars in action."
                wrapMode: Text.WordWrap
                width: parent.width - 40
            }

            CheckBox {
                text: "Enable Feature A"
                checked: true
            }

            CheckBox {
                text: "Enable Feature B"
            }

            Row {
                spacing: 10
                Label {
                    text: "Volume:"
                    anchors.verticalCenter: parent.verticalCenter
                }
                Slider {
                    from: 0
                    to: 100
                    value: 50
                    width: 200
                }
            }

            TextField {
                placeholderText: "Enter some text..."
                width: parent.width - 40
            }

            Switch {
                text: "Dark Mode"
            }

            GroupBox {
                title: "Radio Options"
                Column {
                    RadioButton { text: "Option 1"; checked: true }
                    RadioButton { text: "Option 2" }
                    RadioButton { text: "Option 3" }
                }
            }

            Button {
                text: "Perform Action"
                onClicked: console.log("Action performed")
                width: parent.width - 40
            }

            Repeater {
                model: 10
                Text {
                    text: "Filler text item " + index
                    color: "gray"
                }
            }
        }
    }
}
