#include "wiadomoscodpowiedznazaproszenie.h"

WiadomoscOdpowiedzNaZaproszenie::WiadomoscOdpowiedzNaZaproszenie()
{
    czyZgoda = false;
}

WiadomoscOdpowiedzNaZaproszenie::~WiadomoscOdpowiedzNaZaproszenie()
{

}

// format: zapro_zwrot:nickZapraszajacego-czyZgoda
QString WiadomoscOdpowiedzNaZaproszenie::stworzWiadomosc()
{
    QString tresc;
    tresc = "zapro_odp:";
    tresc.append(nick);
    tresc.append('-');
    tresc.append(czas);
    tresc.append('-');
    if( czyZgoda == true )
        tresc.append("true");
    else
        tresc.append("false");
    return tresc;
}

void WiadomoscOdpowiedzNaZaproszenie::interpretujWiadomosc(QString&)
{
    poprawnieOdebrane = true;
}
