#ifndef DIALOGOPCJELOKALNIEGRACZ_H
#define DIALOGOPCJELOKALNIEGRACZ_H

#include <QDialog>

namespace Ui {
class DialogOpcjeLokalnieGracz;
}

class DialogOpcjeLokalnieGracz : public QDialog
{
    Q_OBJECT

public:
    explicit DialogOpcjeLokalnieGracz(QWidget *parent = 0);
    ~DialogOpcjeLokalnieGracz();

private slots:
    void zmienCzas(int);

public:
    int czas;

private:
    Ui::DialogOpcjeLokalnieGracz *ui;
};

#endif // DIALOGOPCJELOKALNIEGRACZ_H
