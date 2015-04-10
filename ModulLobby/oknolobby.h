#ifndef OKNOLOBBY_H
#define OKNOLOBBY_H

#include <QMainWindow>
#include "uzytkownik.h"
#include "opcje.h"

namespace Ui {
class OknoLobby;
}

class OknoLobby : public QMainWindow
{
    Q_OBJECT

public:
    explicit OknoLobby(QWidget *parent = 0);
    ~OknoLobby();

private slots:
    void wyloguj();
    void zagrajLokalnieGracz();
    void zagrajLokalnieSI();
    void zaprosGracza();
    void szukajGracza();
    void zaloguj();
    void zalogowano(Uzytkownik*);
    void closeEvent(QCloseEvent *);

signals:
    void sygZalogowano(Uzytkownik*);
    void graLokalnie(Opcje*);

private:
    Ui::OknoLobby *ui;
    Uzytkownik* biezacyUzytkownik;
};

#endif // OKNOLOBBY_H
