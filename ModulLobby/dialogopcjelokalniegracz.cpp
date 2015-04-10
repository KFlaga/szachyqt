#include "ModulLobby/dialogopcjelokalniegracz.h"
#include "ui_dialogopcjelokalniegracz.h"

DialogOpcjeLokalnieGracz::DialogOpcjeLokalnieGracz(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogOpcjeLokalnieGracz)
{
    ui->setupUi(this);
    czas = 5;
}

DialogOpcjeLokalnieGracz::~DialogOpcjeLokalnieGracz()
{
    delete ui;
}

void DialogOpcjeLokalnieGracz::zmienCzas(int nCzas)
{
    czas = nCzas;
}
