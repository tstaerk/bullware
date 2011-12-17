#-------------------------------------------------
#
# Project created by QtCreator 2011-02-02T16:03:56
#
#-------------------------------------------------

QT       += core gui sql

TARGET = bullware
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

icons.path = /usr/share/icons
icons.files += bullware.svg
desktop.path = /usr/share/applications
desktop.files += bullware.desktop
target.path = $$[QT_INSTALL_BINS]
INSTALLS += target desktop icons
