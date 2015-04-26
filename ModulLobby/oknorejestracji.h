#ifndef OKNOREJESTRACJI_H
#define OKNOREJESTRACJI_H

#include <QDialog>
#include "Wiadomosci/wiadomoscrejestruj.h"
#include "komunikatorlobbyserwer.h"

namespace Ui {
class OknoRejestracji;
}

class OknoRejestracji : public QDialog
{
    Q_OBJECT

public:
    explicit OknoRejestracji(QWidget *parent, KomunikatorLobbySerwer *kom);
    ~OknoRejestracji();

private:
    bool sprawdzPola();
    bool sprobujZarejestrowac();

private slots:
    void on_buttonRegister_clicked();

private:
    Ui::OknoRejestracji *ui;
    WiadomoscRejestruj* wiadomosc;
    KomunikatorLobbySerwer* komunikator;
};

#endif // OKNOREJESTRACJI_H
