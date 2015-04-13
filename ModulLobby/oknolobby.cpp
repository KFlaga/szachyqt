#include "ModulLobby/oknolobby.h"
#include "ui_oknolobby.h"
#include "ModulLobby/oknologowania.h"
#include "ModulLobby/dialogopcjelokalniegracz.h"
#include "ModulLobby/dialogopcjelokalniesi.h"
#include "ModulLobby/dialogpodajnickgracza.h"
#include <QMessageBox>

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
    // Żadanie o wylogowanie
    // właście nie żądanie, a jedynie komunikat
    // jeśli serwer nie otrzyma go, to znaczy, że
    // nie ma połączenia, czyli z automatu wyloguje
    // Choć trzeba rozważyć możliwość zgubienia wiadmości
    // bądź chwilowego rozłączenia, tak więc może jednak
    // czekać na potwierdzenie bądź timeout
    czy_zalogowano = false;
    aktualizujInterfejs();
}

void OknoLobby::zaloguj()
{
    // emisja sygnału, ze żądanie zalogowania
    // oczekiwanie na otrzymanie sygnału, ze zalogowana bądź nie
    // opcja -> uruchomienie zalogowania stąd
    OknoLogowania* oknoLog = new OknoLogowania(this);
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
    wyslijWiadomosc();
}

void OknoLobby::zaprosGracza()
{
    // Pojawia się dialog z wpisaniem nicku gracza do zaproszenia
    // Gracz otrzymuje zaproszenie -> tak więc trzea dodać system
    // otrzymywania zaproszeń

    DialogPodajNickGracza* dialogNick = new DialogPodajNickGracza(this);
    int result = dialogNick->exec();
    if( result == QDialog::Accepted )
    {
        // znajdz gracza
        if( false ) // if( znalazlo gracza )
        {
            if( false ) // if( jest w grze )
            {
                // powiodom ze w grze
            }
            // wyslij zaproszenie
        }
        else
        {
            QMessageBox* mbNieZnalazlo = new QMessageBox(this);
            mbNieZnalazlo->setWindowTitle("Niepowodzenie");
            mbNieZnalazlo->setText("Nie znaleziono gracza o podanym pseudonimie");
            mbNieZnalazlo->setStandardButtons(QMessageBox::Ok);
            mbNieZnalazlo->exec();
        }
    }
}

void OknoLobby::otrzymanoZaproszenie(Uzytkownik* nadawca)
{
    // stworzyc nowy dialog

    // Opcje: anulowanie zaproszenia i odrzucenie zaproszenia
}

void OknoLobby::wyslijWiadomosc()
{
    KomunikatorLobbySerwer::WynikWyslania res = komunikator->wyslijWiadomosc();
    if( res == KomunikatorLobbySerwer::PrzekroczonoCzas ) // podzielic to na przekroczenie i na anulowanie
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
        this->statusBar()->showMessage("Komunikator jest zajety, 1000");
    }
    else
    {
        this->statusBar()->showMessage("Pomyślnie skomunikowano z serwerem, 1000");
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
