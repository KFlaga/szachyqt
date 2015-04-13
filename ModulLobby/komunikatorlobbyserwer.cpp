#include "komunikatorlobbyserwer.h"

KomunikatorLobbySerwer::KomunikatorLobbySerwer(QWidget *_parent) : QObject((QObject*)_parent)
{
    parent = _parent;
    oczekiwanie  = new PopupOczekiwanieNaSerwer(parent);
    oczekiwanie->setWindowModality(Qt::WindowModal);
    timer = new QTimer(this);
    timeout = 5000;
    zajety = false;
}

KomunikatorLobbySerwer::~KomunikatorLobbySerwer()
{

}

KomunikatorLobbySerwer::WynikWyslania KomunikatorLobbySerwer::wyslijWiadomosc()
{
    if(zajety)
        return WynikWyslania::Zajety;

    zajety = true;
    timer->setSingleShot(true);
    timer->setInterval(timeout);
    connect(timer,SIGNAL(timeout()), this, SLOT(przekroczonyCzasOczekiwania()));
    timer->start();

    int res = oczekiwanie->exec();
    zajety = false;

    if( res == QDialog::Rejected )
        status = WynikWyslania::Anulowano;

    return status;
}

void KomunikatorLobbySerwer::przekroczonyCzasOczekiwania()
{
    status = WynikWyslania::PrzekroczonoCzas;
    oczekiwanie->accept();
}

void KomunikatorLobbySerwer::otrzymanoWiadomosc()
{
    status = WynikWyslania::Powodzenie;
    oczekiwanie->accept();
}

KomunikatorLobbySerwer::WynikWyslania KomunikatorLobbySerwer::wyslijWiadomoscZeZwrotem()
{
   return wyslijWiadomosc();
   // po wyslaniu bedzie jeszcze czekalo na odpowiedz
}
