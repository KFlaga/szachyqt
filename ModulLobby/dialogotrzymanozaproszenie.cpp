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

void DialogOtrzymanoZaproszenie::ustawNick(const QString &nick)
{
    ui->labelNick->setText(nick);
}

void DialogOtrzymanoZaproszenie::ustawCzas(const QString& czas)
{
    ui->labelCzas->setText("(" + czas + "min)");
}
