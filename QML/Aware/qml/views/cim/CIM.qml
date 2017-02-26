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

import Material 0.2

pragma Singleton

/*!
   \qmltype CIM
   \inqmlmodule Material

   \brief Provides access to standard colors that follow the Material Design specification.

   See \l {http://www.google.com/design/spec/style/color.html#color-ui-color-application} for
   details about choosing a color scheme for your application.
 */
Object {
    id: cim


    /*!
       The primary color used for the toolbar background unless a page specifies its own color.
       This can be customized via the \l ApplicationWindow::cim group property. According to the
       Material Design guidelines, this should normally be a 500 color from one of the color
       palettes at \l {http://www.google.com/design/spec/style/color.html#color-color-palette}.
     */
    property color primaryColor: "#FAFAFA"

}
