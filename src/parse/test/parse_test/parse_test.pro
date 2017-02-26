QT       += core sql
QT       -= gui

TARGET = parse_test
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

HEADERS +=

SOURCES += main.cpp

#- start -/////////////////////////////////////////////////////////////////////////////
#
# 作用
#
# a. 生成一般程序结构： 程序路径为app目录.
# b. qml文件，js文件，以及图片格式文件（jpg\gif\png\svg）自动添加到qrc文件内.
# c. 可控制是否将以上自动文件生成的qrc文件编译链接到目标文件中.


# 自定义程序结构
OUT_DIR = $$PWD/../../../../app
TEMP_DIR = $$PWD/tmp
QML_DIR = $$PWD/qml
DAA_DIR = $${OUT_DIR}/data

DESTDIR = $$OUT_DIR/bin
MOC_DIR += $$TEMP_DIR/moc
UI_DIR += $$TEMP_DIR/ui

INCLUDEPATH += $$PWD/../../core

# 编译选项
DEFINES     +=  #QML_IN_QRC_FILE

CONFIG+=debug_and_release
CONFIG(debug, debug|release) {
    TARGET = $${TARGET}d
    OBJECTS_DIR += $$TEMP_DIR/debug/obj

    DEFINES += _DEBUG
    LIBS += -L$${DESTDIR} -lparsed

} else {
    TARGET = $${TARGET}
    OBJECTS_DIR += $$TEMP_DIR/release/obj

    LIBS += -L$${DESTDIR} -llibparse
}
