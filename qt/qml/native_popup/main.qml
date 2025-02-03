import QtQuick
import QtQuick.Controls 2.15

Rectangle {
    width: 1000
    height: 1000

    Popup {
        id: popup
        width: 200
        height: 200
        closePolicy: Popup.CloseOnEscape
        popupType: Popup.Window
    }

    Dialog {
        id: dlg
        width: 300
        height: 400
        popupType: Popup.Window
        visible: true
    }

    Button {
        text: "open"
        onClicked: {
            popup.open();
        }
    }
}
