import QtQuick 2.4
import Material 0.2
import QtQuick.Controls 1.3 as Controls

import "shared"

Item {
    anchors.fill: parent

    Item {
        anchors.fill: parent

        Flickable {
            id: flickable
            anchors.fill: parent
            contentHeight: content.height
            contentWidth: content.width
            clip: true

            View {
                id: content
                x: 30
                y: 30
                width: 3840
                height: 2160
                elevation: 1

                Button {
                    id: btn
                    x: 10
                    y: 10
                    elevation: 1
                    activeFocusOnPress: true
                    backgroundColor: Theme.accentColor
                    text: "FBI"

                    onClicked: {
                        actionSheet.visible = !actionSheet.visible
                    }
                }

                ActionBox {
                    id: actionSheet
                    parent: btn

                    anchors {
                        top: btn.bottom
                    }

                    actions: [
                        Action {
                            iconName: "social/share"
                            name: "Share"
                        },

                        Action {
                            iconName: "file/file_download"
                            name: "Download (Disabled)"
                            enabled: false
                        },

                        Action {
                            iconName: "action/autorenew"
                            name: "THIS SHOULD BE HIDDEN"
                            visible: false
                        },

                        Action {
                            iconName: "action/settings"
                            name: "Details"
                            hasDividerAfter: true
                        },

                        Action {
                            iconName: "content/forward"
                            name: "Move"
                        },

                        Action {
                            iconName: "action/delete"
                            name: "Delete"
                        },

                        Action {
                            iconName: "content/create"
                            name: "Rename"
                        }
                    ]
                }

                SearchBox {
                    anchors {
                        left: btn.right
                        leftMargin: 13
                        verticalCenter: btn.verticalCenter
                    }
                    y: 10
                }
            }

        }

        Scrollbar {
            flickableItem: flickable
        }

        Scrollbar {
            flickableItem: flickable
            orientation: Qt.Horizontal
        }
    }

    ActionButton {
        anchors {
            right: parent.right
            bottom: snackbar.top
            margins: dp(32)
        }

        action: Action {
            id: addContent
            text: "&Copy"
            shortcut: "Ctrl+C"
            onTriggered: snackbar.open("We do actions too!")
        }
        iconName: "content/add"
    }

    Snackbar {
        id: snackbar
    }
}
