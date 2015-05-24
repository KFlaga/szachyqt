#ifndef OKNOLOBBY_H
#define OKNOLOBBY_H

#include <QMainWindow>
#include "uzytkownik.h"
#include "opcje.h"
#include "ModulKomunikacji/ikomunikator.h"
#include "popupoczekiwanienaserwer.h"
#include "ModulKomunikacji/klient.h"
#include "Wiadomosci/wiadomosczaproszenie.h"
#include <QTimer>

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

    void oczekujNaOdpowiedz();
    void koniecGry();

public slots:
    void poloczonoZSerwerem();
    void rozloczonoZSerwerem();
    void nieMoznaPolaczycZSerwerem(int);
    void otrzymanoZaproszenie(QString dane);
    void zacznijPojedynek(QString);
    void anulujPojedynek();
    void anulujPojedynek(QString dane);
    void odmowaPojedynku(QString);


private:
    void aktualizujInterfejs();
    void wyswietlInformacje(const QString& tytul, const QString& info);
    void wyslijZaproszenie(const QString& nick, int czas);

private slots:
    void wyloguj();
    void zagrajLokalnieGracz();
    void zagrajLokalnieSI();
    void zaprosGracza(QString = "");
    void szukajGracza();
    void zaloguj();
    void zalogowano();
    void closeEvent(QCloseEvent *);
    void odpowiedzNaZaproszenie(int);
    void zadajListyUzytkownikow();
    void sprawdzZwrotZaproszenia(Wiadomosc*, bool);
    void aktualizujListeUzytkownikow(Wiadomosc* wiadomosc, bool czyAnulowano);

    void aktualizujRanking(int rank);

signals:
    void sygZalogowano();
    void graLokalnie(Opcje*);
    void graSieciowa(Opcje*);
    void nadajWiadomosc(const QString&, IKomunikator*);

private:
    Ui::OknoLobby *ui;
    Uzytkownik* biezacyUzytkownik;
    bool czy_zalogowano;
    QTimer timerOdswiezListe;
    PopupOczekiwanieNaSerwer* oczekiwanie;
    QTimer* timerOczekiwanie;

    bool czyJestPoloczenie;
    bool oczekiwanieNaOdpowiedz;
    bool zaproszenieOdrzucone;
    bool powodzeniePojedynku;
    Opcje* opts;
};

#endif // OKNOLOBBY_H
