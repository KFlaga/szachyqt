#include "wiadomoscrejestruj.h"
#include <QStringList>

WiadomoscRejestruj::WiadomoscRejestruj()
{
    loginOK = false;
    hasloOK = false;
    nickOK = false;
}

WiadomoscRejestruj::~WiadomoscRejestruj()
{

}

QString WiadomoscRejestruj::stworzWiadomosc()
{
    QString tresc;
    tresc = "register:";
    tresc.append(login);
    tresc.append('-');
    tresc.append(haslo);
    tresc.append('-');
    tresc.append(nick);
    return tresc;
}

// format login:czyLoginOk-czyHasloOk-czyNickOk
void WiadomoscRejestruj::interpretujWiadomosc(QString &tresc)
{
    if( !tresc.startsWith("register:"))
    {
        // blad - powinna byc jakas bool w main class ze zle odebrano
        poprawnieOdebrane = false;
        return;
    }
    loginOK = false;
    hasloOK = false;
    nickOK = false;
    tresc = tresc.mid(9);
    QStringList dane = tresc.split('-');
    if( dane[0] == "true" )
        loginOK = true;
    if( dane[1] == "true" )
        hasloOK = true;
    if( dane[2] == "true" )
        nickOK = true;

    poprawnieOdebrane = true;

}
