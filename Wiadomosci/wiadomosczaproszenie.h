#ifndef WIADOMOSCZAPROSZENIE_H
#define WIADOMOSCZAPROSZENIE_H

#include "wiadomosc.h"

// W przypadku wysylania zaproszenia, to mam pomysl taki:
// - wysylamy wiadomosc na serwer, ze zapraszamy gracza
// - po naszej stronie nastepuje oczekiwanie na odpowiedz
// - z serwera wysylane jest zapytanie do 2 gracza - tak wiec
//  nalezy dorobic mechanizm niezaleznego odbierania wiadomosci
//  przez aplikacje, nie przez komunikatory - np. wykorzystujac
//  inny typ wiadomosci ( 'systemowe' )
// - na serwerze sprawdzany jest takze status gracza - jak jest
//  niezalogowany / niestnieje / gra itd. to oczywiscie z automatu wraca
//  odpowiednia odpowiedz
// - graczowi 2 pojawia sie okienko, i oczywiscie w odpowiedzi do
//  serwera wysylana jest wiadomosc
// - z serwera wysylana jest odpowiedz do gracza 1
//
// - i tu niby inicjalizacja samej gry powinna sie znalezc
//  ale zostaje kwestia wyboru np. czasu gry, choć to moze
//  byc wybierane przez gracza 1 razem z zaproszeniem
//
class WiadomoscZaproszenie : public Wiadomosc
{
public:
    WiadomoscZaproszenie();
    ~WiadomoscZaproszenie();

    QString stworzWiadomosc();
    void interpretujWiadomosc(QString& tresc);

public:
    QString nick;
    int czas;
    bool czyZaproszenieWyslane;
    QString powodNiepowodzenia; // np. zajety gracz - moze byc tez w formie enuma
};

#endif // WIADOMOSCZAPROSZENIE_H
