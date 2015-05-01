#ifndef IKOMUNIKATOR_H
#define IKOMUNIKATOR_H

#include <QObject>
#include "Wiadomosci/wiadomosc.h"

static int IKOM_noweID = 200;

class IKomunikator : public QObject
{
    Q_OBJECT
public:
    IKomunikator(QObject* parent = 0) : QObject(parent)
    {
        ID = IKOM_noweID++;
        anulowano = false;
        zwrot = false;
    }

    int wezID()
    {
        return ID;
    }

    bool zeZwrotem()
    {
        return zwrot;
    }

    void ustawCzyZeZwrotem(bool czyZwrot)
    {
        zwrot = czyZwrot;
    }

public slots:
    void ustawWiadomosc(Wiadomosc* wiad)
    {
        wiadomosc = wiad;
    }

    Wiadomosc* wezWiadomosc()
    {
        return wiadomosc;
    }

    void odbierzWiadomosc(QString* tresc)
    {
        if( anulowano )
            return;
        wiadomosc->interpretujWiadomosc(*tresc);
        emit odebranoWiadomosc();
    }

    QString& stworzWiadomosc()
    {
        anulowano = false;
        dane = wiadomosc->stworzWiadomosc();
        return dane;
    }

    void anuluj() { anulowano = true; }

signals:
    void odebranoWiadomosc();

private:
    Wiadomosc* wiadomosc;
    QString dane;
    bool anulowano;
    bool zwrot;
    int ID;
};

#endif // IKOMUNIKATOR_H
