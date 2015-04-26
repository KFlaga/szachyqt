#ifndef WIADOMOSCODPOWIEDZNAZAPROSZENIE_H
#define WIADOMOSCODPOWIEDZNAZAPROSZENIE_H

#include "wiadomosc.h"

class WiadomoscOdpowiedzNaZaproszenie : public Wiadomosc
{
public:
    WiadomoscOdpowiedzNaZaproszenie();
    ~WiadomoscOdpowiedzNaZaproszenie();

    QString stworzWiadomosc();
    void interpretujWiadomosc(QString& tresc);

public:
    bool czyZgoda;
};

#endif // WIADOMOSCODPOWIEDZNAZAPROSZENIE_H
