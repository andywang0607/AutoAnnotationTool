import QtQuick 2.0
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.0

Page {
    property int defaultIteration: 4
    property double defaultEpsilon: 1
    ColumnLayout{
        anchors.fill: parent
        anchors.margins: 20
        Layout.fillWidth: true
        RowLayout{
            Label{
                id: iterationLabel
                Layout.preferredWidth: 100
                text: qsTr("Iteration")
            }
            Label{
                id: iterationShow
                horizontalAlignment: Text.AlignHCenter
                Layout.preferredWidth: 60
                text: CVParam.iteration
            }
            Slider {
                id: iterationSlider
                Layout.fillWidth: true
                value: defaultIteration
                from: 1
                to: 10
                stepSize :1
                onValueChanged: {
                    CVParam.iteration = value
                }
            }
        }
        RowLayout{
            Label{
                id: epsilonLabel
                Layout.preferredWidth: 100
                text: qsTr("Epsilon")
            }
            Label{
                id: epsilonShow
                horizontalAlignment: Text.AlignHCenter
                Layout.preferredWidth: 60
                text: CVParam.epsilon
            }
            Slider {
                id: epsilonLabelSlider
                Layout.fillWidth: true
                value: defaultEpsilon
                from: 1
                to: 10
                stepSize :0.01
                onValueChanged: {
                    CVParam.epsilon = value.toFixed(2)
                }
            }
        }
    }
}
