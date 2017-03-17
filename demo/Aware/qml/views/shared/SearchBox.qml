/*
 * QML Material - An application framework implementing Material Design.
 * Copyright (C) 2015 Michael Spencer <sonrisesoftware@gmail.com>
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
import QtQuick.Layouts 1.1

import Material 0.2
import Material.ListItems 0.1

/*!
   \qmltype MenuField
   \inqmlmodule Material

   \brief A input search similar to a text search but that opens a dropdown menu.
 */
View {
    id: search

    height: dp(30)
    width: spinBox.implicitWidth
    radius: dp(4)
    activeFocusOnTab: true

    property bool isHovered: false

    Rectangle {
        id: focusRec
        anchors.fill: parent
        border.color: Theme.accentColor
        radius: search.radius
        visible: search.elevation > 0
    }

    RowLayout {
        id: spinBox

        anchors {
            centerIn: parent
            margins: dp(0)
        }
        implicitWidth: dp(120)

        Icon {
            id: dropDownIcon

            Layout.alignment: Qt.AlignCenter
            Layout.leftMargin: dp(4)
            Layout.rightMargin: label.visible ? 0 : dp(4)

            name: "action/search"
            size: dp(22)
        }

        TextField {
            id: label

            placeholderText: "Search..."
            implicitWidth: dp(100)

            showBorder: false
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignVCenter
            Layout.rightMargin: dp(4)
            text: ""

            font.family: "Roboto"
            font.pixelSize: dp(13)
            visible: isHovered
            focus: true
        }

    }

    MouseArea {
        anchors.fill: parent
        propagateComposedEvents: true
        hoverEnabled: true
        focus: true

        onEntered: {
            search.elevation = 1
            search.isHovered = true
            label.focus = true
        }

        onExited:{
            if (label.text.trim() === ""){
                search.elevation = 0
                search.isHovered = false
            }
        }
    }
}
