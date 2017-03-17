/*
 * QML Material - An application framework implementing Material Design.
 * Copyright (C) 2014-2015 Michael Spencer <sonrisesoftware@gmail.com>
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
import QtGraphicalEffects 1.0
import QtQuick.Layouts 1.1
import Material 0.2
import Material.ListItems 0.1 as ListItem

/*!
   \qmltype Standard
   \inqmlmodule Material.ListItems

   \brief A simple list item with a single line of text and optional primary and secondary actions.
 */
ListItem.BaseListItem {
    id: listItem

    implicitHeight: dp(48)
    height: dp(48)

    property int indent : dp(16)

    property bool expanded: false
    property alias text: label.text

    property alias showIcon: iconItem.children
    property alias iconName: icon.name
    property alias iconSource: icon.source

    property alias content: contentItem.children
    property alias secondaryItem: moreItem.children
    property alias subContent: subContentItem.children

    property alias itemLabel: label

    property alias textColor: label.color
    property alias iconColor: icon.color

    dividerInset: iconItem.visible ? listItem.height : 0

    interactive: contentItem.children.length === 0

    implicitWidth: {
        var width = listItem.margins * 2

        if (iconItem.visible)
            width += iconItem.width + row.spacing

        if (contentItem.visible)
            width += contentItem.implicitWidth + row.spacing

        if (contentItem.visible)
            width += label.implicitWidth + row.spacing

        if (secondaryItem.visible)
            width += secondaryItem.width + row.spacing

        width += expander.implicitWidth + row.spacing

        return width
    }

    ColumnLayout {
        id: column

        anchors.fill: parent
        anchors {
            fill: parent
            leftMargin: indent > 0 ? indent : listItem.margins
            rightMargin: listItem.margins
        }

        RowLayout {
            id: row

            Layout.fillWidth: true

            spacing: dp(8)

            Item {
                id: iconItem

                Layout.preferredWidth: dp(40)
                Layout.preferredHeight: width
                Layout.alignment: Qt.AlignCenter

                visible: children.length > 1 || icon.valid

                Icon {
                    id: icon

                    anchors {
                        verticalCenter: parent.verticalCenter
                        left: parent.left
                    }

                    visible: valid
                    color: listItem.selected ? Theme.primaryColor : Theme.light.iconColor
                    size: dp(24)
                }
            }

            Label {
                id: label

                Layout.alignment: Qt.AlignVCenter

                width: dp(80)

                elide: Text.ElideRight
                style: "subheading"

                color: listItem.selected ? Theme.primaryColor : Theme.light.textColor
            }

            Item {
                id: contentItem

                height: dp(30)

                Layout.fillWidth: true
                Layout.preferredHeight: height
                Layout.alignment: Qt.AlignVCenter

                visible: children.length > 0
            }

            Item {
                id: fillItem

//                height: dp(16)

                Layout.fillWidth: true
                Layout.preferredHeight: height
                Layout.alignment: Qt.AlignVCenter

                visible: contentItem.children.length === 0
            }

            Item {
                id: moreItem

                Layout.preferredWidth: dp(32)
                Layout.preferredHeight: width
                Layout.alignment: Qt.AlignRight

                visible: contentItemFocused()
            }

            Item {
                id: expander
                Layout.preferredWidth: dp(40)
                Layout.preferredHeight: width
                Layout.alignment: Qt.AlignRight

                visible: subContentItem.children.length === 0

                Icon {
                    anchors {
                        verticalCenter: parent.verticalCenter
                        left: parent.left
                    }

                    name: "navigation/expand_more"
                    rotation: listItem.expanded ? 180 : 0
                    size: dp(24)

                    Behavior on rotation {
                        NumberAnimation { duration: 200 }
                    }
                }
            }
        }

        Item {
            id: subContentItem

            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignHCenter

            visible: listItem.expanded
        }
    }

    onClicked: {listItem.expanded = !listItem.expanded}

    Component.onCompleted: {
//        debug()
    }

    function contentItemFocused() {
        var list = contentItem.children;

        if (list.length > 0)
            return list[0].focus;
        else
            return false
    }

    function debug() {
        var list = contentItem.children;

        console.log("count: " + list.length);
        for ( var i in list) {
            console.log("list[ " +i + " ] objectName = " + list[i].objectName)
            console.log("list[ " +i + " ] width = " + list[i].width)
            console.log("list[ " +i + " ] height = " + list[i].height)
            console.log("list[ " +i + " ] focus = " + list[i].focus)
        }
    }
}
