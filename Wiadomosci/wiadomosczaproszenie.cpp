#include "wiadomosczaproszenie.h"

WiadomoscZaproszenie::WiadomoscZaproszenie()
{

}

WiadomoscZaproszenie::~WiadomoscZaproszenie()
{

}

// format: invite:nick
QString WiadomoscZaproszenie::stworzWiadomosc()
{
    QString tresc;
    tresc = "invite:";
    tresc.append(nick);
    return tresc;
}

// format: invite:czyWyslane-powod-czyZgoda
void WiadomoscZaproszenie::interpretujWiadomosc(QString& tresc)
{
    if( !tresc.startsWith("invite:"))
    {
        poprawnieOdebrane = false;
        return;
    }
    czyZaproszenieWyslane = false;
    czyZgoda = false;
    tresc = tresc.mid(7);
    QStringList dane = tresc.split('-');
    if( dane[0] == "true" )
    {
        czyWyslane = true;
        if(dane[2] == "true" )
            czyZgoda = true;
    }
    else
    {
        powodNiepowodzenia = dane[1];
    }

    poprawnieOdebrane = true;
}
