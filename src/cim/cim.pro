#-------------------------------------------------
#
# Project created by QtCreator 2016-03-31T17:50:25
#
#-------------------------------------------------

QT       -= gui

TEMPLATE = lib

DEFINES += CIM_LIBRARY

SOURCES += cim.cpp

HEADERS += cim.h\
        cim_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

# Use Precompiled headers (PCH)
PRECOMPILED_HEADER += cim_global.h

SDKInclude = $$PWD/../../sdk/include

INCLUDEPATH += $$SDKInclude \
        $$SDKInclude/metalib/corelib \
        $$SDKInclude/metalib/corelib/private

#include(core.pri)


# SDK Deploy -------------------------------------------------------------------------------------------
# 作用
#
# a. 将头文件发布到SDK目录下

h_searchlist += *.h
for(searchvar, h_searchlist) {
    hlist += $$files($$searchvar, true)
}
#message($$hlist"(hlist)")
for (hvar, hlist) {
        _desDirpath = $$SDKInclude/$$TARGET             # 目标头文件根目录
        _srcFilepath = $$absolute_path($$hvar)          # 源头文件绝对路径
        _desReleFilepath = $$relative_path($$_srcFilepath, $$_desDirpath)           # sdk 头文件引用路径
        _srcReleFilepath = $$relative_path($$_srcFilepath, $$_PRO_FILE_PWD_)        # 源头文件相对于项目的相对路径
        _desFilepath = $$_desDirpath/$$_srcReleFilepath # 目标头文件的绝对路径

        SDK_H_FILE_CONTENT = $$LITERAL_HASH"include \"../$$_desReleFilepath\""
        write_file($$_desFilepath, SDK_H_FILE_CONTENT)

#        message($$SDK_H_FILE_CONTENT)
}

# \end SDK Deploy -------------------------------------------------------------------------------------------



# COMMON -------------------------------------------------------------------------------------------
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
DEFINES     +=

CONFIG+=debug_and_release
CONFIG(debug, debug|release) {
#    TARGET = $${TARGET}d
    OBJECTS_DIR += $$TEMP_DIR/debug/obj

    DEFINES += _DEBUG
    LIBS += -L$${DESTDIR} -lmetalibd

} else {
#    TARGET = $${TARGET}
    OBJECTS_DIR += $$TEMP_DIR/release/obj

    LIBS += -L$${DESTDIR} -lmetalib
}

# \end COMMON -------------------------------------------------------------------------------------------
