#include "dialogszukanie.h"
#include "ui_dialogszukanie.h"
#include <QMessageBox>
DialogSzukanie::DialogSzukanie(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSzukanie)
{
    ui->setupUi(this);
}

DialogSzukanie::~DialogSzukanie()
{
    delete ui;
}

void DialogSzukanie::on_buttonAnuluj_clicked()
{
    close();
}

void DialogSzukanie::on_buttonSzukaj_clicked()
{
    close();
    emit szukajG("szukaj:0-" + QString::number(ui->spinCzas->value()) + ":35.");
}
