import QtQuick 2.3

import "./controls"

Rectangle
{
    id: tabwidget
    color: "transparent"

    function currentChanged(curIndex)
    {
        content.currentChanged2(curIndex)
    }

    Rectangle
    {
        id:content
        width:tabwidget.width
        anchors.fill: parent

        property int current: 0

        function currentChanged2(curIndex)
        {
            content.children[curIndex].x=width
            content.children[curIndex].state='active'
            content.children[current].state='hide'
            current = curIndex
        }

        O3StackPage
        {
            state:"active"

            CIMFileView{}
        }

        O3StackPage
        {
            Rectangle
            {
                anchors.fill: parent
                anchors.margins: 50
                color:"#f95c76"
                Text
                {
                    font.pointSize: 20
                    font.family: "微软雅黑"
                    color:"#FFFFFF"
                    anchors.centerIn: parent
                    text:"一颗红豆，三两淡酒，四五流年够不够？"
                }
            }
        }

        O3StackPage
        {
            Content1 {}
        }
    }
}
