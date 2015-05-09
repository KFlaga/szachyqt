#ifndef OKNOLOGOWANIA_H
#define OKNOLOGOWANIA_H

#include <QDialog>
#include "ModulLobby/oknorejestracji.h"
#include "uzytkownik.h"
#include "Wiadomosci/wiadomoscloguj.h"
#include "ModulKomunikacji/ikomunikator.h"
#include "popupoczekiwanienaserwer.h"

namespace Ui {
class OknoLogowania;
}

enum WynikLogowania { Poprawne, UzytkownikZalogowany, ZleDane };

class OknoLogowania : public QDialog
{
    Q_OBJECT

public:
    explicit OknoLogowania(QWidget *parent);
    ~OknoLogowania();

    void ustawUzytkownika(Uzytkownik* u)
    {
        uzytkownik = u;
    }

private:
    bool sprobujZalogowac();
    void ustawDaneUzytkownika();
    bool sprawdzPola();

private slots:
    void on_buttonLogin_clicked();
    void on_buttonRegister_clicked();
    void sprawdzOdpowiedz(Wiadomosc *wiadomosc, bool czyAnulowano);
    void wyslijDaneLogowania();

signals:
    void zalogowano();
    void nadajWiadomosc(const QString&, IKomunikator*);

private:
    Ui::OknoLogowania *ui;
    OknoRejestracji* oknoRejestracji;
    Uzytkownik* uzytkownik;
    WiadomoscLoguj* wiadomosc;
    PopupOczekiwanieNaSerwer* oczekiwanie;
};

#endif // OKNOLOGOWANIA_H
