import QtQuick 2.3

import "./controls"

Row
{
    id: me
    spacing: 0

    signal skin
    signal feedback
    signal menu
    signal min
    signal max
    signal close

    function change_max_pic(isMax)
    {
        if (isMax){
            max.picNormal = "../../../image/max_normal.png"
        }else{
            max.picNormal = "../../../image/restore_normal.png"
        }
    }

    O3FlatToolButton
    {
        id:skin
        picNormal: "../../../image/skin_normal.png"
        clrHover: "#02aed9"
        clrPressed : "#02aed9"
        onClicked:
        {
            console.log("skin btn clicked")
            me.skin()
        }
    }

    O3FlatToolButton
    {
        id:feedback
        picNormal: "../../../image/feedback_normal.png"
        clrHover: "#02aed9"
        clrPressed : "#02aed9"
        onClicked:
        {
            console.log("feedback btn clicked")
            me.feedback()
        }
    }

    O3FlatToolButton
    {
        id:menu
        picNormal: "../../../image/menu_normal.png"
        clrHover: "#02aed9"
        clrPressed : "#02aed9"
        onClicked:
        {
            console.log("menu btn clicked")
            me.menu()
        }
    }

    O3FlatToolButton
    {
        id:min
        picNormal: "../../../image/min_normal.png"
        clrHover: "#02aed9"
        clrPressed : "#02aed9"
        onClicked:
        {
            console.log("min btn clicked")
            me.min()
        }
    }

    O3FlatToolButton
    {
        id:max
        picNormal: "../../../image/max_normal.png"
        clrHover: "#02aed9"
        clrPressed : "#02aed9"

        property bool isMaximized: false

        onClicked:
        {
            console.log("max btn clicked")
            me.max()
            me.change_max_pic(isMaximized)
            isMaximized = !isMaximized;
        }
    }

    O3FlatToolButton
    {
        id:close
        picNormal: "../../../image/close_normal.png"
        clrHover: "#da3610"
        clrPressed : "#a41b00"
        onClicked:
        {
            console.log("close btn clicked")
            me.close()
        }
    }
}
