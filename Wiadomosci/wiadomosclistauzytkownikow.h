#ifndef WIADOMOSCLISTAUZYTKOWNIKOW_H
#define WIADOMOSCLISTAUZYTKOWNIKOW_H

#include "wiadomosc.h"
#include <QVector>
#include "uzytkownik.h"

class WiadomoscListaUzytkownikow : public Wiadomosc
{
public:
    WiadomoscListaUzytkownikow();
    ~WiadomoscListaUzytkownikow();

    QString stworzWiadomosc();
    void interpretujWiadomosc(QString& tresc);

public:
    QVector<Uzytkownik> uzytkownicy;
};

#endif // WIADOMOSCLISTAUZYTKOWNIKOW_H
