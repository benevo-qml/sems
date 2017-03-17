import QtQuick 2.3

import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.1

import "./controls"

Rectangle
{
    width:parent.width
    height:parent.height
    color:"transparent"

    /* Tool bar */
    Rectangle
    {
        id: taskBar
        width: parent.width
        anchors.top: parent.top
        anchors.topMargin: -3;
        height: 52
        z: tabwidget.z - 1
        color: "#f2f2f2"

        O3ShadowItem
        {
             id: taskBarGlow
             glowRadius: 3
             spread: 0.1
             color: "gray"
             glowOpacity: 0.75
             width: taskBarRect.width
             height: taskBarRect.height
             glowLeftWidth: 0;
             glowRightWidth: 0;
             glowTopHeight: 0;
             biasY: 5

             item: Rectangle
             {
                 id: taskBarRect
                 x: 0
                 width: taskBar.width
                 height: 52

                 color: "#f2f2f2"

                 RowLayout
                 {
                     anchors.fill: parent
                     anchors.verticalCenter: parent.verticalCenter
                     spacing: 12
                     anchors.leftMargin: 12
                     anchors.rightMargin: 12

                     O3ToolButton
                     {
                         text: "open"
                         implicitHeight:   30
                         implicitWidth: 70

                         font.pointSize: 9
                         spacing: 8
                         radius: 4

                         image.source: "../../image/open_file.png"

                         gradientNormal: ["#43d25c", "#43d25c"]
                         gradientHovered: ["#5cb85c", "#5cb85c"]

                         onClicked:openFileDlg.open()
                         FileDialog
                         {
                             id: openFileDlg
                             title: "Open File"
                             nameFilters: [ "CIM files (*.xml *.rdf)", "All files (*)" ]
                             folder: ".."
//                                     selectFolder: true;
                             onAccepted: { console.log(openFileDlg.fileUrl); textShow.text = openFileDlg.fileUrl }
                             onRejected: { console.log("Canceled") }
                         }
                     }

                     ComboBox {
                         currentIndex: 2
                         model: ListModel {
                             id: model1
                             ListElement { text: "Banana"; color: "Yellow" }
                             ListElement { text: "Apple"; color: "Green" }
                             ListElement { text: "Coconut"; color: "Brown" }
                         }
                         width: 80
                         height: 30
                         implicitWidth: 80
                         implicitHeight: 30
                         anchors.verticalCenter: parent.verticalCenter
                         onCurrentIndexChanged: console.debug(model1.get(currentIndex).text + ", " + model1.get(currentIndex).color)
                     }

                     ComboBox {
                         currentIndex: 2
                         model: ListModel {
                             id: cbItems
                             ListElement { text: "Banana"; color: "Yellow" }
                             ListElement { text: "Apple"; color: "Green" }
                             ListElement { text: "Coconut"; color: "Brown" }
                         }
                         width: 80
                         height: 30
                         implicitWidth: 80
                         implicitHeight: 30

                         anchors.verticalCenter: parent.verticalCenter
                         onCurrentIndexChanged: console.debug(cbItems.get(currentIndex).text + ", " + cbItems.get(currentIndex).color)
                     }

                     O3FlatToolButton
                     {
                         id:skin
                         anchors.verticalCenter: parent.verticalCenter
                         picHover: "../../../image/search.png"
                         picNormal: "../../../image/search.png"
                         picPressed: "../../../image/search.png"
                         clrHover: "#02aed9"
                         clrPressed : "#02aed9"
                         opacity: 0.4
                         onClicked:
                         {
                             console.log("skin btn clicked")
                             sysbtngroup.skin()
                         }
                     }

                     Rectangle
                     {
                         Layout.fillWidth:  true;
                     }

                     O3FlatToolButton
                     {
                         id: openBtn
                         anchors.verticalCenter: parent.verticalCenter
                         picHover: "../../../image/open_file - Copy.png"
                         picNormal: "../../../image/open_file - Copy.png"
                         picPressed: "../../../image/open_file - Copy.png"
                         clrHover: "#02aed9"
                         clrPressed : "#02aed9"
                         opacity: 0.4
                         onClicked:
                         {
                             console.log("skin btn clicked")
                             sysbtngroup.skin()
                         }
                     }

                 }
             }
        }
    }


    RowLayout
    {
        width: parent.width
        anchors.top: taskBar.bottom
        anchors.bottom: parent.bottom
        anchors.topMargin: -2
//        spacing: 3
        spacing: 0

        SplitView {
            anchors.fill: parent
            orientation: Qt.Horizontal
            handleDelegate: handler

            Rectangle {
                width: 240
                implicitHeight: parent.height

                color: "#d33e3e"

                TextArea{
                    anchors.fill: parent
                    font.pointSize: 12
                    font.family: "Raavi"

                    text:
                        "Lorem ipsum dolor sit amet, consectetur adipisicing elit, " +
                        "sed do eiusmod tempor incididunt ut labore et dolore magna " +
                        "aliqua. Ut enim ad minim veniam, quis nostrud exercitation " +
                        "ullamco laboris nisi ut aliquip ex ea commodo cosnsequat. ";
                }
            }

            Rectangle {
                Layout.fillWidth: true
                implicitHeight: parent.height

                color:"#99FF0000"

                TextArea {
                    id: textShow
                    anchors.fill: parent

                    font.pointSize: 12
                    font.family: "Raavi"
                    text:
                        "Lorem ipsum dolor sit amet, consectetur adipisicing elit, " +
                        "sed do eiusmod tempor incididunt ut labore et dolore magna " +
                        "aliqua. Ut enim ad minim veniam, quis nostrud exercitation " +
                        "ullamco laboris nisi ut aliquip ex ea commodo cosnsequat. ";
                }
            }
        }

        Component {
            id : handler

            Rectangle {
                implicitHeight: parent.height
                width: 3

                color: "white"

//                MouseArea {
//                    anchors.fill: parent;
//                    hoverEnabled: true;
//                    cursorShape: (containsMouse
//                                  ? Qt.SplitHCursor
//                                  : Qt.ArrowCursor);
//                }
            }
        }


    }

}
