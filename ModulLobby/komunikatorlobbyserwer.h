#ifndef KOMUNIKATORLOBBYSERWER_H
#define KOMUNIKATORLOBBYSERWER_H

#include <QWidget>
#include "popupoczekiwanienaserwer.h"
#include <QTimer>


class KomunikatorLobbySerwer : public QObject
{
    Q_OBJECT
public:
    enum WynikWyslania { Powodzenie, PrzekroczonoCzas, Zajety, Anulowano};

    explicit KomunikatorLobbySerwer(QWidget *parent = 0);
    ~KomunikatorLobbySerwer();

    // narazie oczywiscie nie wysylamy wiadomosci
    WynikWyslania wyslijWiadomosc();
    WynikWyslania wyslijWiadomoscZeZwrotem();
    int nasluchuj();

    void ustawCzasOczekiwania(int ms) { timeout = ms; }

private slots:
    void przekroczonyCzasOczekiwania();

public slots:
    void otrzymanoWiadomosc();

private:
    PopupOczekiwanieNaSerwer* oczekiwanie;
    QTimer* timer;
    QWidget* parent;
    int timeout;
    bool zajety;
    WynikWyslania status;
};

#endif // KOMUNIKATORLOBBYSERWER_H
