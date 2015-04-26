#include "wiadomoscloguj.h"
#include <QStringList>

WiadomoscLoguj::WiadomoscLoguj()
{
    czyDaneOk = false;
}

WiadomoscLoguj::~WiadomoscLoguj()
{

}

// format: login:login-haslo
QString WiadomoscLoguj::stworzWiadomosc()
{
    QString tresc;
    tresc = "login:";
    tresc.append(login);
    tresc.append("-");
    tresc.append(haslo);
    return tresc;
}

// format login:czyOk-nick-ranking-czyZalogowany
void WiadomoscLoguj::interpretujWiadomosc(QString &tresc)
{
    if( !tresc.startsWith("login:"))
    {
        // blad - powinna byc jakas bool w main class ze zle odebrano
        poprawnieOdebrane = false;
        return;
    }
    czyDaneOk = false;
    czy_zalogowany = true;
    tresc = tresc.mid(6);
    QStringList dane = tresc.split('-');
    if( dane[0] == "true" )
    {
        czyDaneOk = true;
        nick = dane[1];
        ranking = dane[2].toInt();
        if( dane[3] == "false" )
            czy_zalogowany = false;
    }
    else
        czyDaneOk = false;
    poprawnieOdebrane = true;
}

