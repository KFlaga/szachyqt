#ifndef WIADOMOSCREJESTRUJ_H
#define WIADOMOSCREJESTRUJ_H

#include "wiadomosc.h"

class WiadomoscRejestruj : public Wiadomosc
{
public:
    WiadomoscRejestruj();
    ~WiadomoscRejestruj();

    QString stworzWiadomosc();
    void interpretujWiadomosc(QString& tresc);

public:
    QString login;
    QString haslo;
    QString nick;
    bool loginOK;
    bool hasloOK;
    bool nickOK;
};

#endif // WIADOMOSCREJESTRUJ_H
