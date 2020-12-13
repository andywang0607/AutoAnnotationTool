import QtQuick 2.0
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.0
import QtQuick.Controls.Material 2.12

import "../materialdesign/"

Page {
    property alias labelPropertyModel: labelPropertyModel
    ListView{
        id: listView
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: parent.width * 0.05
        width: parent.width * 0.4
        height: parent.height * 0.8
        model: labelPropertyModel
        delegate: Pane{
            Material.elevation: 6
            Layout.alignment: Qt.AlignHCenter
            width: parent.width
            height: 70
            RowLayout{
                anchors.fill: parent
                Layout.alignment: Qt.AlignVCenter
                FlatButton{
                    Layout.preferredWidth: 34
                    Image {
                        anchors.fill: parent
                        source: "qrc:/icon/round_delete_black_18dp.png"
                        asynchronous : true
                    }
                    onClicked: {
                        labelPropertyModel.remove(index, 1)
                    }
                }
                Label{
                    text: labelName
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    font.pointSize: 18
                }
            }
        }
    }
    ListModel{
        id: labelPropertyModel
        ListElement{
            labelName: "dog"
        }
        ListElement{
            labelName: "flower"
        }
    }
    Pane{
        anchors.top: listView.top
        anchors.right: parent.right
        anchors.rightMargin: parent.width * 0.05
        width: parent.width * 0.4
        height: 70
        Material.elevation: 6
        TextField{
            id: textField
            width: parent.width
            height: parent.height
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            horizontalAlignment: TextInput.AlignHCenter
        }
        RoundButton{
            anchors.top: textField.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            Material.background: Material.LightGreen
            Image {
                anchors.fill: parent
                source: "qrc:/icon/round_add_circle_black_18dp.png"
                fillMode: Image.PreserveAspectFit
            }
            onClicked: {
                if(textField.text != ""){
                    labelPropertyModel.append({"labelName":textField.text})
                    textField.clear()
                }
            }
        }
    }
}
