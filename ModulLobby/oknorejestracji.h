#ifndef OKNOREJESTRACJI_H
#define OKNOREJESTRACJI_H

#include <QDialog>
#include "Wiadomosci/wiadomoscrejestruj.h"
#include "ModulKomunikacji/ikomunikator.h"
#include "popupoczekiwanienaserwer.h"

namespace Ui {
class OknoRejestracji;
}

class OknoRejestracji : public QDialog
{
    Q_OBJECT

public:
    explicit OknoRejestracji(QWidget *parent);
    ~OknoRejestracji();

private:
    bool sprawdzPola();
    void wyslijDaneRejestracji();

private slots:
    void on_buttonRegister_clicked();
    void sprawdzOdpowiedz(Wiadomosc* wiadomosc, bool czyAnulowano);

signals:
    void nadajWiadomosc(const QString&, IKomunikator*);

private:
    Ui::OknoRejestracji *ui;
    WiadomoscRejestruj* wiadomosc;
    PopupOczekiwanieNaSerwer* oczekiwanie;
};

#endif // OKNOREJESTRACJI_H
