#include "dialogwyslijzaproszenie.h"
#include "ui_dialogwyslijzaproszenie.h"

DialogWyslijZaproszenie::DialogWyslijZaproszenie(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogWyslijZaproszenie)
{
    ui->setupUi(this);
    czas = 5;
}

DialogWyslijZaproszenie::~DialogWyslijZaproszenie()
{
    delete ui;
}

void DialogWyslijZaproszenie::sprawdzPoprawnosc()
{
    QRegExp reserved("[-:,]");
    if( nick == wlasnyNick )
    {
        ui->labelBlad->setText("Nie możesz zaprosić sam siebie");
        ui->labelBlad->show();
    }
    else if( nick.contains(reserved))
    {
        ui->labelBlad->setText("Nick nie może zawierać: - : ,");
        ui->labelBlad->show();
    }
}
