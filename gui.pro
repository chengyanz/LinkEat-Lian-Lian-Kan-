#-------------------------------------------------
#
# Project created by QtCreator 2016-11-19T22:17:59
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = gui
TEMPLATE = app


SOURCES += main.cpp\
        gui.cpp \
    image.cpp \
    location.cpp \
    game.cpp \
    board.cpp

HEADERS  += gui.h \
    image.h \
    location.h \
    game.h \
    picture.h \
    board.h

FORMS    += gui.ui

RESOURCES += \
    resources.qrc
