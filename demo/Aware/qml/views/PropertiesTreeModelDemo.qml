import QtQuick 2.4
import Material 0.2
import Material.ListItems 0.1 as ListItem
import Material.Extras 0.1
import Qt.labs.folderlistmodel 2.1
import QtQuick.Controls 1.4 as Controls
import QtQuick.Layouts 1.1
import QuickWoo 1.0

Item {
    id: section
    state: "list"

    ColumnLayout {
        anchors {
            fill: parent
            centerIn: parent
            margins: dp(24)
        }

        spacing: dp(16)

        View {
            height: dp(54)
//            elevation: 1
            radius: dp(4)

            Layout.alignment: Qt.AlignHCenter
            Layout.fillWidth: true

            RowLayout {
                anchors.centerIn: parent

                spacing: dp(8)

                Label {
                    id: titleLabel

                    Layout.alignment: Qt.AlignVCenter

                    text: "Meta Type: "
                    style: "title"
                }

                TextField {
                    id: metalibText
                    Layout.alignment: Qt.AlignVCenter

                    text: cimobj.className()
                    placeholderText: "Metalib Class Name"
//                    floatingLabel: true

                    onAccepted: reloadProp2()
                }

                Button {
                    Layout.alignment: Qt.AlignVCenter

                    height: parent.height
                    width: dp(40)

                    text: "Apply"

                    onClicked: reloadProp2()
                }
            }
        }

        function expandAll() {
        }

        View {
            elevation: 1
            radius: dp(2)

            Layout.alignment: Qt.AlignHCenter
            Layout.fillWidth: true
            Layout.fillHeight: true

            Controls.TreeView {
                id: propView
                anchors.fill: parent
                model: propModel

                onDoubleClicked : {
                    if (isExpanded(index))
                        collapse(index)
                    else
                        expand(index)
                }

                itemDelegate: TextField {
                    text: styleData.value === undefined ? "" : styleData.value

                    enabled: styleData.column !== 0

                    font {
                        family: "Noto Sans S Chinese Regular"
                        weight: Font.Light
                        pixelSize: dp(10)
                    }
                }

                Controls.TableViewColumn {
                    title: "Name"
                    role: "role_name"
                    resizable: true
                    width: propView.width / 2
                }

                Controls.TableViewColumn {
                    title: "Value"
                    role: "role_value"
                    width: propView.width / 2
                }
            }

//            Loader {
//                id: proploader;

//                anchors {
//                    fill: parent
//                    margins: dp(8)
//                }

//                asynchronous: true
//                visible: status == Loader.Ready
//                source: ""
//                sourceComponent: undefined

//                onLoaded: {
//                    snackbar.open(cimobj.className() + " loaded.  TimeElapsed: " + cimobj.elapsed() / 1000)
//                }
//            }
        }

    }

    Component {
        id: propComponet

        Item {
            anchors.fill: parent

            Flickable {
                id: flickable
                anchors.fill: parent
                contentHeight: content.childrenRect.height

                ColumnLayout {
                    id: content
                    width: parent.width
                    spacing: 0

                    // class info
                    ListItem.SectionHeader {
                        id: classHeader

                        text: "Class Definition (" + cimobj.cim_classInfoCount() + ")"
                        Layout.fillWidth: true

                        ThinDivider {
                            anchors {
                                left: parent.left
                                right: parent.right
                                top: parent.top
                            }
                            visible: classHeader.expanded
                        }
                    }

                    ListView {
                        model: cimobj.cim_classInfoCount()
                        visible: classHeader.expanded
                        height: childrenRect.height
                        Layout.fillWidth: true
                        spacing: 0

                        delegate: PropertyItem {
                            id: me

                            showIcon: Icon {
                                anchors.centerIn: parent
                                name: "action/book"
                                color: me.contentItemFocused() ? Theme.accentColor : Theme.primaryColor
                            }

                            text: cimobj.cim_classInfoName(index)

                            content: TextField {
                                anchors.centerIn: parent
                                width: parent.width

                                text: cimobj.cim_classInfoValue(index)
                            }

                            secondaryItem: IconButton {
                                iconName: "editor/mode_edit"
                                color: me.contentItemFocused() ? Theme.accentColor : Theme.backgroundColor
                                anchors.verticalCenter: parent.verticalCenter

                                onClicked: colorPicker.show()
                            }
                        }
                    }

                    // Properties
                    ListItem.SectionHeader {
                        id: propHeader
                        text: "Properties (" + cimobj.cim_propertyCount() + ")"

                        Layout.fillWidth: true

                        ThinDivider {
                            anchors {
                                left: parent.left
                                right: parent.right
                                top: parent.top
                            }
                            visible: propHeader.expanded
                        }
                    }

                    ListView {
                        visible: propHeader.expanded
                        height: childrenRect.height
                        Layout.fillWidth: true
                        spacing: 0

                        model: cimobj.cim_propertyCount()
                        delegate: ColumnLayout {
                            width: parent.width
                            property variant headerModel: model
                            spacing: 0
                            PropertyItem {
                                id: propItemHeader

                                Layout.fillWidth: true

                                showIcon: Icon {
                                    anchors.centerIn: parent
                                    name: "action/book"
                                    color: propItemHeader.contentItemFocused() ? Theme.accentColor : Theme.primaryColor
                                }

                                text: cimobj.cim_propertyName(index) +
                                      " (" + cimobj.cim_propertyInfoCount(index) + ")"

                                secondaryItem: IconButton {
                                    iconName: "editor/mode_edit"
                                    color: propItemHeader.contentItemFocused() ? Theme.accentColor : Theme.backgroundColor
                                    anchors.verticalCenter: parent.verticalCenter

                                    onClicked: colorPicker.show()
                                }
                            }
                            ListView {
                                visible:  propItemHeader.expanded
                                Layout.fillWidth: true
                                height: childrenRect.height
                                spacing: 0

                                model: cimobj.cim_propertyInfoCount(headerModel.index)
                                delegate: PropertyItem {
                                    id: sub
                                    width: parent.width
                                    height: propItemHeader.height
                                    indent: propItemHeader.indent * 3

                                    showIcon: Icon {
                                        anchors.centerIn: parent
                                        name: "action/account_circle"
                                        color: sub.contentItemFocused() ? Theme.accentColor : Theme.primaryColor
                                    }

                                    text: cimobj.cim_propertyInfoName(headerModel.index, index)

                                    content: TextField {
                                        anchors.centerIn: parent
                                        width: parent.width

                                        text: cimobj.cim_propertyInfoValue(headerModel.index, index)
                                    }

                                    secondaryItem: IconButton {
                                        iconName: "editor/mode_edit"
                                        color: sub.contentItemFocused() ? Theme.accentColor : Theme.backgroundColor
                                        anchors.verticalCenter: parent.verticalCenter

                                        onClicked: colorPicker.show()
                                    }
                                }
                            }
                        }
                    }
                }
            }

            Scrollbar {
                flickableItem: flickable
            }
        }
    }

    ProgressCircle {
        anchors.centerIn: section

        visible: false//proploader.status === Loader.Loading
    }

    ActionButton {
        anchors {
            right: parent.right
            bottom: parent.bottom
            margins: dp(32)
        }

        iconName: section.state === "list" ? "action/view_module"
                                           : "action/view_list"

        onClicked: {
            if (section.state === "list")
                section.state = "module"
            else
                section.state = "list"
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

    RdfPropertyModel {
        id: propModel
    }

    QuickTypeObject {
        id: cimobj
    }

    function reloadProp(){
        if (metalibText.text.trim() === "")
            return

        cimobj.startElapseTimer();
        proploader.sourceComponent = undefined
        if (cimobj.cim_loadMetaObject(metalibText.text.trim())){
            proploader.sourceComponent = propComponet
        }else
            snackbar.open(metalibText.text + " not found !")

    }

    function reloadProp2(){
        if (metalibText.text.trim() === "")
            return

        cimobj.startElapseTimer();
        propModel.retrieveNewModel(metalibText.text.trim());

//        proploader.sourceComponent = undefined
//        if (cimobj.cim_loadMetaObject(metalibText.text.trim())){
//            proploader.sourceComponent = propComponet
//        }else
//            snackbar.open(metalibText.text + " not found !")

    }

    states: [
        State {
            name: "list"
        },
        State {
            name: "module"
        }
    ]
}

