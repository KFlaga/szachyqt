#include "oknologowania.h"
#include "ui_oknologowania.h"

OknoLogowania::OknoLogowania(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OknoLogowania)
{
    ui->setupUi(this);
}

OknoLogowania::~OknoLogowania()
{
    delete ui;
}

void OknoLogowania::on_buttonLogin_clicked()
{
    emit logowanieZakonczone();
}
