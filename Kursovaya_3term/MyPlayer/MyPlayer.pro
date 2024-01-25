#-------------------------------------------------
#
# Project created by QtCreator 2016-06-29T11:25:56
#
#-------------------------------------------------

QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Player
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp

HEADERS  += widget.h \
    exception.h \
    fileexception.h \
    shablon.h \
    shablon.h

FORMS    += widget.ui

RESOURCES += \
    buttons.qrc

DISTFILES +=

