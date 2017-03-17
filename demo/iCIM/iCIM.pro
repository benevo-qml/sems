#-------------------------------------------------
#
# Project created by QtCreator 2013-10-12T14:57:24
#
#-------------------------------------------------

QT       += qml quick widgets

TEMPLATE = app

TARGET =iCIM

# SOURCE FILES
SOURCES += main.cpp

# HEDDERS FILES
HEADERS  += version.h

# Use Precompiled headers (PCH)
PRECOMPILED_HEADER  = Global.h

# UI FILES
FORMS    +=

# RC FILES
RC_FILE += resource.rc

DISTFILES +=


#- start -/////////////////////////////////////////////////////////////////////////////
#
# 作用
#
# a. 生成一般程序结构： 程序路径为app目录.
# b. qml文件，js文件，以及图片格式文件（jpg\gif\png\svg）自动添加到qrc文件内.
# c. 可控制是否将以上自动文件生成的qrc文件编译链接到目标文件中.



# 自定义程序结构
OUT_PWD = $$PWD/../../app
TEMP_DIR = $$PWD/tmp

DESTDIR = $$OUT_PWD/bin
MOC_DIR += $$TEMP_DIR/moc
UI_DIR += $$TEMP_DIR/ui

# 编译选项
DEFINES     +=  #QML_IN_QRC_FILE

CONFIG+=debug_and_release
CONFIG(debug, debug|release) {
    TARGET = $${TARGET}d
    OBJECTS_DIR += $$TEMP_DIR/debug/obj

    DEFINES += _DEBUG
    #LIBS += -L../lib1 -lhellod

} else {
    TARGET = $${TARGET}
    OBJECTS_DIR += $$TEMP_DIR/release/obj

    #LIBS += -L../lib2 -lhello
}

#----------------------------------------------------------------
# search directories.

searchlist += \
    *.qml \
    *.js \
    *.png \
    *.jpg \
    *.gif \
    *.svg \
    *qmldir

for (searchvar, searchlist) {
    qrclist += $$files($$searchvar, true)
}

#----------------------------------------------------------------
# 创建 qml.qrc 文件： 包含qrclist内所有的文件
RESOURCE_CONTENT = \
    "<RCC>" \
    "    <qresource prefix=\"/\"> "
for (qrcvar, qrclist) {
        resourcefileabsolutepath = $$absolute_path($$qrcvar)
        relativepath_in = $$relative_path($$resourcefileabsolutepath, $$PWD)
#        relativepath_out = $$relative_path($$resourcefileabsolutepath, $$OUT_PWD)
        RESOURCE_CONTENT += "<file alias=\"$$relativepath_in\">$$relativepath_in</file>"
}
RESOURCE_CONTENT += \
    '    </qresource>' \
    </RCC>

GENERATED_RESOURCE_FILE = $$PWD/qml.qrc
QMAKE_CLEAN += $$GENERATED_RESOURCE_FILE
write_file($$GENERATED_RESOURCE_FILE, RESOURCE_CONTENT)
#----------------------------------------------------------------
# 是否将qml.qrc文件编译到exe
contains(DEFINES,QML_IN_QRC_FILE){
    message("qml FILES will complied to a .qrc FILE !")
    RESOURCES += $$GENERATED_RESOURCE_FILE  # qml文件编译到qrc文件
    #QMAKE_PRE_LINK += $(DEL_FILE) $$GENERATED_RESOURCE_FILE
} else {

}
#----------------------------------------------------------------

OTHER_FILES += $$qrclist # QML folder


#- end - //////////////////////////////////////////////////////////////////////////////
