import QtQuick 2.15
import QtQuick.Window 2.15

import QtQml.StateMachine

Window {
    width: 1000
    height: 800
    visible: true
    title: qsTr("Pola")

    Rectangle {
        id: background
        anchors.fill: parent
        color: "white"

        Text {
            id: mainDisplay
            text: "Say 'Hey Pola'"
            font.pointSize: 24
            anchors.centerIn: parent
        }

        Text {
            id: infoDisplay
            text: ""
            font.pointSize: 18
            //anchors.horizontalCenter: parent.horizontalCenter
            //anchors.verticalCenterOffset: parent.height / -4
            x: 500
            y: 600
        }

        // Animation to change color
        ColorAnimation on color {
            id: colorAnimation
            duration: 250
        }

        Connections {
            target: controller

            function onReplaceText(newText) {
                mainDisplay.text = newText
            }

            function onKeywordDetected() {
                colorAnimation.to = "#90ee90"
                colorAnimation.start();
                mainDisplay.text = "What can I do for you?"
                infoDisplay.text = "Info: Wakeword detected"
            }

            function onRecordingDone() {
                mainDisplay.text = "Alright, let me think about it."
                infoDisplay.text = "Info: Transcribing speech"
            }

            function onTranscriptionDone() {
                colorAnimation.to = "#87cefa"
                colorAnimation.start();
                infoDisplay.text = "Info: Prompt understood"
            }

            function onLlamaResponse() {
                colorAnimation.to = "#f4a460"
                colorAnimation.start();
                infoDisplay.text = "Info: Response generated"
            }

        }

    }
}
