import QtQuick 2.14
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.2
import QtQuick.Window 2.14

import LabelCollector 1.0
import LabelDataModel 1.0
import DataSaver 1.0

Window {
    visible: true
    width: 870
    height: 360
    title: qsTr("Auto Annotation Tool")
    RowLayout{
        anchors.fill: parent
        ColumnLayout{
            id: buttonCollumn
            Layout.alignment: Qt.AlignLeft
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.preferredWidth: 120
            Layout.maximumWidth: 120
            Button{
                id:openFolderButton
                Layout.fillHeight: true
                Layout.fillWidth: true
                text: qsTr("Open Folder")
                onClicked: {
                    //ToDo
                }
            }
            Button{
                id:openFileButton
                Layout.fillHeight: true
                Layout.fillWidth: true
                text: qsTr("Open File")
                onClicked: {
                    fileDialog.open()
                }
            }
            Button{
                id:saveFileButton
                Layout.fillHeight: true
                Layout.fillWidth: true
                text: qsTr("Save File")
                onClicked: {
                    dataSaver.SaveData(0)
                }
            }
            Button{
                id:nextButton
                Layout.fillHeight: true
                Layout.fillWidth: true
                text: qsTr("Next Image")
                onClicked: {
                    // ToDo
                }
            }
            Button{
                id:lastButton
                Layout.fillHeight: true
                Layout.fillWidth: true
                text: qsTr("Last Image")
                onClicked: {
                    // ToDo
                }
            }
        }
        LabelCollector{
            id:labelCollector
            Layout.alignment: Qt.AlignLeft
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.preferredWidth: 500
            onWidthChanged: {
                labelCollector.setImage(labelCollector.image)
            }
            onHeightChanged: {
                labelCollector.setImage(labelCollector.image)
            }
        }
        Rectangle{
            id: listRect
            Layout.alignment: Qt.AlignRight
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.preferredWidth: 270
            Layout.maximumWidth: 300
            border.color: "dimgray"
            border.width: 3
            Label{
                id: labelListTitle
                anchors.top: parent.top
                anchors.topMargin: 3
                width: parent.width - 6
                height: 30
                text: "Label List"
                font.pixelSize: 18
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            ListView{
                id:listView
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: labelListTitle.bottom
                anchors.topMargin: 3
                width: parent.width
                height: parent.height-30-9
                model: labelDataModel
                clip: true
                delegate: Rectangle {
                    height: 50
                    width: parent.width - 6
                    anchors.horizontalCenter: parent.horizontalCenter
                    RowLayout{
                        anchors.fill: parent
                        anchors.margins: 10
                        TextField {
                            id: classInput
                            Layout.preferredWidth: 100
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            placeholderText: qsTr("Class")
                            horizontalAlignment : TextInput.AlignHCenter
                            onEditingFinished:{
                                labelClass = text
                                classInput.cursorVisible = false
                                classInput.focus = false
                            }
                        }

                        Button{
                            Layout.preferredWidth: 50
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            text: "Remove"
                            onClicked: {
                                labelCollector.RemoveLabel(index)
                            }
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
            nameFilters: ["*.jpg", "*.png", "*.bmp"]
            onAccepted: {
                labelCollector.imgSrc = fileDialog.fileUrl
            }
        }
        LabelDataModel{
            id:labelDataModel
            item: labelCollector
        }
        DataSaver{
            id: dataSaver
            labelCollector: labelCollector
        }
    }
}
