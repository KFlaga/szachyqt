#include "ModulLobby/oknorejestracji.h"
#include "ui_oknorejestracji.h"
#include <QtTest/QSignalSpy>
#include <QMessageBox>

OknoRejestracji::OknoRejestracji(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OknoRejestracji)
{
    ui->setupUi(this);

    ui->labelHasloError->setVisible(false);
    ui->labelLoginError->setVisible(false);
    ui->labelNickError->setVisible(false);
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
    // 2) Wyslij zapytanie czy Login i Nick sa unikalne
    if( !sprawdzUnikalnoscDanych() )
        return;
    // 3) Jak sa to wyslij polecenie utworzenia uzytkownika
    if( !utworzUzytkownika() )
        return;
    // 4) Wysietl info, ze pomyslinie zakonczona i po kliknieciu ok emituj sygnal
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
    if(ui->leLogin->text().count() == 0)
    {
        ui->labelLoginError->setVisible(true);
        ui->labelLoginError->setText("Należy wypełnić wszystkie pola");
        czyDobrze = false;
    }
    if(ui->leHaslo->text().count() == 0)
    {
        ui->labelHasloError->setVisible(true);
        ui->labelHasloError->setText("Należy wypełnić wszystkie pola");
        czyDobrze = false;
    }
    if(ui->leNick->text().count() == 0)
    {
        ui->labelNickError->setVisible(true);
        ui->labelNickError->setText("Należy wypełnić wszystkie pola");
        czyDobrze = false;
    }
    return czyDobrze;
}

bool OknoRejestracji::sprawdzUnikalnoscDanych()
{
    bool czyDobrze = true;
    // Tutaj zapytania - do zrealizowania jak będzie ustalony interfejs
    if( false ) // czy istnieje juz taki login
    {
        ui->labelLoginError->setVisible(true);
        ui->labelLoginError->setText("Istnieje juz taki login");
        czyDobrze = false;
    }
    if( false ) // czy istnieje juz taki nick
    {
        ui->labelNickError->setVisible(true);
        ui->labelNickError->setText("Istnieje juz taki pseudonim");
        czyDobrze = false;
    }
    return czyDobrze;
}

bool OknoRejestracji::utworzUzytkownika()
{
    // Tez do zrobienia dopiero pozniej
    return true;
}
