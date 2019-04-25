#-------------------------------------------------
#
# Project created by QtCreator 2019-04-20T15:54:44
#
#-------------------------------------------------

QT       += core gui serialport

RC_ICONS += ./media/QPrint3D.ico

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = yk
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    gsim/GCodeInterpreter.cpp \
    gsim/GLDrawer.cpp \
    gsim/Helper.cpp \
    gsim/DockLayerList.cpp \
    gsim/DockOpenGL.cpp \
    gsim/GCodeInterpreter.cpp \
    gsim/GLDrawer.cpp \
    gsim/Helper.cpp

HEADERS += \
        mainwindow.h \
    gsim/GCodeInterpreter.h \
    gsim/GLDrawer.h \
    gsim/Helper.hpp \
    gsim/DockLayerList.h \
    gsim/DockOpenGL.h \
    gsim/GCodeInterpreter.h \
    gsim/GLDrawer.h \
    gsim/Helper.hpp

FORMS += \
        mainwindow.ui
