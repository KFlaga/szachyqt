#-------------------------------------------------
#
# Project created by QtCreator 2015-03-21T22:01:48
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SzachyOnline
TEMPLATE = app


SOURCES += main.cpp\
        oknologowania.cpp \
    oknoopcji.cpp \
    Figury/goniec.cpp \
    Figury/hetman.cpp \
    Figury/krol.cpp \
    Figury/pionek.cpp \
    Figury/skoczek.cpp \
    Figury/wieza.cpp \
    oknogry.cpp \
    szachyapp.cpp \
    ModulRozgrywki/silnik.cpp \
    Figury/kreatorfigur.cpp \
    ModulRozgrywki/plansza.cpp \
    Figury/figura.cpp

HEADERS  += oknologowania.h \
    oknoopcji.h \
    Figury/figura.h \
    Figury/goniec.h \
    Figury/hetman.h \
    Figury/krol.h \
    Figury/pionek.h \
    Figury/skoczek.h \
    Figury/wieza.h \
    oknogry.h \
    szachyapp.h \
    ModulRozgrywki/silnik.h \
    Figury/kreatorfigur.h \
    ModulRozgrywki/plansza.h \
    opcje.h

FORMS    += oknologowania.ui \
    oknoopcji.ui \
    oknogry.ui
