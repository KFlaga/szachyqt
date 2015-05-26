#include "ModulLobby/oknolobby.h"
#include "ui_oknolobby.h"
#include "ModulLobby/oknologowania.h"
#include "ModulLobby/dialogopcjelokalniegracz.h"
#include "ModulLobby/dialogopcjelokalniesi.h"
#include "ModulLobby/dialogwyslijzaproszenie.h"
#include "ModulLobby/dialogotrzymanozaproszenie.h"
#include <QMessageBox>
#include "Wiadomosci/wiadomoscwyloguj.h"
#include "Wiadomosci/wiadomosczaproszenie.h"
#include "Wiadomosci/wiadomoscodpowiedznazaproszenie.h"
#include "popupoczekiwanienaserwer.h"
#include "listauzytkownikow.h"
#include "Wiadomosci/wiadomosclistauzytkownikow.h"
#include "komunikatorlobbyserwer.h"

QRegExp znakiZarezerwowane("[-,;:'\"\\\\ ]");

OknoLobby::OknoLobby(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::OknoLobby)
{
    ui->setupUi(this);
    czy_zalogowano = false;
    this->statusBar()->showMessage("Nie ma połączenia z serwerem");
    czyJestPoloczenie = false;
    opts = new Opcje();

    oczekiwanie = new PopupOczekiwanieNaSerwer(this);
    oczekiwanie->setWindowModality(Qt::WindowModal);

    connect(ui->lista, SIGNAL(zaproszono(QString)), this, SLOT(zaprosGracza(QString)));

    timerOdswiezListe.setInterval(10000);
    timerOdswiezListe.setTimerType(Qt::VeryCoarseTimer);
    timerOdswiezListe.setSingleShot(true);
    connect(&timerOdswiezListe, SIGNAL(timeout()), this, SLOT(zadajListyUzytkownikow()));

    timerOczekiwanie = new QTimer(this);
    timerOczekiwanie->setSingleShot(true);
    connect(timerOczekiwanie, SIGNAL(timeout()), this, SLOT(anulujPojedynek()));
}

OknoLobby::~OknoLobby()
{
    delete ui;
    delete oczekiwanie;
}

void OknoLobby::closeEvent(QCloseEvent * ce)
{
    qApp->closeAllWindows();
}

void OknoLobby::wyloguj()
{
    if( czyJestPoloczenie == true )
    {
        KomunikatorLobbySerwer* komunikator = new KomunikatorLobbySerwer(this);
        connect(komunikator, SIGNAL(nadajWiadomosc(QString,IKomunikator*)),
                this, SIGNAL(nadajWiadomosc(QString,IKomunikator*)));
        WiadomoscWyloguj* wiadomosc = new WiadomoscWyloguj();
        komunikator->wyslijWiadomosc(wiadomosc);
        delete wiadomosc;
    }

    czy_zalogowano = false;
    aktualizujInterfejs();
}

void OknoLobby::zaloguj()
{
    OknoLogowania* oknoLog = new OknoLogowania(this);
    oknoLog->ustawUzytkownika(biezacyUzytkownik);
    connect(oknoLog, SIGNAL(zalogowano()), this, SLOT(zalogowano()));
    connect(oknoLog, SIGNAL(nadajWiadomosc(const QString&, IKomunikator*)),
            this, SIGNAL(nadajWiadomosc(QString,IKomunikator*)));
    oknoLog->exec();
    delete oknoLog;
}

void OknoLobby::zalogowano()
{
    czy_zalogowano = true;
    aktualizujInterfejs();
    zadajListyUzytkownikow();
    emit sygZalogowano();
}

void OknoLobby::zagrajLokalnieSI()
{
    DialogOpcjeLokalnieSI* dialogOpts = new DialogOpcjeLokalnieSI(this);
    int result = dialogOpts->exec();
    if( result == QDialog::Rejected )
        return;
    opts->CzyGraAI = true;
    opts->MaxCzas = dialogOpts->czas;
    opts->PoziomTrudnosci = dialogOpts->poziom;
    opts->czyPrzezSiec = false;

    delete dialogOpts;
    emit graLokalnie(opts);
}

