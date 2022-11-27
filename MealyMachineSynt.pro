TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        QuineMcCluskey/dnf.cpp \
        QuineMcCluskey/impl.cpp \
        main.cpp \
        structsynthesis.cpp \
        table.cpp \
        tableitem.cpp

HEADERS += \
    QuineMcCluskey/dnf.h \
    QuineMcCluskey/impl.h \
    structsynthesis.h \
    table.h \
    tableitem.h
