#include "dialogczekajnaodpowiedz.h"
#include "ui_dialogczekajnaodpowiedz.h"

DialogCzekajNaOdpowiedz::DialogCzekajNaOdpowiedz(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogCzekajNaOdpowiedz)
{
    ui->setupUi(this);
}

DialogCzekajNaOdpowiedz::~DialogCzekajNaOdpowiedz()
{
    delete ui;
}
