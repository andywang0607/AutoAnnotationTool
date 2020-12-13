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
import "materialdesign/"

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
                icon.source: "qrc:/icon/round_arrow_back_black_18dp.png"
                visible: view.currentIndex!=0
                onClicked: {
                    view.setCurrentIndex(0)
                }
            }
            ToolButton{
                visible: view.currentIndex==0
                icon.source: drawer.opened ? "qrc:/icon/round_menu_open_black_18dp.png" :
                                             "qrc:/icon/round_menu_black_18dp.png"
                onClicked: {
                    (drawer.position==1) ? drawer.close() : drawer.open()
                }
            }
            ToolButton{
                visible: view.currentIndex==0
                icon.source: "qrc:/icon/round_folder_open_black_18dp.png"
                onClicked: {
                    folderDialog.open()
                }
            }
            ToolButton{
                visible: view.currentIndex==0
                icon.source: "qrc:/icon/round_photo_black_18dp.png"
                onClicked: {
                    fileDialog.open()
                }
            }
            ToolButton{
                visible: !(labelCollector.imgSrc == "") && view.currentIndex==0
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
        visible: view.currentIndex == 0
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

    SwipeView {
        id: view
        currentIndex: 0
        anchors.fill: parent
        interactive : false
        Page {
            id: firstPage
            Label {
                id: labelPage
                anchors.fill: parent
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
            }
        }
        CVParamSettingPage {
            id: settingPage
        }
        LabelClassEditPage{
            id: labelClassEditPage
        }
    }

    Drawer{
        id: drawer
        width: 300
        height: parent.height - toolBar.height
        y: header.height
        dragMargin: 0
        ColumnLayout{
            anchors.top: parent.top
            anchors.topMargin: 16
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width
            spacing: 0
            // for setting
            FlatButton{
                Layout.preferredWidth: parent.width
                Layout.preferredHeight: 50
                RowLayout{
                    anchors.fill: parent
                    Item{ Layout.preferredWidth: 16}
                    Image {
                        Layout.preferredWidth: 34
                        Layout.preferredHeight: 34
                        source: "qrc:/icon/round_settings_black_18dp.png"
                        asynchronous : true
                    }
                    Item{
                        Layout.fillWidth: true
                    }
                    Label{
                        text: qsTr("Algorighm Setting")
                        Layout.preferredWidth: 200
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                        font.pixelSize: 18
                        font.bold: true
                    }
                }
                onClicked: {
                    view.setCurrentIndex(1)
                    drawer.close()
                }
            }
            // for edit label class
            FlatButton{
                Layout.preferredWidth: parent.width
                Layout.preferredHeight: 50
                RowLayout{
                    anchors.fill: parent
                    Item{ Layout.preferredWidth: 16}
                    Image {
                        Layout.preferredWidth: 34
                        Layout.preferredHeight: 34
                        source: "qrc:/icon/round_create_black_18dp.png"
                        asynchronous : true
                    }
                    Item{
                        Layout.fillWidth: true
                    }
                    Label{
                        text: qsTr("Edit label class")
                        Layout.preferredWidth: 200
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                        font.pixelSize: 18
                        font.bold: true
                    }
                }
                onClicked: {
                    view.setCurrentIndex(2)
                    drawer.close()
                }
            }
        }
    }

    Drawer{
        id: listViewDrawer
        width: 360
        height: parent.height - toolBar.height
        y: header.height
        edge: Qt.RightEdge
        dragMargin : 0
        ColumnLayout{
            anchors.fill: parent
            Item{
                height: 16
            }
            Label{
                id: labelListTitle
                Layout.fillWidth:  true
                Layout.preferredHeight: 30
                text: "Label List"
                font.pixelSize: 20
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            Item{
                height: 16
            }
            ListView{
                id:listView
                Layout.fillWidth:  true
                Layout.fillHeight: true
                model: labelDataModel
                clip: true
                spacing: 16
                delegate: Button {
                    flat: true
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
                            Layout.preferredHeight: 32
                            text: labelClass
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            font.pixelSize: 16
                        }
                        ComboBox{
                            id: comboBox
                            Layout.preferredHeight: 32
                            model: labelClassEditPage.labelPropertyModel
                            onActivated: {
                                label.text = labelClassEditPage.labelPropertyModel.get(currentIndex).labelName
                                labelClass = labelClassEditPage.labelPropertyModel.get(currentIndex).labelName
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
}
