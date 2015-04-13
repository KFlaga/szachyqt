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
    sprobujZalogowac();
    if( uzytkownik->id < 0 ) // nie znalazlo uzytkownika
    {
        ui->labelZleDane->show();
    }
    else if( uzytkownik->czy_zalogowany == true )
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
        uzytkownik->czy_zalogowany = true;
        emit zalogowano();
        close();
    }
}

void OknoLogowania::sprobujZalogowac()
{
    // Wyslij zapytanie - sygnal do szachyApp, a z tamtad do modulu netowego
    // narazie testowy
    uzytkownik->czy_zalogowany = false;
    uzytkownik->id = 1;
    uzytkownik->nick = "test_" + ui->leLogin->text();
    uzytkownik->czy_gosc = false;
    uzytkownik->ranking = 100;
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
