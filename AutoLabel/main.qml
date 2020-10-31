import QtQuick 2.14
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.2
import QtQuick.Window 2.14

import LabelCollector 1.0
import LabelDataModel 1.0

Window {
    visible: true
    width: 890
    height: 480
    title: qsTr("Hello World")
    Button{
        id:openFolderButton
        anchors.top: parent.top
        anchors.left: parent.left
        width: 80
        height: width
        text: qsTr("Open Folder")
    }
    Button{
        id:openFileButton
        anchors.top: openFolderButton.bottom
        anchors.left:parent.left
        width:openFolderButton.width
        height: openFolderButton.height
        text: qsTr("Open File")
        onClicked: {
            fileDialog.open()
        }
    }
    LabelCollector{
        id:labelCollector
        anchors.left: openFolderButton.right
        anchors.top: parent.top
        width: parent.width - openFolderButton.width - listRect.width
        height: parent.height
    }
    Rectangle{
        id: listRect
        anchors.top: parent.top
        anchors.right: parent.right
        height: parent.height
        width: 250
        border.color: "black"
        border.width: 3
        ListView{
            id:listView
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: 3
            width: parent.width
            height: parent.height-6
            model: labelDataModel
            clip: true
            delegate: Rectangle {
                id: delegateRec
                z:1
                height: 90
                width: parent.width - 6
                anchors.horizontalCenter: parent.horizontalCenter
                border.color: "black"
                RowLayout{
                    anchors.fill: parent
                    anchors.margins: 10
                    TextField {
                        id: classInput
                        Layout.preferredWidth: 70
                        placeholderText: qsTr("Label class")
                        horizontalAlignment : TextInput.AlignHCenter
                        validator: IntValidator {bottom: 0; top: 100}
                        onEditingFinished:{
                            labelClass = text
                            classInput.cursorVisible = false
                            classInput.focus = false
                        }
                    }

                    Button{
                        Layout.preferredWidth: 60
                        text: "Remove"
                        Layout.fillWidth: true
                        onClicked: {
                            labelCollector.RemoveLabel(index)
                        }
                    }
                }
            }
        }
        FileDialog {
            id: fileDialog
            title: "Choose label image"
            selectExisting: true
            selectFolder: false
            selectMultiple: false
            nameFilters: ["*.png", "*.jpg", "*.bmp"]
            onAccepted: {
                labelCollector.imgSrc = fileDialog.fileUrl
            }
        }
        LabelDataModel{
            id:labelDataModel
            item: labelCollector
        }
    }
}
