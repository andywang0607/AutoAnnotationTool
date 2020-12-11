import QtQuick 2.14
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.2
import QtQuick.Window 2.14
import QtQuick.Controls.Material 2.12

import LabelCollector 1.0
import LabelDataModel 1.0
import AnnotationManager 1.0

import "Setting"

ApplicationWindow {
    visible: true
    width: 870
    height: 560
    title: qsTr("Auto Annotation Tool")
    Material.theme: Material.Dark
    Material.primary: Material.BlueGrey

    header: ToolBar{
        id: toolBar
        RowLayout{
            anchors.fill: parent
            ToolButton{
                icon.source: drawer.opened ? "qrc:/icon/round_menu_open_black_18dp.png" :
                                             "qrc:/icon/round_menu_black_18dp.png"
                onClicked: {
                    (drawer.position==1) ? drawer.close() : drawer.open()
                }
            }
            ToolButton{
                icon.source: "qrc:/icon/round_folder_open_black_18dp.png"
                onClicked: {
                    folderDialog.open()
                }
            }
            ToolButton{
                icon.source: "qrc:/icon/round_photo_black_18dp.png"
                onClicked: {
                    fileDialog.open()
                }
            }
            ToolButton{
                visible: !(labelCollector.imgSrc == "")
                icon.source: "qrc:/icon/round_save_black_18dp.png"
                onClicked: {
                    dataSaver.saveAnnotation(0)
                }
            }
            Item { Layout.fillWidth: true }
            ToolButton{
                icon.source: "qrc:/icon/round_list_black_18dp.png"
                onClicked: {
                    (listViewDrawer.position==1) ? listViewDrawer.close() : listViewDrawer.open()
                }
            }
        }
    }

    footer: RowLayout{
        Button {
            icon.source: "qrc:/icon/round_chevron_left_black_18dp.png"
            onClicked: {
                labelCollector.fileIdx--
            }
        }
        Item{
            Layout.fillWidth: true
        }
        Button {
            icon.source: "qrc:/icon/round_chevron_right_black_18dp.png"
            onClicked: {
                labelCollector.fileIdx++
            }
        }
    }

    Drawer{
        id: drawer
        width: 300
        height: parent.height - toolBar.height
        y: header.height
        dragMargin: 0
        ColumnLayout{
            id: buttonCollumn
            anchors.fill: parent
            spacing: 0
            Button{
                id:settingButton
                Layout.fillHeight: true
                Layout.fillWidth: true
                text: qsTr("Setting")
                onClicked: {
                    settingWindow.show()
                }
            }
        }
    }

    LabelCollector{
        id:labelCollector
        x: 0
        y: 0
        width: parent.width
        height: parent.height
        cvParam: CvParam
        onWidthChanged: {
            labelCollector.setImage(labelCollector.image)
        }
        onHeightChanged: {
            labelCollector.setImage(labelCollector.image)
        }
        onImageChanged: {
            dataSaver.loadAnnotation(0)
        }
    }

    Drawer{
        id: listViewDrawer
        width: 300
        height: parent.height - toolBar.height
        y: header.height
        edge: Qt.RightEdge
        dragMargin : 30
        ColumnLayout{
            anchors.fill: parent
            Label{
                id: labelListTitle
                Layout.fillWidth:  true
                Layout.preferredHeight: 30
                text: "Label List"
                font.pixelSize: 18
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            ListView{
                id:listView
                Layout.fillWidth:  true
                Layout.fillHeight: true
                model: labelDataModel
                clip: true
                delegate: Rectangle {
                    height: 50
                    width: parent.width - 6
                    anchors.horizontalCenter: parent.horizontalCenter
                    RowLayout{
                        anchors.fill: parent
                        anchors.margins: 10
                        Label {
                            id:label
                            Layout.preferredWidth: 40
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            text: labelClass
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            background: Rectangle {
                                border.color: "black"
                                border.width: 1
                            }
                        }
                        TextField {
                            id: classInput
                            Layout.preferredWidth: 85
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            placeholderText: qsTr("Edit Class")
                            horizontalAlignment : TextInput.AlignHCenter
                            onEditingFinished:{
                                labelClass = text
                                label.text = text
                                classInput.cursorVisible = false
                                classInput.focus = false
                            }
                            onFocusChanged: {
                                if(!focus)
                                    text = ""
                            }
                        }
                        Button{
                            Layout.preferredWidth: 40
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            text: "Remove"
                            onClicked: {
                                labelCollector.removeLabel(index)
                            }
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
            drawer.close()
        }
    }
    FileDialog {
        id: folderDialog
        title: "Choose a folder to label"
        selectExisting: true
        selectFolder: true
        selectMultiple: false
        onAccepted: {
            labelCollector.imgSrc = folderDialog.fileUrl
            drawer.close()
        }
    }
    LabelDataModel{
        id:labelDataModel
        item: labelCollector
    }
    AnnotationManager{
        id: dataSaver
        labelCollector: labelCollector
    }

    SettingWindow{
        id: settingWindow
    }
}