void OknoLobby::zagrajLokalnieGracz()
{
    DialogOpcjeLokalnieGracz* dialogOpts = new DialogOpcjeLokalnieGracz(this);
    int result = dialogOpts->exec();
    if( result == QDialog::Rejected )
        return;
    opts->CzyGraAI = false;
    opts->MaxCzas = dialogOpts->czas;
    opts->czyPrzezSiec = false;

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

void OknoLobby::zaprosGracza(QString nick)
{
    if( czyJestPoloczenie == false )
    {
        wyswietlInformacje("Brak Połączenia", "Nie ma połączenia z serwerem");
        return;
    }

    DialogWyslijZaproszenie* dialogZaproszenie = new DialogWyslijZaproszenie(this);
    dialogZaproszenie->podstawNick(nick);
    int result = dialogZaproszenie->exec();
    if( result == QDialog::Accepted )
    {
        wyslijZaproszenie(dialogZaproszenie->wezNick(), dialogZaproszenie->wezCzas());
    }
}

void OknoLobby::wyslijZaproszenie(const QString& nick, int czas)
{
    WiadomoscZaproszenie* zaproszenie = new WiadomoscZaproszenie();
    zaproszenie->nick = nick;
    zaproszenie->czas = czas;
    KomunikatorLobbySerwer* komunikator = new KomunikatorLobbySerwer(this);
    connect(komunikator, SIGNAL(nadajWiadomosc(QString,IKomunikator*)),
            this, SIGNAL(nadajWiadomosc(QString,IKomunikator*)));
    connect(komunikator, SIGNAL(odebranoZwrot(Wiadomosc*,bool)),
            this, SLOT(sprawdzZwrotZaproszenia(Wiadomosc*,bool)));
    oczekiwanie->show();
    komunikator->wyslijWiadomosc(zaproszenie, true);
}

void OknoLobby::sprawdzZwrotZaproszenia(Wiadomosc* wiadomosc, bool czyAnulowano)
{
    sender()->deleteLater();
    WiadomoscZaproszenie* zaproszenie = (WiadomoscZaproszenie*)wiadomosc;
    oczekiwanie->hide();
    if( !czyAnulowano )
    {
        if( zaproszenie->czyZaproszenieWyslane )
        {
            oczekujNaOdpowiedz();
        }
        else if( zaproszenie->czyPoprawnieOdebrane() )
        {
            wyswietlInformacje("Niepowodzenie", "Nie wyslano zaproszenia: "
                               + zaproszenie->powodNiepowodzenia);
        }
    }
    else
    {
        wyswietlInformacje("Niepowodzenie", "Nie otrzymano odpowiedzi serwera");
    }
    delete zaproszenie;
}

void OknoLobby::oczekujNaOdpowiedz()
{
    oczekiwanieNaOdpowiedz = true;
    zaproszenieOdrzucone = false;
    powodzeniePojedynku = false;

    oczekiwanie->ustawTekst("Oczekiwanie na odpowiedź");
    oczekiwanie->show();
    timerOczekiwanie->start(30000);

    while (oczekiwanieNaOdpowiedz)
    {
         qApp->processEvents(QEventLoop::AllEvents,200);
    }
    oczekiwanie->hide();
    oczekiwanie->ustawTekst();
    if( zaproszenieOdrzucone )
    {
        wyswietlInformacje("Odrzucono", "Odrzucono zaproszenie do gry");
    }
    else if( powodzeniePojedynku )
    {
        // ZACZNIJ POJEDYNEK - czyli ew. czyszczenie czego trzeba
        // np. brak zadan o odswiezenie listy userow
        timerOdswiezListe.stop();
        emit graSieciowa(opts);
    }
    else
    {
        wyswietlInformacje("Niepowodzenie", "Nie udało się stworzyć gry");
    }
}

void OknoLobby::otrzymanoZaproszenie(QString nadawca)
{
    DialogOtrzymanoZaproszenie* dialogOdpowiedz = new DialogOtrzymanoZaproszenie(this);
    QStringList dane = nadawca.split('-');
    dialogOdpowiedz->ustawNick(dane[0]);
    dialogOdpowiedz->ustawCzas(dane[1]);

    connect(dialogOdpowiedz, SIGNAL(finished(int)), this, SLOT(odpowiedzNaZaproszenie(int)));
    dialogOdpowiedz->open();
}

void OknoLobby::odpowiedzNaZaproszenie(int result)
{
    ustawStatus("IN: odpowiedzNaZaproszenie", 3000);
    WiadomoscOdpowiedzNaZaproszenie* wiadomosc = new WiadomoscOdpowiedzNaZaproszenie();
    wiadomosc->nick = ((DialogOtrzymanoZaproszenie*)sender())->wezNick();
    wiadomosc->czas = ((DialogOtrzymanoZaproszenie*)sender())->wezCzas();
    sender()->deleteLater();

    if(result == QDialog::Accepted)
        wiadomosc->czyZgoda = true;
    else
        wiadomosc->czyZgoda = false;

    oczekiwanieNaOdpowiedz = true;
    powodzeniePojedynku = false;
    KomunikatorLobbySerwer* komunikator = new KomunikatorLobbySerwer(this);
    connect(komunikator, SIGNAL(nadajWiadomosc(QString,IKomunikator*)),
            this, SIGNAL(nadajWiadomosc(QString,IKomunikator*)));
    komunikator->wyslijWiadomosc(wiadomosc);
    delete wiadomosc;

    if(result == QDialog::Accepted)
    {
        oczekiwanie->show();
        timerOczekiwanie->start(5000);

        while (oczekiwanieNaOdpowiedz)
        {
             qApp->processEvents(QEventLoop::AllEvents,200);
        }
        oczekiwanie->hide();
        if( powodzeniePojedynku )
        {
            // ZACZNIJ POJEDYNEK - czyli ew. czyszczenie czego trzeba
            // np. brak zadan o odswiezenie listy userow
            timerOdswiezListe.stop();
            emit graSieciowa(opts);
        }
        else
        {
            wyswietlInformacje("Niepowodzenie", "Nie udało się stworzyć gry");
        }
    }
    else
        oczekiwanieNaOdpowiedz = false;
}

void OknoLobby::koniecGry()
{
    timerOdswiezListe.start();
}

void OknoLobby::zacznijPojedynek(QString wiad)
{
    ustawStatus("IN: zacznij pojedynek", 2000);
   oczekiwanieNaOdpowiedz = false;
   powodzeniePojedynku = true;
   zaproszenieOdrzucone = false;
   timerOczekiwanie->stop();

   opts->CzyGraAI = false;
   QStringList dane = wiad.split("-");  // 0 - przeciwnik 2 - czas
   opts->MaxCzas = dane[2].toInt();
   if(biezacyUzytkownik->nick == dane[1])
   {
       opts->Tura = 0;
   }
   else
   {
       opts->Tura = 1;
   }
   opts->czyPrzezSiec = true;
   opts->przciwnik = dane[0];
   aktualnyPrzeciwnik = dane[0];
}

void OknoLobby::anulujPojedynek()
{
    ustawStatus("IN: anuluj pojedynek timeout", 2000);
    if( oczekiwanieNaOdpowiedz == true )
    {
        oczekiwanieNaOdpowiedz = false;
        powodzeniePojedynku = false;
        zaproszenieOdrzucone = false;
    }
}

void OknoLobby::anulujPojedynek(QString)
{
    timerOczekiwanie->stop();
     ustawStatus("IN: anuluj pojedynek", 2000);
    oczekiwanieNaOdpowiedz = false;
    powodzeniePojedynku = false;
    zaproszenieOdrzucone = false;
}

void OknoLobby::odmowaPojedynku(QString)
{
     ustawStatus("IN: odmowa pojedynku", 2000);
    oczekiwanieNaOdpowiedz = false;
    powodzeniePojedynku = false;
    zaproszenieOdrzucone = true;
}

void OknoLobby::wyswietlInformacje(const QString &tytul, const QString &info)
{
    QMessageBox mb(this);
    mb.setText(info);
    mb.setWindowTitle(tytul);
    mb.setStandardButtons(QMessageBox::Ok);
    mb.exec();
}

// To w sumie powinno byc w SzachyApp i Lobby nie widzi Klienta
void OknoLobby::podlaczLacze(Klient *lacze)
{
    connect(this, SIGNAL(nadajWiadomosc(QString,IKomunikator*)),
            lacze, SLOT(wyslijWiadomosc(QString,IKomunikator*)));
    connect(lacze, SIGNAL(poloczono()), this, SLOT(poloczonoZSerwerem()));
    connect(lacze, SIGNAL(rozloczono()), this, SLOT(rozloczonoZSerwerem()));
    connect(lacze, SIGNAL(niepowodzeniePoloczenia(int)), this, SLOT(nieMoznaPolaczycZSerwerem(int)));
    connect(lacze, SIGNAL(otrzymanoZaproszenie(QString)), this, SLOT(otrzymanoZaproszenie(QString)));
    connect(lacze, SIGNAL(zacznijPojedynek(QString)), this, SLOT(zacznijPojedynek(QString)));
    connect(lacze, SIGNAL(anulujPojedynek(QString)), this, SLOT(anulujPojedynek(QString)));
    connect(lacze, SIGNAL(odmowaPojedynku(QString)), this, SLOT(odmowaPojedynku(QString)));

    connect(lacze,SIGNAL(nowyRanking(int)),this,SLOT(aktualizujRanking(int)));
}

void OknoLobby::poloczonoZSerwerem()
{
    ui->statusbar->showMessage("Połączono z serwerem", 5000);
    czyJestPoloczenie = true;
    timerOdswiezListe.start();
}

void OknoLobby::rozloczonoZSerwerem()
{
    ui->statusbar->showMessage("Rozłączono z serwerem", 5000);
    czyJestPoloczenie = false;
    czy_zalogowano = false;
    biezacyUzytkownik->nick = "";
    aktualizujInterfejs();
    timerOdswiezListe.stop();
    ui->lista->czysc();
    // Po zerwaniu poloczenia i ponownym przyworceniu sprobuk zalogowac
    // tymi samymi danymi co ostatnio ( tak wiec trzeba trzymac dane
    // najlepiej jako wynik logowania )
}

void OknoLobby::nieMoznaPolaczycZSerwerem(int)
{
    ui->statusbar->showMessage("Nie można nawiązać połączenia z serwerem", 5000);
}

void OknoLobby::zadajListyUzytkownikow()
{
    timerOdswiezListe.stop();
    if( czyJestPoloczenie == true )
    {
        ustawStatus("Zadanie listy",1000);
        WiadomoscListaUzytkownikow* wiadomosc = new WiadomoscListaUzytkownikow();
        KomunikatorLobbySerwer* komunikator = new KomunikatorLobbySerwer(this);
        connect(komunikator, SIGNAL(nadajWiadomosc(QString,IKomunikator*)),
                this, SIGNAL(nadajWiadomosc(QString,IKomunikator*)));
        connect(komunikator, SIGNAL(odebranoZwrot(Wiadomosc*,bool)),
                this, SLOT(aktualizujListeUzytkownikow(Wiadomosc*,bool)));
        komunikator->wyslijWiadomosc(wiadomosc, true);
    }
}

void OknoLobby::aktualizujListeUzytkownikow(Wiadomosc* wiadomosc, bool czyAnulowano)
{
    sender()->deleteLater();
    if( !czyAnulowano && wiadomosc->czyPoprawnieOdebrane() )
    {
        ustawStatus("Nowa lista",2000);
        ui->lista->czysc();
        for(int u = 0; u < ((WiadomoscListaUzytkownikow*)wiadomosc)->uzytkownicy.size(); u++)
        {
            ui->lista->dodajUzytkownika(((WiadomoscListaUzytkownikow*)wiadomosc)->uzytkownicy[u]);
        }
    }
    delete wiadomosc;

    timerOdswiezListe.start();
}

void OknoLobby::aktualizujRanking(int rank)
{
    this->biezacyUzytkownik->ranking = rank;
    ui->teRanking->setText(QString::number(biezacyUzytkownik->ranking));
}

void OknoLobby::aktualizujInterfejs()
{
    if(czy_zalogowano == false)
    {
        ui->buttonZaloguj->setEnabled(true);
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
    ui->buttonZaloguj->setEnabled(false);
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

void OknoLobby::ustawStatus(const QString &status, int czas)
{
    ui->statusbar->showMessage(status, czas);
}

// OPCJA ZACHOWANIA ROZGRYWKI I JEJ KONTYNUACJA
