import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Qt.labs.platform 1.0

ApplicationWindow {
    visible: true
    width: 1200
    height: 600
    title: "Pola Chat"

    property string userText: ""
    property string assistantText: ""
    property bool wakeWordDetected: false

    signal keywordDetected()
    signal recordingDone()
    signal transcriptionDone(string text_t)
    signal llamaResponse(string text_r)

    Rectangle {
        id: background
        width: parent.width
        height: parent.height
        color: "#EEE8EE"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter

        Timer {
                    id: appearanceTimer
                    interval: 500
                    onTriggered: howcanihelp.visible = true
                    repeat: false
                }

        Rectangle {
            id: chat
            width: parent.width * 0.4
            height: parent.height * 0.9
            color: "white"
            radius: 20
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            anchors.bottomMargin: 100

            Rectangle {
                id: greeting
                color: "#FF77AA"
                radius: 10
                y: parent.height * 0.05
                anchors.left: parent.left
                anchors.leftMargin: 10
                anchors.top: parent.top
                anchors.topMargin: 10
                width: parent.width * 0.6
                Layout.fillHeight: true
                Layout.fillWidth: true
                implicitHeight: greetingtext.implicitHeight + 10
                implicitWidth: greetingtext.implicitWidth

                Text {
                    id: greetingtext
                    anchors.fill: parent
                    anchors.leftMargin: 10
                    anchors.rightMargin: 10
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                    text: "Hi, my name is Pola. Say 'Hey Pola!' to ask me a question ;)"
                    font.pointSize: 12
                    wrapMode: Text.WordWrap
                    clip: false
                }
            }

            Rectangle {
                id: heypola
                color: "#779988"
                radius: 10
                anchors.right: parent.right
                anchors.rightMargin: 10
                anchors.top: greeting.bottom
                anchors.topMargin: 10
                width: parent.width * 0.6
                Layout.fillHeight: true
                Layout.fillWidth: true
                implicitHeight: heypolatext.implicitHeight + 10
                implicitWidth: heypolatext.implicitWidth
                visible: false

                Text {
                    id: heypolatext
                    anchors.fill: parent
                    anchors.leftMargin: 10
                    anchors.rightMargin: 10
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                    text: "Hey Pola!"
                    font.pointSize: 12
                    wrapMode: Text.WordWrap
                    clip: false
                }
            }

            Rectangle {
                id: howcanihelp
                color: "#FF77AA"
                radius: 10
                anchors.left: parent.left
                anchors.leftMargin: 10
                anchors.top: heypola.bottom
                anchors.topMargin: 10
                width: parent.width * 0.6
                Layout.fillHeight: true
                Layout.fillWidth: true
                implicitHeight: howcanihelptext.implicitHeight + 10
                implicitWidth: howcanihelptext.implicitWidth
                visible: false

                Text {
                    id: howcanihelptext
                    anchors.fill: parent
                    anchors.leftMargin: 10
                    anchors.rightMargin: 10
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                    text: "How can i help you?"
                    font.pointSize: 12
                    wrapMode: Text.WordWrap
                    clip: false
                }
            }
            Rectangle {
                id: prompt
                color: "#779988"
                radius: 10
                anchors.right: parent.right
                anchors.rightMargin: 10
                anchors.top: howcanihelp.bottom
                anchors.topMargin: 10
                width: parent.width * 0.6
                Layout.fillHeight: true
                Layout.fillWidth: true
                implicitHeight: prompttext.implicitHeight + 10
                implicitWidth: prompttext.implicitWidth
                visible: false

                Text {
                    id: prompttext
                    anchors.fill: parent
                    anchors.leftMargin: 10
                    anchors.rightMargin: 10
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                    text: ""
                    font.pointSize: 12
                    wrapMode: Text.WordWrap
                    clip: false
                }
            }
            Rectangle {
                id: response
                color: "#FF77AA"
                radius: 10
                anchors.left: parent.left
                anchors.leftMargin: 10
                anchors.top: prompt.bottom
                anchors.topMargin: 10
                width: parent.width * 0.6
                Layout.fillHeight: true
                Layout.fillWidth: true
                implicitHeight: responsetext.implicitHeight + 10
                implicitWidth: responsetext.implicitWidth
                visible: false

                Text {
                    id: responsetext
                    anchors.fill: parent
                    anchors.leftMargin: 10
                    anchors.rightMargin: 10
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                    text: ""
                    font.pointSize: 12
                    wrapMode: Text.WordWrap
                    clip: false
                }
            }
            /*
            Rectangle {
                id: error
                width: parent.width * 0.5
                height: 40
                color: "red"
                radius: 5
                anchors.centerIn: parent
                visible: false
                Text {
                    id: errorLabel
                    text: "ERROR"
                    anchors.fill: parent
                    anchors.leftMargin: 10
                    anchors.rightMargin: 10
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                }
            }
            */
        }
        Rectangle {
            id: userCircle
            visible: false
            width:  100
            height: width
            radius: width * 0.5
            x: parent.width * 0.85 - width * 0.5
            y: parent.height * 0.65 - width * 0.5
            anchors.horizontalCenterOffset: -width / 2
            gradient: Gradient {
                GradientStop { position: 0.0; color: "black" }
                GradientStop { position: 1.0; color: "#EEE8EE" }
            }
            Rectangle {
                color: "#EEE8EE"
                visible: parent.visible
                width: parent.width * 0.75
                height: width
                radius: width * 0.5
                anchors.centerIn: parent
            }

            SequentialAnimation {
                id: userCircleAnimation
                running: false
                loops: Animation.Infinite // Infinite loops to keep rotating

                RotationAnimation {
                    target: userCircle
                    property: "rotation"
                    from: 0
                    to: 360
                    duration: 1000 // Duration of one full rotation
                    easing.type: Easing.Linear
                }
            }
        }
        Rectangle {
            visible: userCircle.visible
            x: parent.width * 0.7
            y: parent.height * 0.8
            width: parent.width * 0.3
            Text {
                id: userInfo
                anchors.fill: parent
                text: "Speak now"
                horizontalAlignment: Text.AlignHCenter
                font.pointSize: 14
            }
        }
        Rectangle { //userLabel
            x: parent.width * 0.7
            y: 35
            width: parent.width * 0.3
            Text {
                anchors.fill: parent
                text: "You"
                horizontalAlignment: Text.AlignHCenter
                font.pointSize: 14
            }
        }
        Rectangle {
            id: polaCircle
            visible: false
            width:  100
            height: width
            radius: width * 0.5
            x: parent.width * 0.15 - width * 0.5
            y: parent.height * 0.65 - width * 0.5
            anchors.horizontalCenterOffset: -width / 2
            gradient: Gradient {
                GradientStop { position: 0.0; color: "black" }
                GradientStop { position: 1.0; color: "#EEE8EE" }
            }
            Rectangle {
                color: "#EEE8EE"
                visible: parent.visible
                width: parent.width * 0.75
                height: width
                radius: width * 0.5
                anchors.centerIn: parent
            }
            SequentialAnimation {
                id: polaCircleAnimation
                running: false
                loops: Animation.Infinite // Infinite loops to keep rotating

                RotationAnimation {
                    target: polaCircle
                    property: "rotation"
                    from: 0
                    to: 360
                    duration: 1000 // Duration of one full rotation
                    easing.type: Easing.Linear
                }
            }
        }
        Rectangle {
            visible: polaCircle.visible
            x: 0
            y: parent.height * 0.8
            width: parent.width * 0.3
            Text {
                id: polaInfo
                anchors.fill: parent
                text: "pola info"
                horizontalAlignment: Text.AlignHCenter
                font.pointSize: 14
            }
        }
        Rectangle { //polalabel
            x: 0
            y: 35
            width: parent.width * 0.3
            Text {
                anchors.fill: parent
                text: "Pola"
                horizontalAlignment: Text.AlignHCenter
                font.pointSize: 14
            }

        }



        Image {
            source: "user.png" // Pfad zu Ihrem PNG-Bild
            width: 150
            height: width
            x: parent.width * 0.85 - width * 0.5
            y: parent.height * 0.3 - width * 0.5
            fillMode: Image.PreserveAspectFit // Beibehaltung des Seitenverhältnisses
        }
        Image {
            source: "pola.png" // Pfad zu Ihrem PNG-Bild
            width: 150
            height: width
            x: parent.width * 0.15 - width * 0.5
            y: parent.height * 0.3 - width * 0.5
            fillMode: Image.PreserveAspectFit // Beibehaltung des Seitenverhältnisses
        }

    }


    Connections {
        target: controller

        function onKeywordDetected() {
            wakeWordDetected = true;
            heypola.visible = true;
            appearanceTimer.start();
            userCircle.visible = true;
            userCircleAnimation.start();

            prompt.visible = false;
            response.visible = false;
        }

        function onRecordingDone() {
            userCircle.visible = false;
            userCircleAnimation.stop();

            polaInfo.text = "Wait for transcription";
            polaCircle.visible = true;
            polaCircleAnimation.start();

        }

        function onTranscriptionDone(text_t) {
            prompttext.text = text_t;
            prompt.visible = true;
            polaInfo.text = "Wait for Response";
        }

        function onLlamaResponse(text_r) {
            polaCircle.visible = false;
            polaCircle.visible = false;
            polaCircleAnimation.stop()
            responsetext.text = text_r;
            response.visible = true;
        }
        /*
        function onStateError() {
            error.visible = true
        }
        */
    }

    Component.onCompleted: {
        // Example usage:
        keywordDetected()
        transcriptionDone("What's the weather like?")
        llamaResponse("The weather is sunny with a high of 25°C.")
    }




}
