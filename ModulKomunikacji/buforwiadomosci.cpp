#include "buforwiadomosci.h"
#include <QThread>
#include <QApplication>

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
    while( !zatrzymaj )
    {
        if( bufor.size() > 0 )
            przetwarzajWiadomosci();
        while( wiadomosci.size() > 0 )
        {
            zakonczonoPrzetwarzanie = false;
            emit nowaWiadomosc(wiadomosci.first());
            while( !zakonczonoPrzetwarzanie ) // oczekiwanie na zakonczenie przetwarzania
            {
                qApp->processEvents(QEventLoop::AllEvents,100);
            }
            emit log("Zakonczono nadawanie - bufor");
            wiadomosci.removeFirst();
        }
        qApp->processEvents(QEventLoop::AllEvents,100);
    }
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
                emit log("Nowa wiadomosc: " + nowaWiad);
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

