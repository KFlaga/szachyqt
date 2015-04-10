#ifndef DIALOGOPCJELOKALNIESI_H
#define DIALOGOPCJELOKALNIESI_H

#include <QDialog>

namespace Ui {
class DialogOpcjeLokalnieSI;
}

class DialogOpcjeLokalnieSI : public QDialog
{
    Q_OBJECT

public:
    explicit DialogOpcjeLokalnieSI(QWidget *parent = 0);
    ~DialogOpcjeLokalnieSI();

private slots:
    void zmienCzas(int);
    void zmienPoziom(int);

public:
    int czas;
    int poziom;

private:
    Ui::DialogOpcjeLokalnieSI *ui;
};

#endif // DIALOGOPCJELOKALNIESI_H
