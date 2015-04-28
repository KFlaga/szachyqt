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

    void podlaczLacze(Klient* lacze);

    void ustawStatus(const QString &status, int czas);

public slots:
    void poloczonoZSerwerem();
    void rozloczonoZSerwerem();
    void nieMoznaPolaczycZSerwerem(int);

private:
    void aktualizujInterfejs();
    void wyslijWiadomosc(Wiadomosc* wiadomosc, QString popupTekst = "");
    void otrzymanoZaproszenie(Uzytkownik*);
    void wyswietlInformacje(const QString& tytul, const QString& info);

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

    bool czyJestPoloczenie;
};

#endif // OKNOLOBBY_H
