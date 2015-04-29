#include "wiadomoscwyloguj.h"

WiadomoscWyloguj::WiadomoscWyloguj()
{

}

WiadomoscWyloguj::~WiadomoscWyloguj()
{

}

QString WiadomoscWyloguj::stworzWiadomosc()
{
    QString tresc = "logout:";
    return tresc;
}

void WiadomoscWyloguj::interpretujWiadomosc(QString &tresc)
{
    poprawnieOdebrane = true;
}

