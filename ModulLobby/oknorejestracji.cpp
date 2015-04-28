#include "ModulLobby/oknorejestracji.h"
#include "ui_oknorejestracji.h"
#include <QtTest/QSignalSpy>
#include <QMessageBox>
#include "komunikatorlobbyserwer.h"

OknoRejestracji::OknoRejestracji(QWidget *parent, KomunikatorLobbySerwer* kom) :
    QDialog(parent),
    ui(new Ui::OknoRejestracji)
{
    ui->setupUi(this);

    ui->labelHasloError->setVisible(false);
    ui->labelLoginError->setVisible(false);
    ui->labelNickError->setVisible(false);

    wiadomosc = new WiadomoscRejestruj();
    komunikator = kom;
}

OknoRejestracji::~OknoRejestracji()
{
    delete ui;
}

void OknoRejestracji::on_buttonRegister_clicked()
{
    // 1) Sprawdz czy wszystkie pola wypelnione
    if( !sprawdzPola() )
        return;
    // 2) Wyslij zapytanie czy Login i Nick sa unikalne i jak sa
    //    utworz uzytkownika
    if( !sprobujZarejestrowac() )
        return;
    // 4) Wysietl info, ze pomyslinie zakonczona
    QMessageBox* mbSkonczone = new QMessageBox(this);
    mbSkonczone->setText("Pomyślnie zakończono rejestrację");
    mbSkonczone->setStandardButtons(QMessageBox::Ok);
    mbSkonczone->setWindowTitle("Rejestracja");
    mbSkonczone->exec();

    delete mbSkonczone;
    close();
}

bool OknoRejestracji::sprawdzPola()
{
    bool czyDobrze = true;
    QRegExp reserved("[-:,]");
    if(ui->leLogin->text().count() == 0)
    {
        ui->labelLoginError->setVisible(true);
        ui->labelLoginError->setText("Należy wypełnić wszystkie pola");
        czyDobrze = false;
    }
    else if(ui->leLogin->text().contains(reserved))
    {
        ui->labelLoginError->setVisible(true);
        ui->labelLoginError->setText("Nie można uzywać znaków: '-',':',','");
        czyDobrze = false;
    }
    if(ui->leHaslo->text().count() == 0)
    {
        ui->labelHasloError->setVisible(true);
        ui->labelHasloError->setText("Należy wypełnić wszystkie pola");
        czyDobrze = false;
    }
    else if(ui->leHaslo->text().contains(reserved))
    {
        ui->labelHasloError->setVisible(true);
        ui->labelHasloError->setText("Nie można uzywać znaków: '-',':',','");
        czyDobrze = false;
    }
    if(ui->leNick->text().count() == 0)
    {
        ui->labelNickError->setVisible(true);
        ui->labelNickError->setText("Należy wypełnić wszystkie pola");
        czyDobrze = false;
    }
    else if(ui->leNick->text().contains(reserved))
    {
        ui->labelNickError->setVisible(true);
        ui->labelNickError->setText("Nie można uzywać znaków: '-',':',','");
        czyDobrze = false;
    }
    return czyDobrze;
}

bool OknoRejestracji::sprobujZarejestrowac()
{
    bool czyDobrze = true;
    wiadomosc->login = ui->leLogin->text();
    wiadomosc->haslo = ui->leHaslo->text();
    wiadomosc->nick = ui->leNick->text();
    KomunikatorLobbySerwer::WynikWyslania status = komunikator->wyslijWiadomoscZeZwrotem(wiadomosc);

    if( status != KomunikatorLobbySerwer::Powodzenie )
    {
        QMessageBox* mbBlad = new QMessageBox(this);
        mbBlad->setStandardButtons(QMessageBox::Ok);
        mbBlad->setWindowTitle("Blad");
        if ( status == KomunikatorLobbySerwer::PrzekroczonoCzas )
            mbBlad->setText("Przekroczono czas oczekiwania na odpowiedz serwera");
        else
            mbBlad->setText("Niepowodzenie rejestracji");

        mbBlad->exec();
        delete mbBlad;
        return false;
    }
    if( !wiadomosc->loginOK ) // czy istnieje juz taki login
    {
        ui->labelLoginError->setVisible(true);
        ui->labelLoginError->setText("Istnieje juz taki login");
        czyDobrze = false;
    }
    if( !wiadomosc->nickOK ) // czy istnieje juz taki nick
    {
        ui->labelNickError->setVisible(true);
        ui->labelNickError->setText("Istnieje juz taki pseudonim");
        czyDobrze = false;
    }
    return czyDobrze;
}
