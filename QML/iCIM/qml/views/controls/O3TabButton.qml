import QtQuick 2.3

Rectangle
{
    id:me
    height: parent.height
    width: 70
    gradient: on

    property alias picSrc: pic.source
    property alias btnText: text.text
    property alias btnTextColor: text.color
    property alias spacing: rowLayout.spacing

    property int index:0
    property color bkColor: "#00FFFFFF"

    signal clicked(int btnIndex)

    implicitWidth: 120
    implicitHeight: 60

    Gradient
    {
        id:on
        GradientStop { position: 0.0; color: "#00FFFFFF" }
        GradientStop { position: 1.0; color: bkColor }
    }

    Row
    {
        id: rowLayout
        anchors.centerIn: parent
        spacing: 5

        Image
        {
            id: pic
            source: "";
            anchors.verticalCenter: parent.verticalCenter
        }

        Text
        {
            id: text
            color: "#ffffff"
            font.pointSize: 10
            font.family: "微软雅黑"
            anchors.verticalCenter: parent.verticalCenter
         }
    }

    MouseArea
    {
        hoverEnabled: true
        anchors.fill: parent

        onEntered:
        {
            me.state === 'checked' ? me.state = 'checked' : me.state = 'hover'
        }
        onExited:
        {
            me.state == 'checked' ? me.state = 'checked' : me.state = 'leave'
        }
        onPressed:
        {
            me.state = 'checked'
            me.clicked(me.index)
        }
    }

    states:
    [
        State {
            name: "checked"
            PropertyChanges { target: me; bkColor:"#77FFFFFF" }
        },

        State {
            name: "hover"
            PropertyChanges { target: me; bkColor:"#77FFFFFF" }
        },

        State {
            name: "leave"
            PropertyChanges { target: me; bkColor:"#00FFFFFF" }
        }
    ]

    transitions:
    [
        Transition
        {
            PropertyAnimation
            {
                property:"bkColor"
                easing.type: Easing.Linear
                duration: 200
            }
        }
    ]
}
