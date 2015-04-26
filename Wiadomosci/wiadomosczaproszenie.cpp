#include "wiadomosczaproszenie.h"
#include <QStringList>

WiadomoscZaproszenie::WiadomoscZaproszenie()
{
    czyZaproszenieWyslane = false;
}

WiadomoscZaproszenie::~WiadomoscZaproszenie()
{

}

// format: invite:nick-czas
QString WiadomoscZaproszenie::stworzWiadomosc()
{
    QString tresc;
    tresc = "zaproszenie:";
    tresc.append(nick);
    tresc.append('-');
    tresc.append(QString::number(czas));
    return tresc;
}

// format: invite:czyWyslane-powodJakNie
void WiadomoscZaproszenie::interpretujWiadomosc(QString& tresc)
{
    if( !tresc.startsWith("zaproszenie:"))
    {
        poprawnieOdebrane = false;
        return;
    }
    czyZaproszenieWyslane = false;
    tresc = tresc.mid(7);
    QStringList dane = tresc.split('-');
    if( dane[0] == "true" )
    {
        czyZaproszenieWyslane = true;
    }
    else
    {
        powodNiepowodzenia = dane[1];
    }

    poprawnieOdebrane = true;
}
