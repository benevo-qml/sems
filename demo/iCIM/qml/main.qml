import QtQuick 2.3
import QtQuick.Layouts 1.1
import QtGraphicalEffects 1.0

import "./views"
import "./views/controls"

 Item {
     id: root
     width: mainwindow.width
     height: mainwindow.height

     property bool isMaximized : false

     function max_restore()
     {
         sysbtngrp.change_max_pic(root.isMaximized)
         if (root.isMaximized){
//             effect.offset = 30
             effect.glowRadius = 4
             mainwindow.showNormal()
         } else {
//             effect.offset = 0
             effect.glowRadius = 0
             mainwindow.showMaximized();
         }
         isMaximized = !isMaximized;
//         console.log(effect.offset)
     }

     O3ShadowItem
     {
        id: effect
        glowRadius: 4
        spread: 0.1
        color: "black"
        glowOpacity: 0.75
        width: content.width
        height: content.height

        property int offset : glowRadius * 4

        item: Rectangle
        {
            id: content
            width: mainwindow.width - effect.offset
            height: mainwindow.height - effect.offset

            radius: 5

             Image {
                 id: skinpic
                 x: 0
                 y: 0
                 fillMode: Image.PreserveAspectCrop
                 anchors.fill: parent
                 sourceSize.height: 600
                 sourceSize.width: 800
                 source: "../app/data/image/skin/background/photoshop cc.png"
             }

             MouseArea
             {
                 anchors.fill: parent
                 property variant previousPosition
                 onPressed: {
                     previousPosition = Qt.point(mouseX, mouseY)
                 }
                 onPositionChanged: {
                     if (pressedButtons == Qt.LeftButton && !isMaximized) {
                         var dx = mouseX - previousPosition.x
                         var dy = mouseY - previousPosition.y
                         mainwindow.setX(mainwindow.x+dx)
                         mainwindow.setY(mainwindow.y+dy)
                     }
                 }
                 onDoubleClicked: {
                     if (mouseY < 50)
                     {
                         root.max_restore();
                     }
                     console.log(effect.offset);
                 }
             }

             /* navi panel */
             Rectangle
             {
                 id: navi_panel
                 width: parent.width
                 height: 50
                 color: "#92e6a2"
                 z:10

                 RowLayout
                 {
                    id: layoutRow
                    anchors.fill:  parent
                    spacing: 1

                    // 用户登录
                    Rectangle
                    {
                        width:  219 + 7;
                        Layout.fillHeight: true;
//                        color: "#f3a51c"
                        color: "transparent"
//                        border.color: "red"

                        O3MaskImage
                        {
                            x: 7
                            height: parent.height - 4
                            radius: 20
                            anchors.verticalCenter: parent.verticalCenter
                            source: "../image/usrlogo.jpg"
                            fillMode: Image.PreserveAspectFit
                        }
                    }

                    // 分页栏
                    Rectangle
                    {
                        width:  375;
                        Layout.fillHeight: true;
                        color: "#00000000"

                        NaviBar
                        {
                            anchors.fill: parent
                            onCurrentTlbChanged: maincnt.currentChanged(btnIndex)
                        }
                    }

                    Rectangle
                    {
                        Layout.fillWidth:  true;
                    }

                    // 系统工具栏
                    Image
                    {
                        id: medal
                        anchors.top: parent.top
                        y: 0
                        //source: "image/medal.png"
                    }

                    Column
                    {
                        Layout.fillHeight: true
                        spacing: 3

                        SysBtnGroup
                        {
                            id: sysbtngrp
                            y: 0

                            onMin: mainwindow.showMinimized()
                            onMax: {
                                root.max_restore();
                            }
                            onClose: mainwindow.close()
                        }

                        Text {
                            id: title
                            anchors.right: parent.right
                            anchors.rightMargin: 12
                            height: 12
                            color: "#ffffff"
                            text: "iCIM Compare"
                            font.pointSize: 9
                            font.family: "微软雅黑"
                            font.pixelSize: 12
                        }
                    }
                 }
             }

             StackWidget
             {
                 id: maincnt
                 z: navi_panel.z - 1
                 width: parent.width
                 anchors.top: navi_panel.bottom
                 anchors.topMargin: 0
                 anchors.bottom: parent.bottom
             }
        }
    }
}
