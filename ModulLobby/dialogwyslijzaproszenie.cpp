#include "dialogwyslijzaproszenie.h"
#include "ui_dialogwyslijzaproszenie.h"

extern QRegExp znakiZarezerwowane;

DialogWyslijZaproszenie::DialogWyslijZaproszenie(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogWyslijZaproszenie)
{
    ui->setupUi(this);
    czas = 5;
    ui->labelBlad->hide();
}

DialogWyslijZaproszenie::~DialogWyslijZaproszenie()
{
    delete ui;
}

void DialogWyslijZaproszenie::sprawdzPoprawnosc()
{
    bool ok = true;
    if( nick == wlasnyNick )
    {
        ui->labelBlad->setText("Nie możesz zaprosić sam siebie");
        ui->labelBlad->show();
        ok = false;
    }
    else if( nick.contains(znakiZarezerwowane))
    {
        ui->labelBlad->setText("Nick nie może zawierać znaków specjanych");
        ui->labelBlad->show();
        ok = false;
    }
    if(ok)
        accept();
}

void DialogWyslijZaproszenie::podstawNick(const QString& nick)
{
    this->nick = nick;
    ui->leNick->setText(nick);
}
