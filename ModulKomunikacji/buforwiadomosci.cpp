#include "buforwiadomosci.h"
#include <QThread>
#include <QTimer>

BuforWiadomosci::BuforWiadomosci(char delim)
{
    przerwa = delim;
    zatrzymaj = true;
}

BuforWiadomosci::~BuforWiadomosci()
{

}

void BuforWiadomosci::dodajDane(const QByteArray &dane)
{
    bufor.append(dane);
}

void BuforWiadomosci::start()
{
    zatrzymaj = false;
    sprawdzBufor();
}

void BuforWiadomosci::sprawdzBufor()
{
    if( !zatrzymaj )
    {
        if( bufor.size() > 0 )
            przetwarzajWiadomosci();
        sprawdzWiadomosci();
    }
}

void BuforWiadomosci::sprawdzCzyKoniec()
{
    if( !zakonczonoPrzetwarzanie ) // oczekiwanie na zakonczenie przetwarzania
    {
        QTimer::singleShot(50, this, SLOT(sprawdzCzyKoniec()));
        return;
    }
    emit log("Zakonczono nadawanie - bufor");
    if( !wiadomosci.empty() )
     wiadomosci.removeFirst();
    QTimer::singleShot(50, this, SLOT(sprawdzWiadomosci()));
}

void BuforWiadomosci::sprawdzWiadomosci()
{
    if( wiadomosci.size() > 0 )
    {
        zakonczonoPrzetwarzanie = false;
        QTimer::singleShot(10, this, SLOT(sprawdzCzyKoniec()));
        emit nowaWiadomosc(wiadomosci.first());
        return;
    }
    QTimer::singleShot(100, this, SLOT(sprawdzBufor()));
}

void BuforWiadomosci::stop()
{
    zatrzymaj = true;
}

void BuforWiadomosci::przetwarzajWiadomosci()
{
    emit log("Przetwarzanie wiadomosci. Liczba danych: " + QString::number(bufor.size()));
    QList<QByteArray>::iterator it = bufor.begin();
    while(it != bufor.end())
    {
        przetwarzanaWiadomosc.append(*it);
        it = bufor.erase(it);
    }

    bool jestWiadomosc;
    do
    {
        jestWiadomosc = false;
        QString nowaWiad = "";
        for( int i = 0; i < przetwarzanaWiadomosc.size(); i++)
        {
            QChar c = przetwarzanaWiadomosc.at(i);
            if( c == '.' ) // koniec wiadomosci
            {
                jestWiadomosc = true;
                wiadomosci.append(nowaWiad);
                przetwarzanaWiadomosc = przetwarzanaWiadomosc.mid(i+1);
                break;
            }
            nowaWiad.append(c);
        }
    }
    while( jestWiadomosc );
}

void BuforWiadomosci::nadajKolejnaWiadomosc()
{
    zakonczonoPrzetwarzanie = true;
}

