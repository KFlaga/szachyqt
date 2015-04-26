#ifndef WIADOMOSCLOGUJ_H
#define WIADOMOSCLOGUJ_H

#include "wiadomosc.h"

class WiadomoscLoguj : public Wiadomosc
{
public:
    WiadomoscLoguj();
    ~WiadomoscLoguj();

    QString stworzWiadomosc();
    void interpretujWiadomosc(QString& tresc);

public:
    QString login;
    QString haslo;
    bool czyDaneOk;
    QString nick;
    int ranking;
    bool czy_zalogowany;
    int status;
};

#endif // WIADOMOSCLOGUJ_H
