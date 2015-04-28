#ifndef WIADOMOSC_H
#define WIADOMOSC_H

#include <QString>
#include <QObject>

class Wiadomosc
{
public:
    Wiadomosc() { }

    // Format wiadomosci:
    // operacja:dane-dane-dane
    virtual QString stworzWiadomosc()
    {
        return "Pusta";
    }

    virtual void interpretujWiadomosc(QString&)
    {

    }

    bool czyPoprawnieOdebrane()
    {
        return poprawnieOdebrane;
    }

protected:
    bool poprawnieOdebrane;
};

#endif // WIADOMOSC_H
