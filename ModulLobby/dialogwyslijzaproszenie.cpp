#include "dialogwyslijzaproszenie.h"
#include "ui_dialogwyslijzaproszenie.h"

extern QRegExp znakiZarezerwowane;

DialogWyslijZaproszenie::DialogWyslijZaproszenie(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogWyslijZaproszenie)
{
    ui->setupUi(this);
    ui->labelBlad->hide();
}

DialogWyslijZaproszenie::~DialogWyslijZaproszenie()
{
    delete ui;
}

void DialogWyslijZaproszenie::sprawdzPoprawnosc()
{
    bool ok = true;
    if( wezNick() == wlasnyNick )
    {
        ui->labelBlad->setText("Nie możesz zaprosić sam siebie");
        ui->labelBlad->show();
        ok = false;
    }
    else if( wezNick().contains(znakiZarezerwowane))
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
    ui->leNick->setText(nick);
}

QString DialogWyslijZaproszenie::wezNick()
{
    return ui->leNick->text();
}

int DialogWyslijZaproszenie::wezCzas()
{
    return ui->spinCzas->value();
}
