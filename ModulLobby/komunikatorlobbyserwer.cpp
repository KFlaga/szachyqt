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
    status = Powodzenie;

    if(zajety)
        return Zajety;

    ustawWiadomosc(msg);
    emit nadajWiadomosc(stworzWiadomosc(), (IKomunikator*)this);

    return status;
}

void KomunikatorLobbySerwer::przekroczonyCzasOczekiwania()
{
    status = PrzekroczonoCzas;
    oczekiwanie->accept();
}

void KomunikatorLobbySerwer::odebranoOdpowiedz()
{
    status = Powodzenie;
    oczekiwanie->accept();
}

KomunikatorLobbySerwer::WynikWyslania KomunikatorLobbySerwer::wyslijWiadomoscZeZwrotem(Wiadomosc *msg,
                                                                                       QString popupTekst)
{
    if(popupTekst != "")
        oczekiwanie->ustawTekst(popupTekst);

    ustawCzyZeZwrotem(true);
    wyslijWiadomosc(msg);
    status = Niepowodzenie;

    zajety = true;
    timer->setSingleShot(true);
    timer->setInterval(timeout);
    connect(timer,SIGNAL(timeout()), this, SLOT(przekroczonyCzasOczekiwania()));
    timer->start();

    int res = oczekiwanie->exec();
    zajety = false;

    if( res == QDialog::Rejected )
        status = Anulowano;

    anuluj();
    oczekiwanie->ustawTekst();
    ustawCzyZeZwrotem(false);

    return status;
}
