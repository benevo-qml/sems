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
   \qmltype TransformerWinding
   \inqmlmodule Material

   \brief Visual circular indicator of progress in some operation.
*/
Item {
    id: winding

    property int ratio: 1
    width: dp(40) * ratio
    height: width * 3 / 2

    property int dplineLen: width / 2
    property int windingType: 0
    property int windingPos: 0
    property alias radiusWinding: canvas.radius

    property bool isOpen: false
    property color colorUnPowerOn: "gray"
    property color colorPowerOn: "red"
    property color lineColor: windingPos === 0 ? "red" : (windingPos === 1 ? "green" : "steelblue")

    property int elecType: CIM.TransformerWinding

    Rectangle {
        anchors.fill: parent
        color: "transparent"

        border.color: "red"
        visible: false
    }

    Canvas {
        id: canvas

        property int lineWidth: dp(2)
        property int ratio: Screen.devicePixelRatio
        property int radius: winding.width / 2 - lineWidth / 2

        width: parent.width * ratio
        height: parent.height * ratio
        anchors.centerIn: parent

        scale: 1/ratio

        onWidthChanged: requestPaint()
        onHeightChanged: requestPaint()

        renderStrategy: Canvas.Threaded
        antialiasing: true
        onPaint: draw ()

        opacity:  visible ? 1.0 : 0

        function draw() {
            var ctx = getContext("2d")
            // 设置画笔
            ctx.lineWidth = 2
            ctx.strokeStyle = lineColor
            ctx.fillStyle = (winding.isOpen ? colorUnPowerOn : colorPowerOn)

            // 开始绘制路径
            ctx.beginPath()
            ctx.moveTo(winding.width / 2, 0)
            ctx.lineTo(winding.width / 2, dplineLen)
            drawCircle(winding.width / 2, dplineLen, radius)

            var rad_ratio = 0.5
            if (windingType === 0){
                ctx.moveTo(winding.width / 2 - lineWidth / 2, dplineLen + radius * (1 - rad_ratio))
                ctx.lineTo(winding.width / 2 - lineWidth / 2, dplineLen + radius)
                ctx.lineTo(winding.width / 2 - lineWidth / 2 - radius * rad_ratio * Math.sqrt(3) / 2,
                           dplineLen + radius + radius * rad_ratio / 2)
                ctx.moveTo(winding.width / 2 - lineWidth / 2, dplineLen + radius)
                ctx.lineTo(winding.width / 2 - lineWidth / 2 + radius * rad_ratio * Math.sqrt(3) / 2,
                           dplineLen + radius + radius * rad_ratio / 2)
            }else if (windingType === 1){
                ctx.moveTo(winding.width / 2 - lineWidth / 2, dplineLen + radius * (1 - rad_ratio))
                ctx.lineTo(winding.width / 2 - lineWidth / 2 - radius * rad_ratio * Math.sqrt(3) / 2,
                           dplineLen + radius + radius * rad_ratio / 2)
                ctx.lineTo(winding.width / 2 - lineWidth / 2 + radius * rad_ratio * Math.sqrt(3) / 2,
                           dplineLen + radius + radius * rad_ratio / 2)
                ctx.lineTo(winding.width / 2 - lineWidth / 2, dplineLen + radius * (1 - rad_ratio))
            }


            ctx.stroke()
        }

        Component.onCompleted: {
            requestPaint();
        }

        function drawCircle(a, b, radius){
//            ctx.beginPath()
            var ctx = getContext("2d")
            ctx.moveTo(a, b)
            ctx.arcTo(a + radius, b, a + radius, b + radius, radius)
            ctx.arcTo(a + radius, b + 2 * radius, a, b + 2 * radius, radius)
            ctx.arcTo(a - radius, b + 2 * radius, a - radius, b + radius, radius)
            ctx.arcTo(a - radius, b, a, b, radius)
//            ctx.stroke()
        }
    }

//    Terminal {
//        id: ter
//        radius: radiusTer

//        x: winding.width / 2 - radius
//        y: -radius
//    }

    Timer {
        running: true
        interval: 1000
        repeat: true

        onTriggered: {
            canvas.requestPaint()
        }
    }
}
