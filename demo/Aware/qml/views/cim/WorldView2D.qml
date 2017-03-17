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
import QtQuick.Layouts 1.1
import Material 0.2
import QuickWoo 1.0
import Box2D 2.0
import "../shared"

/*!
   \qmltype WorldView2D
   \inqmlmodule Material

   \brief Visual 2D View of Linker Map for Power System Devices I .
*/

Item {
    ColumnLayout {
        id: worldItem
        anchors {
            fill: parent
            margins: dp(0)
        }
        spacing: dp(0)

        View {
            height: dp(52)

            elevation: 1
            Layout.fillWidth: true
            backgroundColor: Theme.backgroundColor

            clip:false

            RowLayout {
                anchors {
                    fill: parent
                    margins: dp(11)
                }
                spacing: dp(12)

                Button {
                    id: addCimDev
                    text: "+ Add Equipment"
                    implicitHeight: dp(30)
                    elevation: 1
                    activeFocusOnPress: true
                    backgroundColor: Theme.accentColor
                    Layout.alignment: Qt.AlignVCenter

                    onClicked: {
                        actionSheet.visible = !actionSheet.visible
//                        actionSheet.open()
                    }
                }

                SearchBox {
                    height: dp(30)
                    Layout.alignment: Qt.AlignVCenter
                }

                Item {
                    Layout.fillWidth: true
                }

                Button {
                    text: "Actions Menu"
                    implicitHeight: dp(30)
                    elevation: 1
//                    activeFocusOnPress: true
                    backgroundColor: Theme.accentColor
//                    anchors.horizontalCenter: parent.horizontalCenter
                    Layout.alignment: Qt.AlignVCenter

                    onClicked: snackbar.open("That button is colored!")
                }
            }
        }

        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.margins: dp(0)

            Flickable {
                id: flickable
                clip: true
                anchors.fill: parent
                contentHeight: world2d.height
                contentWidth: world2d.width

                View {
                    id: world2d
                    elevation: 1

                    x: 30
                    y: 30
                    width: 3840
                    height: 2160

                    property double dpRatio: 2
                    property variant array: []
                    property var object
                    property var newComponent

                    backgroundColor: "black"
                    Image {
                        anchors.fill: parent
                        fillMode: Image.Tile
                        source: "../images/background.gif"
                        opacity: 0.8

                        visible: world2d.backgroundColor === "transparent"
                    }

                    MouseArea {
                        anchors.fill: parent
                        propagateComposedEvents: true

//                        onWheel: {
//                            if (wheel.angleDelta.y > 0)
//                                world2d.dpRatio += 0.5
//                            else if (wheel.angleDelta.y < 0)
//                                world2d.dpRatio -= 0.5
//                        }

                        onClicked: {
                            console.log(mouse)
                            console.log(world2d.dpRatio)
                        }
                    }

                    World {
                        id: physicsWorld
                        gravity: Qt.point(0, 0)
                    }

                    DebugDraw {
                        id: debugDraw
                        anchors.fill: parent
                        world: physicsWorld
                        opacity: 0.7
                        visible: false
                    }

                    BusbarSection {
                        x: 200
                        y: 80
                        ratio: world2d.dpRatio
                    }

                    Disconnector {
                        id: disconn01
                        x: 240
                        y: 80 + 4 * ratio
                        ratio: world2d.dpRatio
                    }

                    Breaker {
                        id: breaker01
                        x: 50
                        y: 160

                        ratio: world2d.dpRatio
                    }

                    PowerTransformer {
                        id: ptf01
                        x: 100
                        y: 120
                        ratio: world2d.dpRatio
                    }

                    Component.onCompleted: {
                        breaker01.x = disconn01.x
                        breaker01.y = disconn01.y + disconn01.height
                    }

                    function addDevice(typeName){
                        newComponent = Qt.createComponent(Qt.resolvedUrl(typeName + ".qml"))
                        if(Component.Ready === world2d.newComponent.status) {
                            world2d.object = world2d.newComponent.createObject(world2d, {"x": 10, "y": 10})
                            world2d.object.ratio = Qt.binding(function() { return dpRatio } )
//                            world2d.object.x = (world2d.object.width + dp(26)) * (array.length % 24 + 1)
//                            world2d.object.y = dp(80) + (world2d.object.height + dp(16)) * Math.floor(array.length / 12)
                            array.push(world2d.object)
                        }

                    }

                    function switchDebugDraw(){
                        debugDraw.visible = !debugDraw.visible
                    }
                }
            }

            Scrollbar {
                flickableItem: flickable
                orientation: Qt.Vertical
            }

            Scrollbar {
                flickableItem: flickable
                orientation: Qt.Horizontal
            }
        }
    }

    ActionButton {
        anchors {
            right: parent.right
            bottom: parent.bottom
            margins: dp(32)
        }

        iconName: (section.state === "list" ? "action/view_module"
                                            : "action/view_list")

        MouseArea {
            anchors.fill: parent
            drag.target: parent

            onClicked: {
                if (section.state === "list")
                    section.state = "module"
                else
                    section.state = "list"

                world2d.switchDebugDraw()

            }
        }
    }

    ActionBox {
        id: actionSheet
//        parent: worldItem

        width: dp(220)

//        anchors {
//            top: parent.bottom
//        }

        actions: [
            Action {
                iconName: "content/create"
                name: "BusbarSection"
                onTriggered: {
                    world2d.addDevice(name)
                }
            },

            Action {
                iconName: "social/share"
                name: "Breaker"
                onTriggered: {
                    world2d.addDevice(name)
                }
            },

            Action {
                iconName: "file/file_download"
                name: "Disconnector"
                onTriggered: {
                    world2d.addDevice(name)
                }
            },

            Action {
                iconName: "action/settings"
                name: "PowerTransformer"
                hasDividerAfter: true
                onTriggered: {
                    world2d.addDevice(name)
                }
            },

            Action {
                iconName: "content/forward"
                name: "Move"
            }
        ]
    }
}

