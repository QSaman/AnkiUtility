#-------------------------------------------------
#
# Project created by QtCreator 2012-06-22T16:55:24
#
#-------------------------------------------------

QT       += core gui

TARGET = AnkiUtility
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    dictionarytextedit.cpp \
    xmlbasedsettings.cpp \
    htmlmodfier.cpp

HEADERS  += mainwindow.h \
    dictionarytextedit.h \
    xmlbasedsettings.h \
    htmlmodfier.h

FORMS    += mainwindow.ui

RESOURCES += \
    resources.qrc
CONFIG   += console
