import QtQuick 2.15

Rectangle {
    id: chatBubble
    y: yPos
    height: 20
    width: 50
    color: "#00F000"
    radius: 10

    property string text
    property bool isUser
    property int yPos

    anchors.left: parent.left
    anchors.leftMargin: isUser ? parent.width * 0.4 : parent.width * 0.5
    anchors.right: parent.right
    anchors.rightMargin: isUser ? parent.width * 0.95 : parent.width * 0.6

    Text {
        text: parent.text
        color: "green"
        wrapMode: Text.Wrap
    }

}
