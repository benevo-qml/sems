import QtQuick 2.4
import Material 0.2
import QtQuick.Controls 1.3 as Controls
import QtQuick.Layouts 1.1

import "shared"
import "osx86"

Item {
    anchors.fill: parent

    Image {
        anchors.fill: parent
        source: ""
        fillMode: Image.PreserveAspectCrop
    }

    Flow {
        id: flow
//        anchors.centerIn: parent
        anchors {
            horizontalCenter: parent.horizontalCenter
            top: parent.top
            topMargin: dp(42)
        }

        spacing: dp(8)

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

    }

    PageInfo {
        height: dp(800)

        anchors {
            left: flow.left
            right: flow.right
            top: flow.bottom
            topMargin: dp(12)
        }

        anchors.horizontalCenter: flow.horizontalCenter
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
