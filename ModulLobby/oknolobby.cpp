#include "ModulLobby/oknolobby.h"
#include "ui_oknolobby.h"
#include "ModulLobby/oknologowania.h"
#include "ModulLobby/dialogopcjelokalniegracz.h"
#include "ModulLobby/dialogopcjelokalniesi.h"
#include "ModulLobby/dialogwyslijzaproszenie.h"
#include <QMessageBox>
#include "Wiadomosci/wiadomoscwyloguj.h"
#include "Wiadomosci/wiadomosczaproszenie.h"
#include "Wiadomosci/wiadomoscodpowiedznazaproszenie.h"

OknoLobby::OknoLobby(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::OknoLobby)
{
    ui->setupUi(this);
    czy_zalogowano = false;
    this->statusBar()->showMessage("Nie ma połączenia z serwerem");
    komunikator = new KomunikatorLobbySerwer(this);
}

OknoLobby::~OknoLobby()
{
    delete ui;
}

void OknoLobby::closeEvent(QCloseEvent * ce)
{
    // Trzebaby upewnic sie, ze wyloguje poprawnie
    // Przy czym tu spostrzerzenie odrazu, ze
    // na serwerze trzeba będzie reagować nie tylko na
    // żądanie wylogowania, ale także wylogowowyać
    // w razie zerwania połączenia
    // I tu kolejna myśl - przy zerwaniu połączenia podczas gry
    // dawać wiadomość i próbować wznowić połączenie, a przy timeoucie
    // wylogowac i opuść grę na kliencie i serwerze, przy czym
    // trezba rozważyć możliwość wznowienia gry później
    qApp->closeAllWindows();
}

void OknoLobby::wyloguj()
{
    WiadomoscWyloguj* wiadomosc = new WiadomoscWyloguj();
    wyslijWiadomosc(wiadomosc);
    czy_zalogowano = false;
    aktualizujInterfejs();
    delete wiadomosc;
}

void OknoLobby::zaloguj()
{
    OknoLogowania* oknoLog = new OknoLogowania(this, komunikator);
    oknoLog->ustawUzytkownika(biezacyUzytkownik);
    connect(oknoLog, SIGNAL(zalogowano()), this, SLOT(zalogowano()));
    oknoLog->exec();
}

void OknoLobby::zalogowano()
{
    czy_zalogowano = true;
    aktualizujInterfejs();
    emit sygZalogowano();
}

void OknoLobby::zagrajLokalnieSI()
{
    DialogOpcjeLokalnieSI* dialogOpts = new DialogOpcjeLokalnieSI(this);
    int result = dialogOpts->exec();
    if( result == QDialog::Rejected )
        return;
    Opcje* opts = new Opcje();
    opts->CzyGraAI = true;
    opts->MaxCzas = dialogOpts->czas;
    opts->PoziomTrudnosci = dialogOpts->poziom;

    delete dialogOpts;
    emit graLokalnie(opts);
}

void OknoLobby::zagrajLokalnieGracz()
{
    DialogOpcjeLokalnieGracz* dialogOpts = new DialogOpcjeLokalnieGracz(this);
    int result = dialogOpts->exec();
    if( result == QDialog::Rejected )
        return;
    Opcje* opts = new Opcje();
    opts->CzyGraAI = false;
    opts->MaxCzas = dialogOpts->czas;

    delete dialogOpts;
    emit graLokalnie(opts);
}

void OknoLobby::szukajGracza()
{
    // Pomysł jest taki:
    // istnieje kolejka graczy, którzy dali opcje szukaj gracza
    // są 3 opcje wyszukiwania - silniejszy, równy, lepszy ( liczone jakimś tam rankingiem )
    // Z kolejki wybiera odpowiedniego gracza ( przy czym jak szuka np. gracz z 100pkt
    // z zanaczonym lepszy, to dobierze tego z 200pkt jeśli tamten ma zaznaczone słabszy,
    // można także rozważyć opcje wyboru przedziału szukania, lub jakiś inny jeszcze system  kolejki )
    // Po wyszukaniu gracza pyta obu o zgodę i ustalenie czasu rozgrywki -> tu
    // opcja jest taka, że widzimy propozycję 2 gracza, a on naszą i jak obaj dadzą taką samą
    // to gramy - trochę dziwne rozwiązanie ale nie mam pomysłu jak to rozwiązać inaczej
    //wyslijWiadomosc();
}

