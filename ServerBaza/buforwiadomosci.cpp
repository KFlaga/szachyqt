#include "buforwiadomosci.h"
#include <QThread>
#include <QTimer>

BuforWiadomosci::BuforWiadomosci(QTcpSocket* klient, char delim)
{
    socket = klient;
    przerwa = delim;
    zatrzymaj = true;
}

BuforWiadomosci::~BuforWiadomosci()
{

}

void BuforWiadomosci::wczytajDane()
{
    bufor.append(socket->readAll());
}

void BuforWiadomosci::start()
{
    zatrzymaj = false;
    QTimer::singleShot(100, Qt::CoarseTimer, this, SLOT(sprawdzBufor()));
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

void BuforWiadomosci::sprawdzWiadomosci()
{
    while( wiadomosci.size() > 0 )
    {
        QString wiad = wiadomosci.first();
        wiadomosci.removeFirst();
        emit nowaWiadomosc(wiad, socket);
    }
    QTimer::singleShot(100, Qt::CoarseTimer, this, SLOT(sprawdzBufor()));
}

void BuforWiadomosci::stop()
{
    zatrzymaj = true;
}

void BuforWiadomosci::przetwarzajWiadomosci()
{
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


