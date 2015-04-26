#include "komunikatorlobbyserwer.h"

KomunikatorLobbySerwer::KomunikatorLobbySerwer(QWidget *parent) : IKomunikator((QObject*)parent)
{
    oczekiwanie  = new PopupOczekiwanieNaSerwer(parent);
    oczekiwanie->setWindowModality(Qt::WindowModal);
    timer = new QTimer(this);
    timeout = 5000;
    zajety = false;
    connect(this, SIGNAL(odebranoWiadomosc()),
            this, SLOT(odebranoOdpowiedz()));
}

KomunikatorLobbySerwer::~KomunikatorLobbySerwer()
{
    delete timer;
    delete oczekiwanie;
}

KomunikatorLobbySerwer::WynikWyslania KomunikatorLobbySerwer::wyslijWiadomosc(Wiadomosc* msg)
{
    status = WynikWyslania::Powodzenie;

    if(zajety)
        return WynikWyslania::Zajety;

    ustawWiadomosc(msg);
    emit nadajWiadomosc(stworzWiadomosc(), (IKomunikator*)this);

    return status;
}

void KomunikatorLobbySerwer::przekroczonyCzasOczekiwania()
{
    status = WynikWyslania::PrzekroczonoCzas;
    oczekiwanie->accept();
}

void KomunikatorLobbySerwer::odebranoOdpowiedz()
{
    status = WynikWyslania::Powodzenie;
    oczekiwanie->accept();
}

KomunikatorLobbySerwer::WynikWyslania KomunikatorLobbySerwer::wyslijWiadomoscZeZwrotem(Wiadomosc *msg,
                                                                                       QString popupTekst)
{
    if(popupTekst != "")
        oczekiwanie->ustawTekst(popupTekst);

    wyslijWiadomosc(msg);
    status = WynikWyslania::Niepowodzenie;

    zajety = true;
    timer->setSingleShot(true);
    timer->setInterval(timeout);
    connect(timer,SIGNAL(timeout()), this, SLOT(przekroczonyCzasOczekiwania()));
    timer->start();

    int res = oczekiwanie->exec();
    zajety = false;

    if( res == QDialog::Rejected )
        status = WynikWyslania::Anulowano;

    anuluj();
    oczekiwanie->ustawTekst();

    return status;
}
