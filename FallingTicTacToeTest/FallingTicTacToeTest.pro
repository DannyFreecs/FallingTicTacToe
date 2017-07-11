#-------------------------------------------------
#
# Project created by QtCreator 2017-05-14T10:28:38
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = tst_fallingtictactoetest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_fallingtictactoetest.cpp \
    game.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

HEADERS += \
    game.h \
    storage.h \
    storagemock.h
