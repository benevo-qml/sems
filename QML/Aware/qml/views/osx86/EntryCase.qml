import QtQuick 2.4
import Material 0.3
import QtQuick.Controls 1.3 as Controls
import QtQuick.Layouts 1.1

/*!
  \qmltype EntryCase

  \desc outline for osx86.cn
  */

View{
//    height: dp(232) + dp(16)
    elevation: 1
    radius: 2

    property alias title: title_case.text
    property alias desc: case_desc.text
    property alias preview: casePreview.source
    property alias redu: redu.text
    property alias labelButton: caseBtn.text

    ColumnLayout {
        anchors.fill: parent

        RowLayout {
            width: parent.width
            height: dp(20)
            visible: redu.text !== ""

            Item {
                anchors.fill: parent
                Layout.fillWidth: true
            }

            Label {
                id: redu
                text: ""
                color: "white"
                Rectangle {
                    anchors.fill: parent
                    color: "#c40000"
                    z: parent.z -1
                }
            }
        }

        RowLayout {
            width: parent.width
            height: dp(26)

            Rectangle {
                height: parent.height
                width: dp(5)
                color: Theme.accentColor//"#c40000"
            }

            Button {
                id: title_case
                implicitHeight: parent.height
                Layout.fillWidth: true
                text: ""
                Layout.alignment: Qt.AlignHCenter


                MagicDivider {
                    anchors {
                        left: parent.left
                        right: parent.right
                        top: parent.bottom
                    }

                    styleDivider:  2
                    dash_len: 3
                    color: Theme.accentColor
                }
            }
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.margins: dp(6)

            Image {
                id: casePreview
                source: ""
                fillMode: Image.PreserveAspectCrop
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignVCenter
            }

            ColumnLayout {
                Layout.fillHeight: true
                Layout.fillWidth: true

                Label {
                    id: case_desc
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    text: ""
                    wrapMode: Text.WordWrap
                    style: "body1"
                }

                Button {
                    id: caseBtn
                    elevation: 1
                    activeFocusOnPress: true
                    backgroundColor: Theme.accentColor
                    text: "Enter"
                    implicitHeight: dp(30)

                    Layout.alignment: Qt.AlignRight

                    onClicked: {
                        console.log("enter ...")
                    }
                }
            }
        }
    }
}
