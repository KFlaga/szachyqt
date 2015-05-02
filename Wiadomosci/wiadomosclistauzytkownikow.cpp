#include "wiadomosclistauzytkownikow.h"
#include <QStringList>

WiadomoscListaUzytkownikow::WiadomoscListaUzytkownikow()
{

}

WiadomoscListaUzytkownikow::~WiadomoscListaUzytkownikow()
{

}

// format: zadajlisty:
QString WiadomoscListaUzytkownikow::stworzWiadomosc()
{
    QString tresc = "zadajlisty:";
    return tresc;
}

// format: invite:czyWyslane-powodJakNie
void WiadomoscListaUzytkownikow::interpretujWiadomosc(QString& tresc)
{
    if( !tresc.startsWith("listazwrot:"))
    {
        poprawnieOdebrane = false;
        return;
    }

    tresc = tresc.mid(11);
    QStringList dane = tresc.split('-');
    int liczbaUz = dane[0].toInt();

    if( dane.size() != 3*liczbaUz + 1)
    {
        poprawnieOdebrane = false;
        return;
    }

    uzytkownicy = QVector<Uzytkownik>();
    uzytkownicy.reserve(liczbaUz);

    for(int i = 1; i <= liczbaUz; i++)
    {
        Uzytkownik u;
        u.nick = dane[3*i - 2];
        u.ranking = dane[3*i - 1].toInt();
       // u.status = dane[3*i];
        uzytkownicy.push_back(u);
    }

    poprawnieOdebrane = true;
}
