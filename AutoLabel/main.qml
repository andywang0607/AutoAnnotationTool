import QtQuick 2.14
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.2
import QtQuick.Window 2.14

import LabelCollector 1.0

Window {
    visible: true
    width: 640
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
        width: parent.width - openFolderButton.width
        height: parent.height
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
}
