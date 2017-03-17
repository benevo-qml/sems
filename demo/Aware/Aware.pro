#-------------------------------------------------
#
# Project created by QtCreator 2013-10-12T14:57:24
#
#-------------------------------------------------

QT       += qml quick widgets

TEMPLATE = app

#CONFIG +=console

# Use Precompiled headers (PCH)
PRECOMPILED_HEADER  = Global.h

# HEDDERS FILES
HEADERS  += version.h

# SOURCE FILES
SOURCES += main.cpp

# UI FILES
FORMS    +=

# RC FILES
RC_FILE += resource.rc

DISTFILES +=

# include pri
include(core/core.pri)

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH = #$(QML2_IMPORT_PATH)
#message($$QML_IMPORT_PATH)

SDKInclude = $$PWD/../../sdk/include
INCLUDEPATH +=  $$SDKInclude

#- start -/////////////////////////////////////////////////////////////////////////////
#
# 作用
#
# a. 生成一般程序结构： 程序路径为app目录.
# b. qml文件，js文件，以及图片格式文件（jpg\gif\png\svg）自动添加到qrc文件内.
# c. 可控制是否将以上自动文件生成的qrc文件编译链接到目标文件中.


TARGET = $$qtLibraryTarget($$TARGET)

# 自定义程序结构
OUT_PWD = $$PWD/../../app
TEMP_DIR = $$PWD/tmp

DESTDIR = $$OUT_PWD/bin
MOC_DIR += $$TEMP_DIR/moc
UI_DIR += $$TEMP_DIR/ui

# 编译选项
DEFINES     +=  #Q_USE_QML_RC

CONFIG+=debug_and_release
CONFIG(debug, debug|release) {
#    TARGET = $${TARGET}d
    OBJECTS_DIR += $$TEMP_DIR/debug/obj

    DEFINES += _DEBUG
    LIBS += -L$${DESTDIR} -lmetalibd
    LIBS += -L$${DESTDIR} -litunesd
    LIBS += -L$${DESTDIR} -lparsed
    LIBS += -L$${DESTDIR} -lcimd

} else {
#    TARGET = $${TARGET}
    OBJECTS_DIR += $$TEMP_DIR/release/obj

    LIBS += -L$${DESTDIR} -lmetalib
    LIBS += -L$${DESTDIR} -litunes
    LIBS += -L$${DESTDIR} -lparse
    LIBS += -L$${DESTDIR} -lcim
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
contains(DEFINES,Q_USE_QML_RC){
    message("qml FILES will complied to a .qrc FILE !")
    RESOURCES += $$GENERATED_RESOURCE_FILE  # qml文件编译到qrc文件
    #QMAKE_PRE_LINK += $(DEL_FILE) $$GENERATED_RESOURCE_FILE
} else {

}
#----------------------------------------------------------------

OTHER_FILES += $$qrclist # QML folder

RESOURCES += $$[QT_INSTALL_QML]/icons/icons_all.qrc

#- end - //////////////////////////////////////////////////////////////////////////////

