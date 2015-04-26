#-------------------------------------------------
#
# Project created by QtCreator 2015-03-21T22:01:48
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SzachyOnline
TEMPLATE = app


SOURCES += main.cpp\
    Figury/goniec.cpp \
    Figury/hetman.cpp \
    Figury/krol.cpp \
    Figury/pionek.cpp \
    Figury/skoczek.cpp \
    Figury/wieza.cpp \
    szachyapp.cpp \
    ModulRozgrywki/silnik.cpp \
    Figury/kreatorfigur.cpp \
    ModulRozgrywki/plansza.cpp \
    ModulLobby/dialogopcjelokalniegracz.cpp \
    ModulLobby/dialogopcjelokalniesi.cpp \
    ModulLobby/oknolobby.cpp \
    ModulLobby/oknologowania.cpp \
    ModulLobby/oknorejestracji.cpp \
    ModulRozgrywki/oknogry.cpp \
    ModulLobby/dialogotrzymanozaproszenie.cpp \
    ModulLobby/dialogczekajnaodpowiedz.cpp \
    ModulLobby/popupoczekiwanienaserwer.cpp \
    ModulLobby/komunikatorlobbyserwer.cpp \
    ModulKomunikacji/klient.cpp \
    Wiadomosci/wiadomoscloguj.cpp \
    Wiadomosci/wiadomoscrejestruj.cpp \
    Wiadomosci/wiadomoscwyloguj.cpp \
    ModulLobby/dialogwyslijzaproszenie.cpp \
    Wiadomosci/wiadomoscodpowiedznazaproszenie.cpp \
    Wiadomosci/wiadomosczaproszenie.cpp


HEADERS  += Figury/figura.h \
    Figury/goniec.h \
    Figury/hetman.h \
    Figury/krol.h \
    Figury/pionek.h \
    Figury/skoczek.h \
    Figury/wieza.h \
    szachyapp.h \
    ModulRozgrywki/silnik.h \
    Figury/kreatorfigur.h \
    ModulRozgrywki/plansza.h \
    opcje.h \
    uzytkownik.h \
    ModulLobby/dialogopcjelokalniegracz.h \
    ModulLobby/dialogopcjelokalniesi.h \
    ModulLobby/oknolobby.h \
    ModulLobby/oknologowania.h \
    ModulLobby/oknorejestracji.h \
    ModulRozgrywki/oknogry.h \
    ModulLobby/dialogotrzymanozaproszenie.h \
    ModulLobby/dialogczekajnaodpowiedz.h \
    ModulLobby/popupoczekiwanienaserwer.h \
    ModulLobby/komunikatorlobbyserwer.h \
    ModulLobby/obrotowylabel.h \
    ModulKomunikacji/klient.h \
    ModulKomunikacji/ikomunikator.h \
    Wiadomosci/wiadomosc.h \
    Wiadomosci/wiadomoscloguj.h \
    Wiadomosci/wiadomoscrejestruj.h \
    Wiadomosci/wiadomoscwyloguj.h \
    ModulLobby/dialogwyslijzaproszenie.h \
    Wiadomosci/wiadomoscodpowiedznazaproszenie.h \
    Wiadomosci/wiadomosczaproszenie.h

FORMS    += ModulLobby/oknorejestracji.ui \
    ModulLobby/oknologowania.ui \
    ModulRozgrywki/oknogry.ui \
    ModulLobby/oknolobby.ui \
    ModulLobby/dialogopcjelokalniesi.ui \
    ModulLobby/dialogopcjelokalniegracz.ui \
    ModulLobby/dialogotrzymanozaproszenie.ui \
    ModulLobby/dialogczekajnaodpowiedz.ui \
    ModulLobby/popupoczekiwanienaserwer.ui \
    ModulLobby/dialogwyslijzaproszenie.ui

