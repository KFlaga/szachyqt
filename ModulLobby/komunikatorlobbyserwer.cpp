#include "komunikatorlobbyserwer.h"
#include <QApplication>
#include "oknolobby.h"

KomunikatorLobbySerwer::KomunikatorLobbySerwer(QWidget* parent) : QObject(parent)
{
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(przekroczonyCzasOczekiwania()));
}

KomunikatorLobbySerwer::~KomunikatorLobbySerwer()
{
    delete timer;
}

void KomunikatorLobbySerwer::przekroczonyCzasOczekiwania()
{
    czyPrzekroczonoCzas = true;
    czyOdebranoOdpowiedz = true;
}

void KomunikatorLobbySerwer::odebranoOdpowiedz()
{
    czyOdebranoOdpowiedz = true;
}

void KomunikatorLobbySerwer::wyslijWiadomosc(Wiadomosc *msg, bool czyZwrot, int timeout)
{
    czyOdebranoOdpowiedz = false;
    czyPrzekroczonoCzas = false;

    IKomunikator* skrzynka = new IKomunikator(this);
    connect(skrzynka, SIGNAL(odebranoWiadomosc()),
            this, SLOT(odebranoOdpowiedz()));
    skrzynka->ustawCzyZeZwrotem(czyZwrot);

    skrzynka->ustawWiadomosc(msg);
    emit nadajWiadomosc(skrzynka->stworzWiadomosc(), skrzynka);

    if( czyZwrot == true )
    {
        timer->setSingleShot(true);
        timer->setInterval(timeout);
        timer->start();

        while( !czyOdebranoOdpowiedz )
        {
            qApp->processEvents(QEventLoop::AllEvents,50);
        }

        emit odebranoZwrot(msg, czyPrzekroczonoCzas);
    }

    skrzynka->deleteLater();
}
