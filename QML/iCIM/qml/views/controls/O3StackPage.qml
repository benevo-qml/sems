import QtQuick 2.3

Rectangle
{
    id:me

    width:parent.width
    height:parent.height
    color:"white"
    state:"hide"

    property int index: 0

    states:
    [
        State
        {
            name:"active"
            PropertyChanges
            {
                target: me
                x:parent.x
            }

        },
        State
        {
            name:"hide"
            PropertyChanges
            {
                target: me
                x:parent.x - width - 10
            }
        }

    ]

    transitions:
    [
        Transition
        {
            PropertyAnimation
            {
                property:"x"
                duration: 200
                easing.type: Easing.Linear//.OutBounce
                easing.amplitude: 0.1
            }
        }
    ]
}
