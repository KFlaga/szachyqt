#ifndef OKNOLOBBY_H
#define OKNOLOBBY_H

#include <QMainWindow>
#include "uzytkownik.h"
#include "opcje.h"
#include "komunikatorlobbyserwer.h"
#include "ModulKomunikacji/klient.h"
#include "Wiadomosci/wiadomosc.h"

namespace Ui {
class OknoLobby;
}

class OknoLobby : public QMainWindow
{
    Q_OBJECT

public:
    explicit OknoLobby(QWidget *parent = 0);
    ~OknoLobby();

    void ustawUzytkownika(Uzytkownik* u)
    {
        biezacyUzytkownik = u;
    }

    void podlaczLacze(Klient* lacze)
    {
        connect(komunikator, SIGNAL(nadajWiadomosc(QString*,IKomunikator*)),
                lacze, SLOT(wysliWiadomosc(QString*,IKomunikator*)));
    }

private:
    void aktualizujInterfejs();
    void wyslijWiadomosc(Wiadomosc* wiadomosc);
    void otrzymanoZaproszenie(Uzytkownik*);

private slots:
    void wyloguj();
    void zagrajLokalnieGracz();
    void zagrajLokalnieSI();
    void zaprosGracza();
    void szukajGracza();
    void zaloguj();
    void zalogowano();
    void closeEvent(QCloseEvent *);

signals:
    void sygZalogowano();
    void graLokalnie(Opcje*);

private:
    Ui::OknoLobby *ui;
    Uzytkownik* biezacyUzytkownik;
    bool czy_zalogowano;
    KomunikatorLobbySerwer* komunikator;
};

#endif // OKNOLOBBY_H
