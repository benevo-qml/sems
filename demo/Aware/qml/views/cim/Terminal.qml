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
   \qmltype ConnectivityNode
   \inqmlmodule Material

   \brief Visual circular indicator of progress in some operation.
*/
Rectangle {
    id: connNode

    // PhysicsItem properties
    property alias b2fixture: fixture

    radius: dp(5)
    height: 2 * radius
    width: 2 * radius
    color: fixture.clrShow

    property int elecType: CIM.Terminal

    signal beginContact(Fixture other)
    signal endContact(Fixture other)
    signal mouseReleased(point mouse)

    Circle {
        id: fixture

        property int cimType: CIM.Terminal
        property bool isColliding: false
        property color clrPowerOn: "red"
        readonly property color clrNormal: "steelblue"
        property color clrShow: clrNormal
        property point collidingPoint
        property int collidingCimType: -1
        property int collidingW: -1
        property int collidingH: -1
        property int collidingAngle: 0
        property int angle: 0
        property int width: 2 * radius
        property int height: 2 * radius

        radius: connNode.radius
        density: 1
        friction: 1
        restitution: 0
        sensor: true    // 刚体只检测AABB，不参与碰撞

        function getWorldCenter(){
            return getBody().toWorldPoint(Qt.point(x + radius, y + radius))
        }

        onBeginContact: {
            collidingCimType = other.cimType
            collidingH = other.height
            collidingW = other.width
            collidingAngle = other.angle
            if (other.cimType === CIM.Terminal){  // 和连接性结点碰撞
                clrShow = clrPowerOn
                isColliding = true
                other.clrShow = clrPowerOn
                other.isColliding = true
                collidingPoint = other.getWorldCenter()
            } else if (other.cimType === CIM.BusbarSection){
                clrShow = clrPowerOn
                isColliding = true
                other.clrShow = clrPowerOn
                other.isColliding = true
                collidingPoint = other.getWorldCenter()
            }

            connNode.beginContact(other)
        }

        onEndContact: {
            collidingCimType = -1
            collidingH = -1
            collidingW = -1
            collidingAngle = 0
            collidingCimType = -1
            if (other.cimType === CIM.Terminal){
                clrShow = clrNormal
                isColliding = false
                other.clrShow = clrNormal
                other.isColliding = false
            }

            connNode.endContact(other)
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent

        onReleased: {
            if (fixture.isColliding){
                if (fixture.collidingCimType === CIM.Terminal){
                    fixture.getBody().target.x += fixture.collidingPoint.x - getWorldCenter().x
                    fixture.getBody().target.y += fixture.collidingPoint.y - getWorldCenter().y
                }else if (fixture.collidingCimType === CIM.BusbarSection){
                    if (fixture.collidingH < fixture.collidingW){
                        if (fixture.collidingAngle === 0 || fixture.collidingAngle === 180)
                            fixture.getBody().target.y += fixture.collidingPoint.y - getWorldCenter().y;
                        else
                            fixture.getBody().target.x += fixture.collidingPoint.x - getWorldCenter().x;
                    }else{
                        if (fixture.collidingAngle === 0 || fixture.collidingAngle === 180)
                            fixture.getBody().target.x += fixture.collidingPoint.x - getWorldCenter().x;
                        else
                            fixture.getBody().target.y += fixture.collidingPoint.y - getWorldCenter().y;
                    }
                }

//                console.log(getWorldCenter() + priv(fixture.collidingPoint, getWorldCenter())
//                            + ", " + fixture.collidingPoint)
            }
        }

        onPressed: {
        }
    }

    onMouseReleased: {
        mouseArea.onReleased(mouse)
    }

    function getWorldCenter(){
        return fixture.getWorldCenter()
    }

    function priv(p1, p2){
        return Qt.point(p1.x - p2.x, p1.y - p2.y)
    }
}
