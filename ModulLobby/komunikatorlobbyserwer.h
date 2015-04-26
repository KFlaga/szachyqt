#ifndef KOMUNIKATORLOBBYSERWER_H
#define KOMUNIKATORLOBBYSERWER_H

#include <QWidget>
#include "popupoczekiwanienaserwer.h"
#include <QTimer>
#include "Wiadomosci/wiadomosc.h"
#include "ModulKomunikacji/ikomunikator.h"


class KomunikatorLobbySerwer : public IKomunikator
{
    Q_OBJECT
public:
    enum WynikWyslania { Powodzenie, PrzekroczonoCzas, Zajety, Anulowano, Niepowodzenie };

    explicit KomunikatorLobbySerwer(QWidget *parent = 0);
    ~KomunikatorLobbySerwer();

    WynikWyslania wyslijWiadomosc(Wiadomosc* msg);
    WynikWyslania wyslijWiadomoscZeZwrotem(Wiadomosc* msg);

    void ustawCzasOczekiwania(int ms) { timeout = ms; }

private slots:
    void przekroczonyCzasOczekiwania();
    void odebranoOdpowiedz();

signals:
    void nadajWiadomosc(QString*, IKomunikator*);

private:
    PopupOczekiwanieNaSerwer* oczekiwanie;
    QTimer* timer;
    int timeout;
    bool zajety;
    WynikWyslania status;
};

#endif // KOMUNIKATORLOBBYSERWER_H
