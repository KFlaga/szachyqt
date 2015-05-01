#include "ModulLobby/oknologowania.h"
#include "ui_oknologowania.h"
#include <QMessageBox>

extern QRegExp znakiZarezerwowane;

OknoLogowania::OknoLogowania(QWidget *parent, KomunikatorLobbySerwer* kom) :
    QDialog(parent),
    ui(new Ui::OknoLogowania)
{
    ui->setupUi(this);
    ui->labelZleDane->hide();
    wiadomosc = new WiadomoscLoguj();
    komunikator = kom;
}

OknoLogowania::~OknoLogowania()
{
    delete ui;
}

void OknoLogowania::on_buttonLogin_clicked()
{
    if( !sprawdzPola() )
        return;
    if( !sprobujZalogowac() )
        return;
    if( wiadomosc->czyDaneOk == false ) // nie znalazlo uzytkownika
    {
        ui->labelZleDane->setText("Podano zły login lub hasło");
        ui->labelZleDane->show();
        return;
    }
    ustawDaneUzytkownika();
    if( uzytkownik->czy_zalogowany == true )
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

bool OknoLogowania::sprobujZalogowac()
{
    wiadomosc->czyDaneOk = false;
    wiadomosc->login = ui->leLogin->text();
    wiadomosc->haslo = ui->leHaslo->text();
    KomunikatorLobbySerwer::WynikWyslania status = komunikator->wyslijWiadomoscZeZwrotem(wiadomosc);
    // powinno wrocic dopiero po wiadomosci zwrotnej
    if( status == KomunikatorLobbySerwer::Powodzenie )
        return true;

    QMessageBox* mbBlad = new QMessageBox(this);
    mbBlad->setText("Przekroczono czas oczekiwania na odpowiedz serwera");
    mbBlad->setStandardButtons(QMessageBox::Ok);
    mbBlad->setWindowTitle("Blad");
    if ( status == KomunikatorLobbySerwer::PrzekroczonoCzas )
        mbBlad->setText("Przekroczono czas oczekiwania na odpowiedz serwera");
    else
        mbBlad->setText("Niepowodzenie logowania");

    mbBlad->exec();
    delete mbBlad;

    return false;
}

bool OknoLogowania::sprawdzPola()
{
    if(ui->leHaslo->text().count() == 0 ||
       ui->leLogin->text().count() == 0 )
    {
        ui->labelZleDane->setText("Należy wypełnić wszystkie pola");
    }
    else if(ui->leHaslo->text().contains(znakiZarezerwowane) ||
            ui->leLogin->text().contains(znakiZarezerwowane) )
    {
        ui->labelZleDane->setText("Nie można uzywać znaków specjalnych");
    }
    else
        return true;

    ui->labelZleDane->show();
    return false;
}

void OknoLogowania::ustawDaneUzytkownika()
{
    uzytkownik->nick = wiadomosc->nick;
    uzytkownik->ranking = wiadomosc->ranking;
    uzytkownik->czy_zalogowany = wiadomosc->czy_zalogowany;
    uzytkownik->status = wiadomosc->status;
}

void OknoLogowania::on_buttonRegister_clicked()
{
    oknoRejestracji = new OknoRejestracji(this, komunikator);
    oknoRejestracji->exec();
    delete oknoRejestracji;
}
