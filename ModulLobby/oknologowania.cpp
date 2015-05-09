#include "ModulLobby/oknologowania.h"
#include "ui_oknologowania.h"
#include <QMessageBox>
#include "komunikatorlobbyserwer.h"

extern QRegExp znakiZarezerwowane;

OknoLogowania::OknoLogowania(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OknoLogowania)
{
    ui->setupUi(this);
    ui->labelZleDane->hide();
    wiadomosc = new WiadomoscLoguj();
    oczekiwanie = new PopupOczekiwanieNaSerwer(this);
}

OknoLogowania::~OknoLogowania()
{
    delete ui;
    delete oczekiwanie;
    delete wiadomosc;
}

void OknoLogowania::on_buttonLogin_clicked()
{
    if( !sprawdzPola() )
        return;
    wyslijDaneLogowania();
}

void OknoLogowania::wyslijDaneLogowania()
{
    wiadomosc->czyDaneOk = false;
    wiadomosc->login = ui->leLogin->text();
    wiadomosc->haslo = ui->leHaslo->text();

    KomunikatorLobbySerwer* komunikator = new KomunikatorLobbySerwer(this);
    connect(komunikator, SIGNAL(nadajWiadomosc(QString,IKomunikator*)),
            this, SIGNAL(nadajWiadomosc(QString,IKomunikator*)));
    connect(komunikator, SIGNAL(odebranoZwrot(Wiadomosc*,bool)),
            this, SLOT(sprawdzOdpowiedz(Wiadomosc*, bool)));
    oczekiwanie->open();
    komunikator->wyslijWiadomosc(wiadomosc,true);
}

void OknoLogowania::sprawdzOdpowiedz(Wiadomosc* w, bool czyAnulowano)
{
    oczekiwanie->close();
    sender()->deleteLater();
    WiadomoscLoguj* wiadomosc = (WiadomoscLoguj*)w;

    if( !czyAnulowano )
    {
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
    else
    {
        QMessageBox mbBlad(this);
        mbBlad.setText("Przekroczono czas oczekiwania na odpowiedz serwera");
        mbBlad.setStandardButtons(QMessageBox::Ok);
        mbBlad.setWindowTitle("Blad");
        mbBlad.exec();
    }
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
    oknoRejestracji = new OknoRejestracji(this);
    connect(oknoRejestracji, SIGNAL(nadajWiadomosc(const QString&, IKomunikator*)),
            this, SIGNAL(nadajWiadomosc(QString,IKomunikator*)));
    oknoRejestracji->exec();
    delete oknoRejestracji;
}
