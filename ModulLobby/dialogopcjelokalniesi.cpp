#include "ModulLobby/dialogopcjelokalniesi.h"
#include "ui_dialogopcjelokalniesi.h"

DialogOpcjeLokalnieSI::DialogOpcjeLokalnieSI(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogOpcjeLokalnieSI)
{
    ui->setupUi(this);
    czas = 5;
    poziom = 0;
}

DialogOpcjeLokalnieSI::~DialogOpcjeLokalnieSI()
{
    delete ui;
}

void DialogOpcjeLokalnieSI::zmienCzas(int nCzas)
{
    czas = nCzas;
}

void DialogOpcjeLokalnieSI::zmienPoziom(int nPoziom)
{
    poziom = nPoziom;
}
