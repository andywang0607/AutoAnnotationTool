import QtQuick 2.0
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.0
import QtQuick.Window 2.14

Window {
    id: settingWindow
    width: 350
    height: 150
    title: qsTr("Setting")
    flags: Qt.Window
    modality: Qt.WindowModal
    visible: false
    property color tabButtonColorPressed: "#CCCCCC"
    property color tabButtonColorReleased: "#808080"

    TabBar {
        id: settingWindowTab
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width
        height: 30
        TabButton{
            text: qsTr("CV Setting")
            background: Rectangle {
                color: settingWindowTab.currentIndex == 0 ? tabButtonColorPressed : tabButtonColorReleased
            }
        }
    }
    SwipeView{
        id: settingSwipe
        interactive: false
        visible: true
        width: parent.width
        height: parent.height - settingWindowTab.height
        anchors.top: settingWindowTab.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        CVParamSettingPage{
            id: cvParamSettingPage
        }
    }
}
