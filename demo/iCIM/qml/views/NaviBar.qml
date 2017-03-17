import QtQuick 2.3

import "./controls"

Row
{
    id: me
    spacing: 6
    anchors.left: tabwidget.left
    anchors.leftMargin: 3
    property int current: 0
    onCurrentChanged:setCurrentToolBtn()

    signal currentTlbChanged(int btnIndex)

    function setCurrentToolBtn()
    {
        for(var i = 0; i < me.children.length; i++)
        {
            me.children[i].state = (me.current == i ? 'checked' : 'leave')
        }
        me.currentTlbChanged(me.current)
    }

    O3TabButton
    {
        index: 0

        width: 48
        state:"checked"

        picSrc: "../../image/home_normal.png"
        btnText:""
        onClicked: me.current = btnIndex
    }

    O3TabButton
    {
        index: 1

        width: 105
        picSrc: "../../image/netnavi_normal.png"
        btnText: "网络资源"
        onClicked: me.current = btnIndex
    }

    O3TabButton
    {
        index: 2

        width: 110
        picSrc: "../../image/more_tools.png"
        btnText: "更多应用"
        onClicked: me.current = btnIndex
    }
}

