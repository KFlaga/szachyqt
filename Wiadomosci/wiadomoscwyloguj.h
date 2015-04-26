#ifndef WIADOMOSCWYLOGUJ_H
#define WIADOMOSCWYLOGUJ_H

#include "wiadomosc.h"

class WiadomoscWyloguj : public Wiadomosc
{
public:
    WiadomoscWyloguj();
    ~WiadomoscWyloguj();

    QString stworzWiadomosc();
    void interpretujWiadomosc(QString& tresc);
};

#endif // WIADOMOSCWYLOGUJ_H
