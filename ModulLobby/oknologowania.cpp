#include "ModulLobby/oknologowania.h"
#include "ui_oknologowania.h"
#include <QtTest/QSignalSpy>
#include <QMessageBox>

OknoLogowania::OknoLogowania(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OknoLogowania)
{
    ui->setupUi(this);
    ui->labelZleDane->hide();
}

OknoLogowania::~OknoLogowania()
{
    delete ui;
}

void OknoLogowania::on_buttonLogin_clicked()
{
    Uzytkownik* uzyt = sprobujZalogowac();
    if( uzyt->id < 0 ) // nie znalazlo uzytkownika
    {
        ui->labelZleDane->show();
    }
    else if( uzyt->czy_zalogowany == true )
    {
       QMessageBox* mbZalogowany = new QMessageBox(this);
       mbZalogowany->setText("Użytkownik o podanym loginie i haśle jest już zalogowany");
       mbZalogowany->setStandardButtons(QMessageBox::Ok);
       mbZalogowany->setWindowTitle("Zalogowany");
       mbZalogowany->exec();
       delete mbZalogowany;
    }
    else
    {
        uzyt->czy_zalogowany = true;
        emit zalogowano(uzyt);
    }
}

Uzytkownik* OknoLogowania::sprobujZalogowac()
{
    // Wyslij zapytanie - sygnal do szachyApp, a z tamtad do modulu netowego
    // narazie testowy
    Uzytkownik* uzyt = new Uzytkownik();
    uzyt->czy_zalogowany = false;
    uzyt->id = 1;
    uzyt->nick = "Test";
    uzyt->czy_gosc = false;
    uzyt->ranking = 100;
    return uzyt;
}

void OknoLogowania::on_buttonRegister_clicked()
{
    oknoRejestracji = new OknoRejestracji(this);
    oknoRejestracji->exec();
    delete oknoRejestracji;
}


void OknoLogowania::on_leHaslo_textChanged(const QString &arg1)
{}

void OknoLogowania::on_leLogin_textChanged(const QString &arg1)
{}