void OknoLobby::zaprosGracza()
{
    DialogWyslijZaproszenie* dialogZaproszenie = new DialogWyslijZaproszenie(this);
    int result = dialogZaproszenie->exec();
    if( result == QDialog::Accepted )
    {
        WiadomoscZaproszenie* zaproszenie = new WiadomoscZaproszenie();
        zaproszenie->nick = dialogZaproszenie->wezNick();
        zaproszenie->czas = dialogZaproszenie->wezCzas();
        wyslijWiadomosc(zaproszenie);

        if( zaproszenie->czyZaproszenieWyslane )
        {
            WiadomoscOdpowiedzNaZaproszenie* odpowiedz = new WiadomoscOdpowiedzNaZaproszenie();
            komunikator->ustawCzasOczekiwania(40000); // w tym 30 sek na zgode gracza
            wyslijWiadomosc(odpowiedz, "Oczekiwanie na odpowiedz\n         gracza");
            if( odpowiedz->czyZgoda )
            {
                // i tutaj inicjalizacja pojedynku
            }
            else if( odpowiedz->czyPoprawnieOdebrane() )
            {
                QMessageBox* mbNiezgoda = new QMessageBox(this);
                mbNiezgoda->setWindowTitle("Odmowa");
                mbNiezgoda->setText("Gracz odmówił gry");
                mbNiezgoda->setStandardButtons(QMessageBox::Ok);
                mbNiezgoda->exec();
                delete mbNiezgoda;
            }
            delete odpowiedz;
        }
        else
        {
            QMessageBox* mbNieZnalazlo = new QMessageBox(this);
            mbNieZnalazlo->setWindowTitle("Niepowodzenie");
            mbNieZnalazlo->setText("Nie wyslano zaproszenia: " + zaproszenie->powodNiepowodzenia);
            mbNieZnalazlo->setStandardButtons(QMessageBox::Ok);
            mbNieZnalazlo->exec();
            delete mbNieZnalazlo;
        }

        delete zaproszenie;
    }
}

void OknoLobby::otrzymanoZaproszenie(Uzytkownik* nadawca)
{
    // stworzyc nowy dialog

    // Opcje: anulowanie zaproszenia i odrzucenie zaproszenia
}

void OknoLobby::wyslijWiadomosc(Wiadomosc* wiadomosc, QString popupTekst)
{
    KomunikatorLobbySerwer::WynikWyslania res =
            komunikator->wyslijWiadomoscZeZwrotem(wiadomosc,popupTekst);
    if( res == KomunikatorLobbySerwer::PrzekroczonoCzas )
    {
        QMessageBox mb(this);
        mb.setText("Przekroczono czas oczekiwania na odpowiedź serwera");
        mb.setWindowTitle(" ");
        mb.setStandardButtons(QMessageBox::Ok);
        mb.exec();
    }
    // Wiadmosci poniezej w celach informacyjnych dla nas
    else if( res == KomunikatorLobbySerwer::Anulowano )
    {
        this->statusBar()->showMessage("Anulowano probę komunikacji z serwerem", 1000);
    }
    else if( res == KomunikatorLobbySerwer::Zajety )
    {
        this->statusBar()->showMessage("Komunikator jest zajety", 1000);
    }
    else
    {
        this->statusBar()->showMessage("Pomyślnie skomunikowano z serwerem", 1000);
    }

}

void OknoLobby::aktualizujInterfejs()
{
    if(czy_zalogowano == false)
    {
        ui->teGrajInternet->setEnabled(false);
        ui->teProfil->setEnabled(false);
        ui->buttonSzukaj->setEnabled(false);
        ui->buttonZapros->setEnabled(false);
        ui->tePseudonimInfo->setEnabled(false);
        ui->teRankingInfo->setEnabled(false);
        ui->tePseudonim->setEnabled(false);
        ui->teRanking->setEnabled(false);
        ui->tePseudonim->setText("nie zalogowano");
        ui->teRanking->setText("nie zalogowano");
        return;
    }
    ui->teGrajInternet->setEnabled(true);
    ui->teProfil->setEnabled(true);
    ui->buttonSzukaj->setEnabled(true);
    ui->buttonZapros->setEnabled(true);
    ui->tePseudonimInfo->setEnabled(true);
    ui->teRankingInfo->setEnabled(true);
    ui->tePseudonim->setEnabled(true);
    ui->teRanking->setEnabled(true);
    ui->tePseudonim->setText(biezacyUzytkownik->nick);
    ui->teRanking->setText(QString::number(biezacyUzytkownik->ranking));
}

// OPCJA ZACHOWANIA ROZGRYWKI I JEJ KONTYNUACJA
