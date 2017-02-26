# Qt tools module

intel_icc: QMAKE_CXXFLAGS += -fp-model strict

HEADERS +=  \
        $$PWD/logme.h


SOURCES += \
        $$PWD/logme.cpp
