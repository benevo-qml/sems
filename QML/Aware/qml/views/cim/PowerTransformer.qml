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
    id: powertransformer

    property double ratio: 1
    implicitWidth: dp(40) * ratio
    implicitHeight: implicitWidth * 11 / 4//dp(110) * ratio

    property int radiusTer: dp(4) * ratio

    property bool isOpen: false
    property color colorUnPowerOn: "gray"
    property color colorPowerOn: "red"
    property color lineColor: "white"

    property int elecType: CIM.PowerTransformer

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        drag.target: parent

        acceptedButtons: Qt.LeftButton | Qt.RightButton
        propagateComposedEvents: true

        onClicked: {
            if (mouse.button === Qt.RightButton)
                powertransformer.rotation += 90
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

    TransformerWinding {
        anchors.top: powertransformer.top

        windingType: 0
        windingPos: 0
        width: powertransformer.width
        ratio: powertransformer.ratio
    }

    TransformerWinding {
        anchors.bottom: powertransformer.bottom

        windingType: 1
        windingPos: 3
        width: powertransformer.width
        ratio: powertransformer.ratio

        rotation: 180
    }

    Terminal {
        id: ter1
        radius: radiusTer

        x: powertransformer.width / 2 - radiusTer
        y: -radiusTer
    }

    Terminal {
        id: ter2
        radius: radiusTer

        x: powertransformer.width / 2 - radiusTer
        y: powertransformer.height - radiusTer
    }

    // 碰撞检测模型实体
    Body {
        id: body
        target: powertransformer

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

    function getWorldCenter(){
        return body.toWorldPoint(Qt.point(powertransformer.width/2, powertransformer.height/2))
    }

    function getTerWorldCenter(idx){ // 此函数仅外部调用
        if (idx === 0)
            return ter1.getWorldCenter()
        else
            return ter2.getWorldCenter()
    }
}
