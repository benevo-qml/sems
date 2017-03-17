import QtQuick 2.3

Rectangle
{
    id:me

    property string picCurrent: ""
    property string picNormal: ""
    property string picHover: ""
    property string picPressed: ""
    property color clrNormal:"transparent"
    property color clrHover:"transparent"
    property color clrPressed:"transparent"

    signal clicked

    width: 27
    height: 22
    color:"#00000000"
    state:"normal"

    Image
    {
        anchors.centerIn: parent
        source: picCurrent;
    }

    MouseArea
    {
        hoverEnabled: true
        anchors.fill: parent
        onEntered: me.state == "pressed" ? me.state = "pressed" : me.state = "hover"
        onExited: me.state == "pressed" ? me.state = "pressed" : me.state = "normal"
        onPressed: me.state = "pressed"
        onReleased:
        {
            me.state = "normal"
            me.clicked()
        }
    }

    states:
    [
        State{
            name:"hover"
            PropertyChanges {
                target: me
                picCurrent: (picHover!="" ? picHover : picNormal)
                opacity: (picHover!="" ? 1 : 0.8)
                color: clrHover
            }
        },
        State {
            name: "normal"
            PropertyChanges {
                target: me
                picCurrent:picNormal
                color: clrNormal
            }
        },
        State {
            name: "pressed"
            PropertyChanges {
                target: me
                picCurrent: (picPressed!="" ? picPressed : picNormal)
                opacity: (picPressed!="" ? 1 : 0.8)
                color: clrPressed
            }
        }

    ]
}
