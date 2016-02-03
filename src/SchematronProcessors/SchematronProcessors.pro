#-------------------------------------------------
#
# Project created by QtCreator 2016-01-29T14:36:10
#
#-------------------------------------------------

QT       += xml axcontainer

QT       -= gui widgets

CONFIG  += c++11
CONFIG += axcontainer

TARGET = SchematronProcessors
TEMPLATE = lib

DEFINES += SCHEMATRONPROCESSORS_LIBRARY

SOURCES += schematronprocessors.cpp \
    javasaxonprocessor.cpp \
    libxmlxsltprocessor.cpp \
    msxmlxsltprocessor.cpp \
    xsltprocessor.cpp \
    schematronprocessor.cpp

HEADERS += schematronprocessors.h\
        schematronprocessors_global.h \
    javasaxonprocessor.h \
    jnienv.h \
    libxmlxsltprocessor.h \
    msxmlxsltprocessor.h \
    xsltprocessor.h \
    schematronprocessor.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

LIBS += -L../SchematronProcessors/bin
LIBS += -L../SchematronProcessors/libs/libxml2-2.7.8.win32/lib -llibxml2 -Llibs/iconv/lib -L../SchematronProcessors/libs/libxslt-1.1.26.win32/lib -llibxslt
INCLUDEPATH += libs/libxml2-2.7.8.win32/include
INCLUDEPATH += libs/iconv/include
INCLUDEPATH += libs/libxslt-1.1.26.win32/include

INCLUDEPATH += "libs/java1.8.0.66/include"
INCLUDEPATH += "libs/java1.8.0.66/include/win32"

LIBS += -L"C:/Program Files (x86)/Java/jdk1.8.0_66/lib" -L"C:\Program Files (x86)\Java\jdk1.8.0_66\jre\bin\server" -ljvm  -ljawt
