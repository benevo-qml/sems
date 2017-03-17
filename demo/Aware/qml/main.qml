import QtQuick 2.4
import Material 0.3
import Material.ListItems 0.1 as ListItem
import "views"

ApplicationWindow {
    id: demo

    title: "Aware Workstation"// + page.title

    width: dp(900)
    height: dp(600)

    // Necessary when loading the window from C++
    visible: true

    flags: Qt.FramelessWindowHint | Qt.WindowSystemMenuHint | Qt.WindowMinimizeButtonHint| Qt.Window

    theme {
        primaryColor: "Teal"
        accentColor: "#2dc3e8"
        tabHighlightColor: "white"
    }

    property var homeComponents: [
        "home"
    ]

    property var basicComponents: [
        "Test", "World View 2D", "Class View Map", "Properties"//, "Button", "CheckBox", "Progress Bar", "Radio Button",
        //"Slider", "Switch", "TextField"
    ]

    property var styles: [
        "Icons", "Custom Icons", "Color Palette", "Typography"
    ]

    property var compoundComponents: [
        "Bottom Sheet", "Dialog", "Forms", "List Items", "Page Stack", "Time Picker", "Date Picker"
    ]

    property var sections: [ homeComponents, basicComponents/*, styles, compoundComponents */]

    property var sectionTitles: [ "Home", "Basic"/*, "styles Design", "compoundComponents Design" */]

    property string selectedComponent: sections[0][0]

    initialPage: TabbedPage {
        id: page

        title: "CIM 5" + " - " + demo.title

        actionBar.maxActionCount: navDrawer.enabled ? 3 : 4

        actions: [
            Action {
                iconName: "image/color_lens"
                name: "Colors"
                onTriggered: colorPicker.show()
            },

            Action {
                iconName: "action/settings"
                name: "Settings"
                hoverAnimation: true
                onTriggered: {
                    smallLoader.showSiderbar = !smallLoader.showSiderbar
                }
            },

            Action {
                iconName: "action/exit_to_app"
                name: "Exit"
                onTriggered: Qt.quit()
            },

            Action {
                iconName: "alert/warning"
                name: "Dummy error"
                onTriggered: demo.showError("Something went wrong", "Do you want to retry?", "Close", true)
            },

            Action {
                iconName: "alert/warning"
                name: "THIS SHOULD BE HIDDEN!"
                visible: false
            },

            Action {
                iconName: "action/language"
                name: "Language"
                enabled: false
            },

            Action {
                iconName: "action/account_circle"
                name: "Accounts"
            }
        ]

        backAction: navDrawer.action

        NavigationDrawer {
            id: navDrawer

            enabled: page.width < dp(500)

            onEnabledChanged: smallLoader.active = enabled

            Flickable {
                anchors.fill: parent

                contentHeight: Math.max(content.implicitHeight, height)

                Column {
                    id: content
                    anchors.fill: parent

                    Repeater {
                        model: sections

                        delegate: Column {
                            width: parent.width

                            ListItem.Subheader {
                                text: sectionTitles[index]
                            }

                            Repeater {
                                model: modelData
                                delegate: ListItem.Standard {
                                    text: modelData
                                    selected: modelData == demo.selectedComponent
                                    onClicked: {
                                        demo.selectedComponent = modelData
                                        navDrawer.close()
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        Repeater {
            model: !navDrawer.enabled ? sections : 0

            delegate: Tab {
                title: sectionTitles[index]

                property string selectedComponent: modelData[0]
                property var section: modelData

                sourceComponent: tabDelegate
            }
        }

        Loader {
            id: smallLoader
            property bool showSiderbar: true
            anchors.fill: parent
            sourceComponent: tabDelegate

            property var section: []
            visible: active
            active: false
        }
    }

    Dialog {
        id: colorPicker
        title: "Pick color"

        positiveButtonText: "Done"

        MenuField {
            id: selection
            model: ["Primary color", "Accent color", "Background color", "Theme color"]
            width: dp(160)
        }

        Grid {
            columns: 7
            spacing: dp(8)

            Repeater {
                model: [
                    "red", "pink", "purple", "deepPurple", "indigo",
                    "blue", "lightBlue", "cyan", "teal", "green",
                    "lightGreen", "lime", "yellow", "amber", "orange",
                    "deepOrange", "grey", "blueGrey", "brown", "black",
                    "white"
                ]

                Rectangle {
                    width: dp(30)
                    height: dp(30)
                    radius: dp(2)
                    color: Palette.colors[modelData]["500"]
                    border.width: modelData === "white" ? dp(2) : 0
                    border.color: Theme.alpha("#000", 0.26)

                    Ink {
                        anchors.fill: parent

                        onPressed: {
                            switch(selection.selectedIndex) {
                            case 0:
                                theme.primaryColor = parent.color
                                break;
                            case 1:
                                theme.accentColor = parent.color
                                break;
                            case 2:
                                theme.backgroundColor = parent.color
                                break;
                            case 3:
                                demo.theme.tabHighlightColor = parent.color
                                break;
                            }
                        }
                    }
                }
            }
        }

        onRejected: {
            // TODO set default colors again but we currently don't know what that is
        }
    }

    Component {
        id: tabDelegate

        Item {
            Sidebar {
                id: sidebar

                expanded: !navDrawer.enabled
                visible:  section.length > 1 && smallLoader.showSiderbar

                Column {
                    width: parent.width

                    Repeater {
                        model: section
                        delegate: ListItem.Standard {
                            text: modelData
                            selected: modelData == selectedComponent
                            onClicked: selectedComponent = modelData
                        }
                    }
                }
            }
            Flickable {
                id: flickable
                anchors {
                    left: sidebar.visible ? sidebar.right : parent.left
                    right: parent.right
                    top: parent.top
                    bottom: parent.bottom
                }
                clip: true
                contentHeight: Math.max(example.implicitHeight + 40, height)
                Loader {
                    id: example
                    anchors.fill: parent
                    asynchronous: true
                    visible: status == Loader.Ready
                    // selectedComponent will always be valid, as it defaults to the first component
                    source: {
                        if (navDrawer.enabled) {
                            return Qt.resolvedUrl("views/%1Demo.qml").arg(demo.selectedComponent.replace(/ /g, ""))
                        } else {
                            return Qt.resolvedUrl("views/%1Demo.qml").arg(selectedComponent.replace(/ /g, ""))
                        }
                    }
                }

                ProgressCircle {
                    anchors.centerIn: parent
                    visible: example.status == Loader.Loading
                }
            }
            Scrollbar {
                flickableItem: flickable
            }
        }
    }

    Snackbar {
        id: snackbar
    }
}
