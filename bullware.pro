#-------------------------------------------------
#
# Project created by QtCreator 2011-02-02T16:03:56
#
#-------------------------------------------------

QT       += core gui

TARGET = bullware
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

desktop.path = /usr/share/applications
desktop.files += bullware.desktop
target.path = $$[QT_INSTALL_BINS]
INSTALLS += target desktop
