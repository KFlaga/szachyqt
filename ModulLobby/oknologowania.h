#ifndef OKNOLOGOWANIA_H
#define OKNOLOGOWANIA_H

#include <QDialog>
#include "ModulLobby/oknorejestracji.h"
#include "uzytkownik.h"
#include "Wiadomosci/wiadomoscloguj.h"
#include "komunikatorlobbyserwer.h"

namespace Ui {
class OknoLogowania;
}

enum WynikLogowania { Poprawne, UzytkownikZalogowany, ZleDane };

class OknoLogowania : public QDialog
{
    Q_OBJECT

public:
    explicit OknoLogowania(QWidget *parent, KomunikatorLobbySerwer *kom);
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

signals:
    void zalogowano();

private:
    Ui::OknoLogowania *ui;
    OknoRejestracji* oknoRejestracji;
    Uzytkownik* uzytkownik;
    WiadomoscLoguj* wiadomosc;
    KomunikatorLobbySerwer* komunikator;
};

#endif // OKNOLOGOWANIA_H
