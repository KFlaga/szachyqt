#include "oknoopcji.h"
#include "ui_oknoopcji.h"

OknoOpcji::OknoOpcji(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OknoOpcji)
{
    ui->setupUi(this);
    ui->radioButton->setChecked(true);
    ui->comboBox->setDisabled(false);
    ui->comboBox_2->setDisabled(true);
}

OknoOpcji::~OknoOpcji()
{
    delete ui;
}

void OknoOpcji::on_buttonBox_accepted()
{
    this->hide();
    emit nowa_gra();
}

void OknoOpcji::on_buttonBox_rejected()
{
    this->hide();
}

void OknoOpcji::on_radioButton_clicked()
{
    ui->comboBox_2->setDisabled(true);
}

void OknoOpcji::on_radioButton_2_clicked()
{
    ui->comboBox_2->setDisabled(false);
}


bool OknoOpcji::gra_z_kompem()
{
    if(ui->radioButton->isChecked())
        return false;
    else
        return true;
}
int OknoOpcji::max_czas()
{
    return ui->comboBox->currentIndex();
}
int OknoOpcji::poziom_trudnosci()
{
    return ui->comboBox_2->currentIndex();
}
