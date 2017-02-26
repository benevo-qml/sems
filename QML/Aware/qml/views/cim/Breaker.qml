/*
 * QML Material - An application framework implementing Material Design.
 * Copyright (C) 2015 Jordan Neidlinger <JNeidlinger@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 2.1 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
import QtQuick 2.4
import QtQuick.Window 2.2
import QtQuick.Controls 1.3 as Controls
import QtQuick.Controls.Styles 1.3 as Styles
import Material 0.2
import QuickWoo 1.0
import Box2D 2.0
import "../shared"

/*!
   \qmltype Breaker
   \inqmlmodule Material

   \brief Visual circular indicator of progress in some operation.
*/
Item {
    id: breaker

    property int ratio: 1
    implicitHeight: dp(75) * ratio
    implicitWidth: dp(18) * ratio

    property int dpRecW: width
    property int dpRecH: height * 1 / 3
    property int dplineLen: height * 1 / 3
    property int dpRadius: dp(4) * ratio

    property bool isOpen: false
    property color colorUnPowerOn: "gray"
    property color colorPowerOn: "red"
    property color lineColor: "white"

    property int elecType: CIM.Breaker

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        drag.target: parent

        acceptedButtons: Qt.LeftButton | Qt.RightButton
        propagateComposedEvents: true

        onClicked: {
            if (mouse.button === Qt.RightButton)
                breaker.rotation += 90
        }

        onReleased: {
            ter1.mouseReleased(mouse)
            ter2.mouseReleased(mouse)
        }
    }

    Rectangle {
        anchors.fill: parent
        color: "transparent"

        border.color: "red"
        visible: false
    }

    Canvas {
        id: canvas

        property int ratio: Screen.devicePixelRatio

        width: parent.width * ratio
        height: parent.height * ratio
        anchors.centerIn: parent

        scale: 1/ratio

        onWidthChanged: requestPaint()
        onHeightChanged: requestPaint()

        renderStrategy: Canvas.Threaded
        antialiasing: true
        onPaint: drawBreaker ()

        opacity:  visible ? 1.0 : 0

        //        Behavior on opacity {
        //            PropertyAnimation {
        //                duration: 800
        //            }
        //        }

        QtObject {
            id: internal

            property real arcEndPoint: 0
            onArcEndPointChanged: canvas.requestPaint();

            property real arcStartPoint: 0
            onArcStartPointChanged: canvas.requestPaint();

            property real rotate: 0
            onRotateChanged: canvas.requestPaint();

            property real longDash: 3 * Math.PI / 2
            property real shortDash: 19 * Math.PI / 10
        }

        function drawBreaker() {
            var ctx = getContext("2d")
            // 设置画笔
            ctx.lineWidth = 2
            ctx.strokeStyle = lineColor
            ctx.fillStyle = (breaker.isOpen ? colorUnPowerOn : colorPowerOn)

            // 设置阴影属性
//            ctx.shadowColor = "transparent";
//            ctx.shadowOffsetX = 0;
//            ctx.shadowOffsetY = 0;
//            ctx.shadowBlur = 0;

            ctx.fillRect(0, dplineLen - ctx.lineWidth / 2, dpRecW, dpRecH)

            // 开始绘制路径
            ctx.beginPath()

            // 上边线
            ctx.moveTo(dpRecW / 2, 0)
            ctx.lineTo(dpRecW / 2, dplineLen)
            // 下边线
            ctx.moveTo(dpRecW / 2, dpRecH + dplineLen)
            ctx.lineTo(dpRecW / 2, dpRecH + dplineLen * 2)
            // 左边线，并结束路径

            ctx.moveTo(0, dplineLen)
            ctx.lineTo(dpRecW, dplineLen)
            ctx.lineTo(dpRecW, dplineLen + dpRecH)
            ctx.lineTo(0, dplineLen + dpRecH)
            ctx.lineTo(0, dplineLen)

            // 使用画笔绘制边线
            ctx.closePath()
            ctx.stroke()
        }

        Component.onCompleted: {
            requestPaint();
        }
    }

    Terminal {
        id: ter1
        radius: dpRadius

        x: dpRecW / 2 - dpRadius
        y: -dpRadius
    }

    Terminal {
        id: ter2
        radius: dpRadius

        x: dpRecW / 2 - dpRadius
        y: dplineLen * 2 + dpRecH - dpRadius
    }

    // 碰撞检测模型实体
    Body {
        id: body
        target: breaker

        bodyType: Body.Dynamic
        fixedRotation: true
        sleepingAllowed: false
        bullet: true

        Component.onCompleted: {
            ter1.b2fixture.x = Qt.binding(function() { return ter1.x })
            ter1.b2fixture.y = Qt.binding(function() { return ter1.y })
            addFixture(ter1.b2fixture);

            ter2.b2fixture.x = Qt.binding(function() { return ter2.x })
            ter2.b2fixture.y = Qt.binding(function() { return ter2.y })
            addFixture(ter2.b2fixture)
        }
    }

    Timer {
        running: true
        interval: 1000
        repeat: true

        onTriggered: {
            breaker.isOpen = !breaker.isOpen
            canvas.requestPaint()
        }
    }

    function getWorldCenter(){
        return body.toWorldPoint(Qt.point(breaker.width/2, breaker.height/2))
    }

    function getTerWorldCenter(idx){ // 此函数仅外部调用
        if (idx === 0)
            return ter1.getWorldCenter()
        else
            return ter2.getWorldCenter()
    }
}
