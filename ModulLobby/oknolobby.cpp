#include "ModulLobby/oknolobby.h"
#include "ui_oknolobby.h"
#include "ModulLobby/oknologowania.h"
#include "ModulLobby/dialogopcjelokalniegracz.h"
#include "ModulLobby/dialogopcjelokalniesi.h"

OknoLobby::OknoLobby(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::OknoLobby)
{
    ui->setupUi(this);

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
}

void OknoLobby::zaloguj()
{
    // emisja sygnału, ze żądanie zalogowania
    // oczekiwanie na otrzymanie sygnału, ze zalogowana bądź nie
    // opcja -> uruchomienie zalogowania stąd
    OknoLogowania* oknoLog = new OknoLogowania(this);
    qRegisterMetaType<Uzytkownik>("Uzytkownik");
    connect(oknoLog, SIGNAL(zalogowano(Uzytkownik*)), this, SLOT(zalogowano(Uzytkownik*)));
    oknoLog->exec();
}

void OknoLobby::zalogowano(Uzytkownik* uzyt)
{
    biezacyUzytkownik = uzyt;
    emit sygZalogowano(uzyt);
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
}

void OknoLobby::zaprosGracza()
{
    // Pojawia się dialog z wpisaniem nicku gracza do zaproszenia
    // Gracz otrzymuje zaproszenie -> tak więc trzea dodać system
    // otrzymywania zaproszeń
    // Zapraszanie z listy zalogowanch będzie odbywało się poprzez
    // kliknięcie prawym na listę i zaproś bądź też
    // pod listą damy dodatkowy przycisk
}

// OPCJA ZACHOWANIA ROZGRYWKI I JEJ KONTYNUACJA
