import QtQuick 2.15

Rectangle {
    id: chatBubble
    width: parent.width * 0.8
    color: isUser ? "#aed581" : "#e1bee7"
    radius: 10

    property string text
    property bool isUser

    anchors.left: isUser ? parent.width * 0.4 : parent.width * 0.05
    anchors.right: isUser ? parent.width * 0.95 : parent.width * 0.6

    Column {
        anchors.fill: parent
        anchors.margins: 10

        Text {
            text: qsTr(chatBubble.text)
            color: "black"
            wrapMode: Text.Wrap
        }
    }
}
