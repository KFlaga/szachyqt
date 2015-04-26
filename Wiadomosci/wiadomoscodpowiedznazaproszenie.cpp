#include "wiadomoscodpowiedznazaproszenie.h"

WiadomoscOdpowiedzNaZaproszenie::WiadomoscOdpowiedzNaZaproszenie()
{
    czyZgoda = false;
}

WiadomoscOdpowiedzNaZaproszenie::~WiadomoscOdpowiedzNaZaproszenie()
{

}

// format: zapro_zwrot:
QString WiadomoscOdpowiedzNaZaproszenie::stworzWiadomosc()
{
    QString tresc;
    tresc = "zapro_zwrot:";
    return tresc;
}

// format: zapro_zwrot:czyZgoda
void WiadomoscOdpowiedzNaZaproszenie::interpretujWiadomosc(QString& tresc)
{
    if( !tresc.startsWith("zapro_zwrot:"))
    {
        poprawnieOdebrane = false;
        return;
    }
    czyZgoda = false;
    tresc = tresc.mid(12);
    if( tresc == "true" )
    {
        czyZgoda = true;
    }

    poprawnieOdebrane = true;
}
