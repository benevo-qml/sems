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
    id: busbarsection

    property int ratio: 1
    property int dpRadius: dp(3) * ratio
    width: dp(180) * ratio
    height: dp(8) * ratio

    property int dpRecW: width
    property int dpRecH: height

    property color colorUnPowerOn: "gray"
    property color colorPowerOn: "steelblue"

    property bool isWarning: false

    property int elecType: CIM.BusbarSection

    signal beginContact(Fixture other)
    signal endContact(Fixture other)

    Rectangle {
        anchors.fill: parent
        color: isWarning ? colorPowerOn : colorUnPowerOn

        width: parent.width
        height: parent.height
        radius: dpRadius
    }

//    Canvas {
//        id: canvas

//        property int ratio: Screen.devicePixelRatio

//        width: parent.width * ratio
//        height: parent.height * ratio
//        anchors.centerIn: parent

//        scale: 1/ratio

//        onWidthChanged: requestPaint()
//        onHeightChanged: requestPaint()

//        renderStrategy: Canvas.Threaded
//        antialiasing: true
//        onPaint: draw ()

//        opacity:  visible ? 1.0 : 0

//        //        Behavior on opacity {
//        //            PropertyAnimation {
//        //                duration: 800
//        //            }
//        //        }

//        function draw() {
//            var ctx = getContext("2d")
//            // 设置画笔
//            ctx.lineWidth = 2
//            ctx.strokeStyle = colorPowerOn
//            ctx.fillStyle = isWarning ? colorPowerOn : colorUnPowerOn

//            // 设置阴影属性
//            ctx.shadowColor = "gray";
//            ctx.shadowOffsetX = 0;
//            ctx.shadowOffsetY = .5;
//            ctx.shadowBlur = 0.2;

//            ctx.fillRect(dpRadius, 0, dpRecW - 2*dpRadius, dpRecH)
//        }

//        Component.onCompleted: {
//            requestPaint();
//        }
//    }

    Body {
        id: body
        target: busbarsection

        bodyType: Body.Dynamic
        sleepingAllowed: false
        fixedRotation: true
        bullet: true

        fixtures: Box {
            id: fixture

            property int cimType: CIM.BusbarSection
            property bool isColliding: false
            property color clrPowerOn: "red"
            readonly property color clrNormal: "steelblue"
            property color clrShow: clrNormal
            property point collidingPoint
            property int collidingCimType: -1
            property int collidingW: -1
            property int collidingH: -1
            property int collidingAngle: 0
            property int angle: busbarsection.rotation % 360

            width: busbarsection.dpRecW
            height: busbarsection.dpRecH

            density: 1
            friction: 1
            restitution: 0
            sensor: true    // 刚体只检测AABB，不参与碰撞

            function getWorldCenter(){
                return getBody().toWorldPoint(Qt.point(x + busbarsection.width/2, y + busbarsection.height / 2))
            }

            onBeginContact: {
                if (other.cimType === undefined)
                    return
                collidingCimType = other.cimType
                collidingH = other.height
                collidingW = other.width
                collidingAngle = other.angle
                if (other.cimType === CIM.Terminal){
                    clrShow = clrPowerOn
                    isColliding = true
                    other.clrShow = clrPowerOn
                    other.isColliding = true
                    collidingPoint = other.getWorldCenter()
                }

                busbarsection.beginContact(other)
            }

            onEndContact: {
                collidingCimType = -1
                collidingH = -1
                collidingW = -1
                collidingAngle = 0
                if (other.cimType === CIM.Terminal){
                    clrShow = clrNormal
                    isColliding = false
                    other.clrShow = clrNormal
                    other.isColliding = false
                }

                busbarsection.endContact(other)
            }
        }

    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        drag.target: parent

        acceptedButtons: Qt.LeftButton | Qt.RightButton
        propagateComposedEvents: true

        onClicked: {
            if (mouse.button === Qt.RightButton)
                busbarsection.rotation += 90
        }

        onReleased: {
            if (fixture.isColliding){
                if (fixture.collidingCimType === CIM.Terminal){
                    if (fixture.height < fixture.width){
                        if (fixture.angle === 0 || fixture.angle === 180)
                            fixture.getBody().target.y += fixture.collidingPoint.y - getWorldCenter().y;
                        else
                            fixture.getBody().target.x += fixture.collidingPoint.x - getWorldCenter().x;
                    }else{
                        if (fixture.angle === 0 || fixture.angle === 180)
                            fixture.getBody().target.x += fixture.collidingPoint.x - getWorldCenter().x;
                        else
                            fixture.getBody().target.y += fixture.collidingPoint.y - getWorldCenter().y;
                    }
                }

//                console.log(getWorldCenter() + priv(fixture.collidingPoint, getWorldCenter())
//                            + ", " + fixture.collidingPoint)
            }
        }
    }

    Timer {
        running: true
        interval: 1000
        repeat: true

        onTriggered: {
            busbarsection.isWarning = !busbarsection.isWarning
//            canvas.requestPaint();
        }
    }

    function getWorldCenter(){
        return body.toWorldPoint(Qt.point(busbarsection.width/2, busbarsection.height/2))
    }
}
