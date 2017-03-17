import QtQuick 2.3

import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.1

import "./controls"

Rectangle
{
    width:parent.width
    height:parent.height
    color:"transparent"


    O3ToolButton
    {
        text: "open"
        implicitHeight:   30
        implicitWidth: 70

        font.pointSize: 9
        spacing: 8
        radius: 4

        image.source: "../../image/open_file.png"

        gradientNormal: ["#43d25c", "#43d25c"]
        gradientHovered: ["#5cb85c", "#5cb85c"]

        onClicked:openFileDlg.open()
        FileDialog
        {
            id: openFileDlg
            title: "Open File"
            nameFilters: [ "CIM files (*.xml *.rdf)", "All files (*)" ]
            folder: ".."
//                                     selectFolder: true;
            onAccepted: { console.log(openFileDlg.fileUrl); textShow.text = openFileDlg.fileUrl }
            onRejected: { console.log("Canceled") }
        }
    }

    Image
    {
        x: 50
        y: 50
        source:"../../image/error.png"
    }

    Row
    {
        x: 250
        y: 55

        Text
        {
            color:"black"
            font.pointSize: 13
            font.family: "微软雅黑"
            text:"您的电脑已经 "
        }

        Text
        {
            color:"red"
            font.pointSize: 15
            font.family: "微软雅黑"
            text:"24"
        }

        Text
        {
            color:"black"
            font.pointSize: 13
            font.family: "微软雅黑"
            text:" 天没有体检，建议立即体检！"
        }
    }

    Text
    {
        x:250
        y:100
        color:"#AA000000"
        font.pointSize: 10
        font.family: "微软雅黑"
        text:"系统可能已经存在大量风险，安全性和性能都在急速下降，\n建议您每天坚持电脑体检，提高电脑的安全和性能"
    }

//    O3FlatToolButton
//    {
//        x:250
//        y:250
//        width:166
//        height:66
//        picNormal: "../../../image/check_normal.png"
//        picHover: "../../../image/check_hover.png"
//        picPressed: "../../../image/check_pressed.png"
//        onClicked:
//        {
//            console.log("check btn clicked")
//        }
//    }

    O3FlatToolButton
    {
        x:250
        y:250
        width:166
        height:66

        picNormal: "../../../image/check_normal.png"

        onClicked:
        {
            console.log("check btn clicked")
        }
    }

    Image {
        id: image1
        x: 650
        y: 10
        source: "../../image/register_bg.png"

        AnimatedImage
        {
            fillMode:Image.PreserveAspectFit
            anchors.left: parent.left
            anchors.leftMargin: 100
            source: "../../image/panda.gif"
        }
    }

    Row
    {
        x : 665
        y : 145
        spacing:30

        O3FlatToolButton
        {
            width:50
            height:45
            picNormal: "../../../image/firewall_open_normal.png"
        }
        O3FlatToolButton
        {
            width:50
            height:45
            picNormal: "../../../image/guard_open_normal.png"
        }
        O3FlatToolButton
        {
            width:50
            height:45
            picNormal: "../../../image/payinsure_close_normal.png"
        }
    }

    Row
    {
        x: 658
        y: 190
        spacing: 32

        Text
        {
            color:"#AA000000"
            font.pointSize: 9
            font.family: "微软雅黑"
            text:"木马防火墙"
        }
        Text
        {
            color:"#AA000000"
            font.pointSize: 9
            font.family: "微软雅黑"
            text:"360保镖"
        }
        Text
        {
            color:"#AA000000"
            font.pointSize: 9
            font.family: "微软雅黑"
            text:"网购先赔"
        }
    }

    Rectangle
    {
        x: 645
        y: 0
        width:1
        height:parent.height
        color:"#BBBBBB"
    }

    Rectangle
    {
        x: 645
        y: 130
        width:260
        height:1
        color:"#BBBBBB"
    }

    Rectangle
    {
        x: 645
        y: 220
        width:260
        height:1
        color:"#BBBBBB"
    }

    Rectangle
    {
        x: 645+85
        y: 130
        width:1
        height:90
        color:"#BBBBBB"
    }

    Rectangle
    {
        x: 645+85*2
        y: 130
        width:1
        height:90
        color:"#BBBBBB"
    }

    Grid
    {
        x:680
        y:250
        columns: 3
        rows:3
        spacing:25

        LabelBtn
        {
            picSrc: "../../../image/net_pretext.png"
            text:"瞒天过海"
        }
        LabelBtn
        {
            picSrc: "../../../image/mobile.png"
            text:"指桑骂槐"
        }
        LabelBtn
        {
            picSrc: "../../../image/net_repair.png"
            text:"声东击西"
        }
        LabelBtn
        {
            picSrc: "../../../image/net_speed.png"
            text:"过河拆桥"
        }
        LabelBtn
        {
            picSrc: "../../../image/recovery.png"
            text:"趁火打劫"
        }
        LabelBtn
        {
            picSrc: "../../../image/net_pretext.png"
            text:"以逸待劳"
        }
        LabelBtn
        {
            picSrc: "../../../image/desktop.png"
            text:"顺手牵羊"
        }
        LabelBtn
        {
            picSrc: "../../../image/game_box.png"
            text:"借刀杀人"
        }
        LabelBtn
        {
            picSrc: "../../../image/first_add.png"
            text:"金蝉脱壳"
        }

    }

}
