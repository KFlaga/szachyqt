#include "dialogpodajnickgracza.h"
#include "ui_dialogpodajnickgracza.h"

DialogPodajNickGracza::DialogPodajNickGracza(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogPodajNickGracza)
{
    ui->setupUi(this);
}

DialogPodajNickGracza::~DialogPodajNickGracza()
{
    delete ui;
}
