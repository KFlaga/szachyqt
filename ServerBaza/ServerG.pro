#-------------------------------------------------
#
# Project created by QtCreator 2015-04-15T11:01:43
#
#-------------------------------------------------

QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ServerG
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    pojedynek.cpp \
    buforwiadomosci.cpp

HEADERS  += mainwindow.h \
    pojedynek.h \
    uzytkownik.h \
    buforwiadomosci.h

FORMS    += mainwindow.ui
