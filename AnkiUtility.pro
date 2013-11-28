#-------------------------------------------------
#
# Project created by QtCreator 2012-06-22T16:55:24
#
#-------------------------------------------------

QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AnkiUtility
TEMPLATE = app


SOURCES += src/core/main.cpp\
        src/forms/src/mainwindow.cpp \
    src/widgets/dictionarytextedit.cpp \
    src/core/xmlbasedsettings.cpp \
    src/core/htmlmodfier.cpp

HEADERS  += src/forms/src/mainwindow.h \
    src/widgets/dictionarytextedit.h \
    src/core/xmlbasedsettings.h \
    src/core/htmlmodfier.h

FORMS    += src/forms/ui/mainwindow.ui

RESOURCES += \
    resources.qrc
#CONFIG   += console
