#include "ModulLobby/oknorejestracji.h"
#include "ui_oknorejestracji.h"
#include <QtTest/QSignalSpy>
#include <QMessageBox>
#include "komunikatorlobbyserwer.h"
#include "popupoczekiwanienaserwer.h"

extern QRegExp znakiZarezerwowane;

OknoRejestracji::OknoRejestracji(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OknoRejestracji)
{
    ui->setupUi(this);

    ui->labelHasloError->setVisible(false);
    ui->labelLoginError->setVisible(false);
    ui->labelNickError->setVisible(false);

    wiadomosc = new WiadomoscRejestruj();
    oczekiwanie = new PopupOczekiwanieNaSerwer(this);
}

OknoRejestracji::~OknoRejestracji()
{
    delete ui;
    delete oczekiwanie;
    delete wiadomosc;
}

void OknoRejestracji::on_buttonRegister_clicked()
{
    // 1) Sprawdz czy wszystkie pola wypelnione
    if( !sprawdzPola() )
        return;
    // 2) Wyslij zapytanie czy Login i Nick sa unikalne i jak sa
    //    utworz uzytkownika
    wyslijDaneRejestracji();
}

bool OknoRejestracji::sprawdzPola()
{
    bool czyDobrze = true;
    QString pustePola = "Nalezy wypelnic wszystkie pola";
    QString zarezerwowane = "Nie można używać znaków specjanych";
    if(ui->leLogin->text().count() == 0)
    {
        ui->labelLoginError->setVisible(true);
        ui->labelLoginError->setText(pustePola);
        czyDobrze = false;
    }
    else if(ui->leLogin->text().contains(znakiZarezerwowane))
    {
        ui->labelLoginError->setVisible(true);
        ui->labelLoginError->setText(zarezerwowane);
        czyDobrze = false;
    }
    if(ui->leHaslo->text().count() == 0)
    {
        ui->labelHasloError->setVisible(true);
        ui->labelHasloError->setText(pustePola);
        czyDobrze = false;
    }
    else if(ui->leHaslo->text().contains(znakiZarezerwowane))
    {
        ui->labelHasloError->setVisible(true);
        ui->labelHasloError->setText(zarezerwowane);
        czyDobrze = false;
    }
    if(ui->leNick->text().count() == 0)
    {
        ui->labelNickError->setVisible(true);
        ui->labelNickError->setText(pustePola);
        czyDobrze = false;
    }
    else if(ui->leNick->text().contains(znakiZarezerwowane))
    {
        ui->labelNickError->setVisible(true);
        ui->labelNickError->setText(zarezerwowane);
        czyDobrze = false;
    }
    return czyDobrze;
}

void OknoRejestracji::wyslijDaneRejestracji()
{
    wiadomosc->login = ui->leLogin->text();
    wiadomosc->haslo = ui->leHaslo->text();
    wiadomosc->nick = ui->leNick->text();
    KomunikatorLobbySerwer* komunikator = new KomunikatorLobbySerwer(this);
    connect(komunikator, SIGNAL(nadajWiadomosc(QString,IKomunikator*)),
            this, SIGNAL(nadajWiadomosc(QString,IKomunikator*)));
    connect(komunikator, SIGNAL(odebranoZwrot(Wiadomosc*,bool)),
            this, SLOT(sprawdzOdpowiedz(Wiadomosc*,bool)));
    oczekiwanie->open();
    komunikator->wyslijWiadomosc(wiadomosc,true);
}

void OknoRejestracji::sprawdzOdpowiedz(Wiadomosc* w, bool czyAnulowano)
{
    WiadomoscRejestruj* wiadomosc = (WiadomoscRejestruj*)w;
    oczekiwanie->close();
    sender()->deleteLater();
    if( czyAnulowano )
    {
        QMessageBox mbBlad(this);
        mbBlad.setStandardButtons(QMessageBox::Ok);
        mbBlad.setWindowTitle("Blad");
        mbBlad.setText("Przekroczono czas oczekiwania na odpowiedz serwera");
        mbBlad.exec();
        return;
    }
    bool czyDobrze = true;
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
    if( czyDobrze )
    {
        QMessageBox* mbSkonczone = new QMessageBox(this);
        mbSkonczone->setText("Pomyślnie zakończono rejestrację");
        mbSkonczone->setStandardButtons(QMessageBox::Ok);
        mbSkonczone->setWindowTitle("Rejestracja");
        mbSkonczone->exec();
        close();
    }
}
