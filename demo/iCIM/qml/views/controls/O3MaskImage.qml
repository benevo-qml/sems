import QtQuick 2.2
import QtGraphicalEffects 1.0

Rectangle {
    color: "transparent"
    width: usrlogo.width

    property alias source: usrlogo.source
    property alias fillMode: usrlogo.fillMode
    property alias radius: mask.radius

    Image {
        id: usrlogo; source: ""
        anchors.centerIn: parent
        sourceSize: Qt.size(parent.width, parent.height)
        smooth: true
        visible: false
    }

    Rectangle {
        id:mask

        radius: 50
        anchors.fill: usrlogo
        anchors.centerIn: usrlogo
        color: { parent.color == "transparent" ? parent.color : "white"}
    }

    OpacityMask {
        anchors.fill: usrlogo; source: usrlogo; maskSource: mask
    }
}

