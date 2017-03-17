import QtQuick 2.4
import Material 0.2
import Material.ListItems 0.1 as ListItem
import Material.Extras 0.1
import Qt.labs.folderlistmodel 2.1
import QtQuick.Controls 1.4 as Controls
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.1 as Dialogs
import QuickWoo 1.0
import "cim"
import Box2D 2.0
import "shared"

Item {
    id: section
    state: "list"

    property var actions: ["ADD", "RDF Split", "RDF MOC", "C++ Generator"]
    property bool busy: false

    ColumnLayout {
        anchors {
            fill: parent
            margins: dp(0)
        }
        spacing: dp(0)

        View {
            height: dp(36)

            elevation: 1
            Layout.fillWidth: true
            backgroundColor: Theme.backgroundColor

            RowLayout {
                anchors {
                    fill: parent
                    margins: dp(11)
                }
                spacing: dp(8)

                Controls.TextField{
                    id: pathRDF
                     Layout.fillWidth: true
                     text: "D:/devel/Studio/SEMS/app/data/cim/rdf/cim_schema.rdf"
                     height: dp(26)
                     anchors {
                         verticalCenter: parent.verticalCenter
                     }
                }

                IconButton {
                    height: dp(20)

                    anchors {
                        verticalCenter: parent.verticalCenter
                    }

                    iconName: "editor/insert_drive_file"
                    onClicked: {
                        if (0 == index){
                            openFileDlg.open()
                            return
                        }
                    }
                }
            }
        }

        Item {
            Layout.fillHeight: true
            Layout.fillWidth: true
        }

        View {
            id: actionsBar
            height: dp(52)

            elevation: 1
            elevationInverted: true
            Layout.fillWidth: true
            backgroundColor: Theme.backgroundColor


            ProgressBar {
                opacity: 0.5
                id: progessBar
                anchors.fill: parent
                color: Theme.accentColor
                value: quickObj.progress
                visible: value > 0 && value < 1

                Behavior on value {
                    PropertyAnimation {
                        id: valueAnima
                        duration: 1000
                    }
                }

                MouseArea {
                    anchors.fill: parent
                    propagateComposedEvents: true
                }

                onVisibleChanged:  {
                    if (value === 1)
                    {
                        mytimer.stop();
                        actionsBar.enabled = true;
                        console.log("ProgressBar: finished!\n")
                    }
                }
            }

            RowLayout {
                anchors {
                    fill: parent
                    margins: dp(11)
                }
                spacing: dp(8)

                Repeater {
                    model: actions

                    delegate: Button {

                        height: dp(30)
                        anchors {
                            verticalCenter: parent.verticalCenter
                        }

                        text: modelData

                        onHoveredChanged: {
                            if (elevation == 0){
                                elevation = 1;
                                backgroundColor = Theme.primaryColor
                            }else{
                                backgroundColor = Theme.backgroundColor
                                elevation = 0;
                            }
                        }
                        onClicked: {
                            if (0 == index){
//                                openFileDlg.open()
                                view2d.addBreakers(2)
                                return
                            }

                            if (!quickObj.isRdfExist(pathRDF.text)){
                                snackbar.open("RDF file path is invalid!")
                                pathRDF.focus = true;
                                return
                            }

                            quickObj.progress = 0;
                            mytimer.start()
                            quickObj.runRdfTask(index, pathRDF.text,
                                                quickObj.metalib_dbPath(),
                                                pathRDF.text.substring(0, pathRDF.text.lastIndexOf("/")))
                            actionsBar.enabled = false
                        }
                    }
                }

                Item {
                    Layout.fillWidth: true
                }
            }
        }
    }

//    Dialogs.FileDialog {
//        id: openFileDlg
//        title: "Open File"
//        nameFilters: [ "CIM files (*.xml *.rdf)", "All files (*)" ]
//        folder: ".."
////                                     selectFolder: true;
//        onAccepted: {
//            console.log(openFileDlg.fileUrl);
//            pathRDF.text = openFileDlg.fileUrl
//            pathRDF.text = pathRDF.text.replace("file:///", "")
//        }

//        onRejected: { console.log("Canceled") }
//    }

    Component {
        id: propComponent

        ColumnLayout {
            Layout.fillWidth: true
            ListItem.SectionHeader {
                id: classHeader

                border.color: "red"

                text: "Class Definition"
                Layout.fillWidth: true
                expanded: true

                ThinDivider {
                    anchors {
                        left: parent.left
                        right: parent.right
                        top: parent.top
                    }
                    visible: classHeader.expanded
                }
            }

            Repeater {
                model: 2

                delegate: ColumnLayout {
                    visible: classHeader.expanded

                    PropertyItem {
                        id: me
                        Layout.fillWidth: true

                        showIcon: Icon {
                            anchors.centerIn: parent
                            name: "action/book"
                            color: me.contentItemFocused() ? Theme.accentColor : Theme.primaryColor
                        }

                        text: "action/book"

                        secondaryItem: IconButton {
                            anchors.fill: parent
                            iconName: "editor/mode_edit"
                            anchors.verticalCenter: parent.verticalCenter
                            color: me.contentItemFocused() ? Theme.accentColor : Theme.backgroundColor

                            onClicked: colorPicker.show()
                        }
                    }


                    Repeater {
                        model: 2

                        delegate: ColumnLayout {
                            visible:  me.expanded

                            PropertyItem {
                                id: sub
                                Layout.fillWidth: true

                                indent: me.indent * 2

                                showIcon: Icon {
                                    anchors.centerIn: parent
                                    name: "action/book"
                                    color: sub.contentItemFocused() ? Theme.accentColor : Theme.primaryColor
                                }

                                text: "sub/book"

                                content: TextField {
                                    width: parent.width
                                    text: "action/book"
                                }

                                secondaryItem: IconButton {
                                    anchors.fill: parent
                                    iconName: "editor/mode_edit"
                                    anchors.verticalCenter: parent.verticalCenter
                                    color: sub.contentItemFocused() ? Theme.accentColor : Theme.backgroundColor

                                    onClicked: colorPicker.show()
                                }
                            }

                        }
                    }
                }
            }
        }
    }

    ActionButton {
        anchors {
            right: parent.right
            bottom: parent.bottom
            margins: dp(32)
        }

        iconName: lb.visible ? "" : (section.state === "list" ? "action/view_module"
                                           : "action/view_list")
        Label {
            id: lb
            anchors.centerIn: parent
            text: Math.round(progessBar.value* 100)  + "%"
            visible: progessBar.value > 0 && progessBar.value < 1
            color: Theme.backgroundColor
        }

        MouseArea {
            anchors.fill: parent
            drag.target: parent

            onClicked: {
                if (section.state === "list")
                    section.state = "module"
                else
                    section.state = "list"

                view2d.switchDebugDraw()
            }
        }
    }

    Snackbar {
        id: snackbar
    }

    states: [
        State {
            name: "list"
        },
        State {
            name: "module"
        }
    ]


    AwesomeIcon {
        id: awesomeIcon
        visible: false
    }

    Icon {
        id: icon
        visible: false
    }

    QuickTypeObject {
        id: quickObj

        onProgressChanged: {
//            if (progress === 1)
//                console.log("QuickTypeObject: onProgressChanged")
        }
    }

    Timer {
        id: mytimer

        interval: 200
        repeat: true

        onTriggered: {
            quickObj.progressChanged()
//            console.log("Timer: retrie progress ... " + progessBar.value * 100 + " %");
        }

        running: false
    }


    function reloadProp(){
        if (metalibText.text.trim() === "")
            return

        loader.sourceComponent = undefined
        if (quickObj.cim_loadMetaObject(metalibText.text.trim())){
            loader.sourceComponent = propComponent
        }else
            snackbar.open(metalibText.text + " not found !")

    }
}

