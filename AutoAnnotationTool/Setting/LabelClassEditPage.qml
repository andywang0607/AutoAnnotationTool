import QtQuick 2.0
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.0
import QtQuick.Controls.Material 2.12
Page {

    ListView{
        anchors.centerIn: parent
        width: 400
        height: parent.height * 0.8
        model: labelPropertyModel
        delegate: Pane{
            Material.elevation: 6
            Layout.alignment: Qt.AlignHCenter
            width: parent.width
            height: 34
            Label{
                text: labelName
                width: parent.width
                height: parent.height
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
        }
    }
    ListModel{
        id: labelPropertyModel
        ListElement{
            labelName: "Cat"
        }
    }
}
