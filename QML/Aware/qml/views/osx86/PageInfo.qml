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
    property alias preview: casePreview.source
    property alias redu: redu.text

    ColumnLayout {
        anchors.fill: parent

        RowLayout {
            width: parent.width
            height: dp(32)
            visible: redu.text !== ""
            Layout.margins: dp(10)

            Button {
                implicitHeight: dp(26)
                implicitWidth: dp(38)
                text: "A+"
                Layout.alignment: Qt.AlignVCenter
                Rectangle {
                    anchors.fill: parent
                    border.color: Theme.backgroundColor
                    z: -1
                }
            }

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

        Button {
            id: title_case
            implicitHeight: dp(26)
            Layout.fillWidth: true

            text: ""
            Layout.alignment: Qt.AlignHCenter

            Rectangle {
                id: bkground
                height: parent.height
                width: parent.width
                color: Theme.accentColor//"#c40000"
                z: -1
                opacity: 0.2
            }
        }

        ThinDivider {
            width: parent.width
            anchors.margins: dp(12)
            Layout.margins: dp(12)
            height:  dp(80)

            styleDivider:  3
            dash_len: 3
            color: Theme.accentColor

            Label {
                anchors {
                    left: parent.left
                    right: parent.right
                    leftMargin: dp(8)
                    rightMargin: dp(8)
                    verticalCenter: parent.verticalCenter
                }

                text: "MultiBeast是一款结合黑苹果常见的驱动和软件的工具包，用户可以根据自己的硬件需求选择性的安装更新你的黑苹果驱动。MultiBeast 8.1这次更新了一些网卡驱动包括AtherosE2200Ethernet v2.1.0 等这些网卡驱动程序，这个之前黑苹果乐园已经有发布，需要单独安装的可以在乐园搜索得到。"
                wrapMode: Text.WordWrap
                style: "body1"
            }
        }

        RowLayout {
            width: parent.width
            height: dp(30)
            Layout.margins: dp(12)

            Image {
                id: casePreview
                source: ""
                fillMode: Image.PreserveAspectCrop
                width: parent.width / 2
                Layout.alignment: Qt.AlignVCenter
            }

            Label {
                Layout.fillWidth: true
                text: ""
                wrapMode: Text.WordWrap
                style: "body1"
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
                implicitHeight: parent.height
                text: "Introduction"

                ThinDivider {
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

        Label {
            Layout.fillHeight: true
            anchors {
                left: parent.left
                right: parent.right
                leftMargin: dp(12)
                rightMargin: dp(12)
            }
            //            wrapMode: Text.WordWrap
            style: "body1"
            text: "最近帝都迎来了第一场雪。" + "够冷、一点也不浪漫。\n" +
                  "我踩在银白的沙堤上，你踏浪而来，水溅了我一身！！！\n" +
                  "就是我内心最真实的造影。\n" +
                  "做主题的时候，回头翻了翻以前的日记，\n" +
                  "时间过的真是太快了，\n" +
                  "曾经刻骨铭心的悲伤，现在也不过是，我嘴角微微卷起的苦笑。\n" +
                  "回头看看，一年又过去了。\n"
        }
    }
}
