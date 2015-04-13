#include "dialogotrzymanozaproszenie.h"
#include "ui_dialogotrzymanozaproszenie.h"

DialogOtrzymanoZaproszenie::DialogOtrzymanoZaproszenie(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogOtrzymanoZaproszenie)
{
    ui->setupUi(this);
}

DialogOtrzymanoZaproszenie::~DialogOtrzymanoZaproszenie()
{
    delete ui;
}
