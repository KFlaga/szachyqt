#ifndef KOMUNIKATORLOBBYSERWER_H
#define KOMUNIKATORLOBBYSERWER_H

#include <QWidget>
#include <QTimer>
#include "Wiadomosci/wiadomosc.h"
#include "ModulKomunikacji/ikomunikator.h"


class KomunikatorLobbySerwer : public QObject
{
    Q_OBJECT
public:
    explicit KomunikatorLobbySerwer(QWidget *parent = 0);
    ~KomunikatorLobbySerwer();

    void wyslijWiadomosc(Wiadomosc* msg, bool czyZwrot = false, int timeout = 5000);

private slots:
    void przekroczonyCzasOczekiwania();
    void odebranoOdpowiedz();

signals:
    void nadajWiadomosc(QString, IKomunikator*);
    void odebranoZwrot(Wiadomosc*, bool);

private:
    QTimer* timer;
    bool czyOdebranoOdpowiedz;
    bool czyPrzekroczonoCzas;
};

#endif // KOMUNIKATORLOBBYSERWER_H
