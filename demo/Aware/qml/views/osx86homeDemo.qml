import QtQuick 2.4
import Material 0.2
import QtQuick.Controls 1.3 as Controls
import QtQuick.Layouts 1.1

import "shared"
import "osx86"

Item {
    anchors.fill: parent

    Flickable {
        id: flickable
        clip: true

        anchors {
            fill: parent
        }

        topMargin: dp(8)
        leftMargin: dp(8)
        rightMargin: dp(8)
        bottomMargin: dp(8)

        contentHeight: Math.max(content.childrenRect.height, height)
        contentWidth: Math.max(content.childrenRect.width, width)

        Rectangle {
            id: content
            color: Theme.backgroundColor
            border.color: Theme.accentColor
            anchors.fill: parent

            ColumnLayout {
                spacing: dp(8)
                anchors.horizontalCenter: parent.horizontalCenter

                RowLayout {
                    spacing: dp(8)
                    Layout.margins: dp(12)
                    Layout.alignment: Qt.AlignCenter

                    EntryCase {
                        width: dp(400)
                        height: dp(232) + dp(16)
                        title: " JIANGXI PRO. GRID /2015"
                        desc: "这段时间看四叶草更新的很快啊，这是好事，最起码每一新版本都有新的修复或者添加新的功能，这次编译了最新Clover v2.3k r3434的PKG安装包和EIF文件夹，也打包了Win..."
                        preview: "http://osx86.cn/images/a.png"
                        redu: " 热 801 °"
                    }

                    EntryCase {
                        width: dp(400)
                        height: dp(232) + dp(16)
                        title: " JIANGXI PRO. GRID /2015"
                        desc: "这段时间看四叶草更新的很快啊，这是好事，最起码每一新版本都有新的修复或者添加新的功能，这次编译了最新Clover v2.3k r3434的PKG安装包和EIF文件夹，也打包了Win..."
                        preview: "http://osx86.cn/images/b.png"
                        redu: " 热 801 °"
                    }

                    EntryCase {
                        width: dp(400)
                        height: dp(232) + dp(16)
                        title: " JIANGXI PRO. GRID /2015"
                        desc: "这段时间看四叶草更新的很快啊，这是好事，最起码每一新版本都有新的修复或者添加新的功能，这次编译了最新Clover v2.3k r3434的PKG安装包和EIF文件夹，也打包了Win..."
                        preview: "http://osx86.cn/images/b.png"
                        redu: " 热 801 °"
                    }

                    EntryCase {
                        width: dp(400)
                        height: dp(232) + dp(16)
                        title: " JIANGXI PRO. GRID /2015"
                        desc: "这段时间看四叶草更新的很快啊，这是好事，最起码每一新版本都有新的修复或者添加新的功能，这次编译了最新Clover v2.3k r3434的PKG安装包和EIF文件夹，也打包了Win..."
                        preview: "http://osx86.cn/images/b.png"
                        redu: " 热 801 °"
                    }
                }

                PageInfo {
                    Layout.margins: dp(12)
                    Layout.alignment: Qt.AlignCenter

                    width: dp(800)
                    height: dp(632) + dp(16)
                    title: " JIANGXI PRO. GRID /2015"
                    preview: "http://osx86.cn/wp-content/themes/begin/img/random/17.jpg"
                    redu: " 热 801 °"
                }
            }
        }
    }

    Scrollbar {
        flickableItem: flickable
        orientation: Qt.Vertical
    }

    Scrollbar {
        flickableItem: flickable
        orientation: Qt.Horizontal
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
