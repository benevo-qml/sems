#-------------------------------------------------
#
# Project created by QtCreator 2016-03-20T21:16:05
#
#-------------------------------------------------

QT       += core sql

QT       -= gui

TEMPLATE = lib

DEFINES += PARSE_LIBRARY #USE_METALIB_LIB

SOURCES += parse.cpp

HEADERS += parse.h\
        parse_global.h

DEFINES += MARKUP_STL UNICODE
win32 {
    DEFINES +=
}

unix {
    DEFINES += MARKUP_STDCONV
    target.path = /usr/lib
    INSTALLS += target
}

include(core/core.pri)

#PRECOMPILED_HEADER += corelib/meta_macro.h

SDKInclude = $$PWD/../../sdk/include


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
        _desDirpath = $$SDKInclude/$$TARGET                 # 目标头文件根目录
        _srcFilepath = $$absolute_path($$hvar)              # 源头文件绝对路径
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

TARGET = $$qtLibraryTarget($$TARGET) # Attension: should post the back

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
    OBJECTS_DIR += $$TEMP_DIR/debug/obj
} else {
    OBJECTS_DIR += $$TEMP_DIR/release/obj
}

contains(DEFINES, USE_METALIB_LIB){
    message("set depended lib: metalib !")

    CONFIG(debug, debug|release) {
        LIBS += -L$${DESTDIR} -lmetalibd
    }else{
        LIBS += -L$${DESTDIR} -lmetalib
    }
} else {
}

# \end COMMON -------------------------------------------------------------------------------------------
