#ifndef OKNOOPCJI_H
#define OKNOOPCJI_H

#include <QDialog>
#include "opcje.h"

namespace Ui {
class OknoOpcji;
}

class OknoOpcji : public QDialog
{
    Q_OBJECT

public:
    explicit OknoOpcji(QWidget *parent = 0);
    ~OknoOpcji();
    bool gra_z_kompem();                    //metoda zwraca z ui czy zaznaczone jest pole z grą z kompem czy z człowiekiem
    int max_czas();                         //metoda zwraca z ui wybrany czas
    int poziom_trudnosci();                 //metoda zwraca z ui wybrany poziom trudności

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_radioButton_clicked();

    void on_radioButton_2_clicked();

signals:
    void noweOpcje();
    void stareOpcje();

private:
    Ui::OknoOpcji *ui;
};

#endif // OKNOOPCJI_H
