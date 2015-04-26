#include "dialogwyslijzaproszenie.h"
#include "ui_dialogwyslijzaproszenie.h"

DialogWyslijZaproszenie::DialogWyslijZaproszenie(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogWyslijZaproszenie)
{
    ui->setupUi(this);
}

DialogWyslijZaproszenie::~DialogWyslijZaproszenie()
{
    delete ui;
}
