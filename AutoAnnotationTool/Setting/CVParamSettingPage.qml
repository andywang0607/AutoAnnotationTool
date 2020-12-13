import QtQuick 2.0
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.0

Page {
    property int defaultIteration: 4
    property double defaultEpsilon: 1
    ColumnLayout{
        anchors.centerIn: parent
        Layout.preferredWidth: 600
        Layout.preferredHeight: 400
        RowLayout{
            Label{
                id: iterationLabel
                Layout.preferredWidth: 100
                text: qsTr("Iteration")
                font.pixelSize: 20
            }
            Label{
                id: iterationShow
                horizontalAlignment: Text.AlignHCenter
                Layout.preferredWidth: 60
                text: CvParam.iteration
                font.pixelSize: 20
            }
            Slider {
                id: iterationSlider
                Layout.fillWidth: true
                value: defaultIteration
                from: 1
                to: 10
                stepSize :1
                onValueChanged: {
                    CvParam.iteration = value
                }
            }
        }
        RowLayout{
            Label{
                id: epsilonLabel
                Layout.preferredWidth: 100
                text: qsTr("Epsilon")
                font.pixelSize: 20
            }
            Label{
                id: epsilonShow
                horizontalAlignment: Text.AlignHCenter
                Layout.preferredWidth: 60
                text: CvParam.epsilon
                font.pixelSize: 20
            }
            Slider {
                id: epsilonLabelSlider
                Layout.fillWidth: true
                value: defaultEpsilon
                from: 1
                to: 10
                stepSize :0.01
                onValueChanged: {
                    CvParam.epsilon = value.toFixed(2)
                }
            }
        }
    }
}
