#-------------------------------------------------
#
# Project created by QtCreator 2019-07-12T08:36:55
#
#-------------------------------------------------

QT       += core gui serialport  opengl openglwidgets
QMAKE_CXX               = /usr/bin/g++
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = QPrint3dNew
TEMPLATE = app

#CONFIG += c++17
# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
#DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    gcodeinterpreter.cpp \
    helper.cpp \
    glwidget.cpp \
    serialthread.cpp

HEADERS += \
        mainwindow.h \
    gcodeinterpreter.h \
    helper.h \
    glwidget.h \
    serialthread.h

FORMS += \
    mainwindow.ui

RESOURCES += \
    textures.qrc
