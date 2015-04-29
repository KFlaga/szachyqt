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

OknoLobby::OknoLobby(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::OknoLobby)
{
    ui->setupUi(this);
    czy_zalogowano = false;
    this->statusBar()->showMessage("Nie ma połączenia z serwerem");
    komunikator = new KomunikatorLobbySerwer(this);
    czyJestPoloczenie = NULL;
}

OknoLobby::~OknoLobby()
{
    delete ui;
}

void OknoLobby::closeEvent(QCloseEvent * ce)
{
    qApp->closeAllWindows();
    qApp->exit();
}

void OknoLobby::wyloguj()
{
    if( czyJestPoloczenie == true )
    {
        WiadomoscWyloguj* wiadomosc = new WiadomoscWyloguj();
        komunikator->wyslijWiadomosc(wiadomosc);
        delete wiadomosc;
    }

    czy_zalogowano = false;
    aktualizujInterfejs();
}

void OknoLobby::zaloguj()
{
    OknoLogowania* oknoLog = new OknoLogowania(this, komunikator);
    oknoLog->ustawUzytkownika(biezacyUzytkownik);
    connect(oknoLog, SIGNAL(zalogowano()), this, SLOT(zalogowano()));
    oknoLog->exec();
    delete oknoLog;
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
    if( czyJestPoloczenie == false )
    {
        wyswietlInformacje("Brak Połączenia", "Nie ma połączenia z serwerem");
        return;
    }

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
            // czekamy na ustawienie zmiennych: pojedynek lub niezgoda
            oczekiwanieNaOdpowiedz = true;
            zaproszenieOdrzucone = false;
            powodzeniePojedynku = false;
            PopupOczekiwanieNaSerwer* oczekiwanie = new PopupOczekiwanieNaSerwer(this);
            oczekiwanie->setWindowModality(Qt::WindowModal);
            oczekiwanie->ustawTekst("Oczekiwanie na odpowiedź");
            oczekiwanie->show();
            QTimer::singleShot(40000,this, SLOT(anulujPojedynek()));
            //wyswietlInformacje("zaproszenie wyslane", "");
            while (oczekiwanieNaOdpowiedz)
            {
                 qApp->processEvents(QEventLoop::AllEvents,200);
            }
            oczekiwanie->hide();
            if( zaproszenieOdrzucone )
            {
                wyswietlInformacje("Odrzucono", "Odrzucono zaproszenie do gry");
            }
            else if( powodzeniePojedynku )
            {
                wyswietlInformacje("","Pojedynek!");
                // ZACZNIJ POJEDYNEK - czyli ew. czyszczenie czego trzeba
                // np. brak zadan o odswiezenie listy userow
            }
            else
            {
                wyswietlInformacje("Niepowodzenie", "Nie udało się stworzyć gry");
            }
            delete oczekiwanie;
        }
        else if( zaproszenie->czyPoprawnieOdebrane() )
        {
            wyswietlInformacje("Niepowodzenie", "Nie wyslano zaproszenia: "
                                + zaproszenie->powodNiepowodzenia);
        }

        delete zaproszenie;
    }
}

void OknoLobby::otrzymanoZaproszenie(QString& nadawca)
{
    DialogOtrzymanoZaproszenie* dialogOdpowiedz = new DialogOtrzymanoZaproszenie(this);
    QStringList dane = nadawca.split('-');
    dialogOdpowiedz->ustawNick(dane[0]);
    dialogOdpowiedz->ustawCzas(dane[1]);
    WiadomoscOdpowiedzNaZaproszenie* wiadomosc = new WiadomoscOdpowiedzNaZaproszenie();
    wiadomosc->nick = dane[0];
    wiadomosc->czas = dane[1];

    int result = dialogOdpowiedz->exec();
    if(result == QDialog::Accepted)
        wiadomosc->czyZgoda = true;
    else
        wiadomosc->czyZgoda = false;

    oczekiwanieNaOdpowiedz = true;
    powodzeniePojedynku = false;
    komunikator->wyslijWiadomosc(wiadomosc);
    delete wiadomosc;

    if(result == QDialog::Accepted)
    {
        PopupOczekiwanieNaSerwer* oczekiwanie = new PopupOczekiwanieNaSerwer(this);
//        oczekiwanie->setWindowModality(Qt::WindowModal);
//        oczekiwanie->show();
//        QTimer::singleShot(5000, this, SLOT(anulujPojedynek()));

        while (oczekiwanieNaOdpowiedz)
        {
             qApp->processEvents(QEventLoop::AllEvents,200);
        }
//        oczekiwanie->hide();
        if( powodzeniePojedynku )
        {
            wyswietlInformacje("","Pojedynek!");
            // ZACZNIJ POJEDYNEK - czyli ew. czyszczenie czego trzeba
            // np. brak zadan o odswiezenie listy userow
        }
        else
        {
            wyswietlInformacje("Niepowodzenie", "Nie udało się stworzyć gry");
        }
        delete oczekiwanie;
    }
    else
        oczekiwanieNaOdpowiedz = false;

}

void OknoLobby::zacznijPojedynek(QString&)
{
    ustawStatus("IN: zacznij pojedynek", 2000);
   oczekiwanieNaOdpowiedz = false;
   powodzeniePojedynku = true;
   zaproszenieOdrzucone = false;
    // biezacyUzytkownik->status = Rozgrywka;
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

void OknoLobby::anulujPojedynek(QString&)
{
     ustawStatus("IN: anuluj pojedynek", 2000);
    oczekiwanieNaOdpowiedz = false;
    powodzeniePojedynku = false;
    zaproszenieOdrzucone = false;
}

void OknoLobby::odmowaPojedynku(QString&)
{
     ustawStatus("IN: odmowa pojedynku", 2000);
    oczekiwanieNaOdpowiedz = false;
    powodzeniePojedynku = false;
    zaproszenieOdrzucone = true;
}

void OknoLobby::wyslijWiadomosc(Wiadomosc* wiadomosc, QString popupTekst)
{
    KomunikatorLobbySerwer::WynikWyslania res =
            komunikator->wyslijWiadomoscZeZwrotem(wiadomosc,popupTekst);
    if( res == KomunikatorLobbySerwer::PrzekroczonoCzas )
    {
        wyswietlInformacje("Brak Odpowiedzi", "Przekroczono czas oczekiwania na odpowiedź serwera");
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

void OknoLobby::wyswietlInformacje(const QString &tytul, const QString &info)
{
    QMessageBox mb(this);
    mb.setText(info);
    mb.setWindowTitle(tytul);
    mb.setStandardButtons(QMessageBox::Ok);
    mb.exec();
}

void OknoLobby::podlaczLacze(Klient *lacze)
{
    connect(komunikator, SIGNAL(nadajWiadomosc(const QString&,IKomunikator*)),
            lacze, SLOT(wyslijWiadomosc(const QString&,IKomunikator*)));
    connect(lacze, SIGNAL(poloczono()), this, SLOT(poloczonoZSerwerem()));
    connect(lacze, SIGNAL(rozloczono()), this, SLOT(rozloczonoZSerwerem()));
    connect(lacze, SIGNAL(niepowodzeniePoloczenia(int)), this, SLOT(nieMoznaPolaczycZSerwerem(int)));
    connect(lacze, SIGNAL(otrzymanoZaproszenie(QString&)), this, SLOT(otrzymanoZaproszenie(QString&)));
    connect(lacze, SIGNAL(zacznijPojedynek(QString&)), this, SLOT(zacznijPojedynek(QString&)));
    connect(lacze, SIGNAL(anulujPojedynek(QString&)), this, SLOT(anulujPojedynek(QString&)));
    connect(lacze, SIGNAL(odmowaPojedynku(QString&)), this, SLOT(odmowaPojedynku(QString&)));
}

void OknoLobby::poloczonoZSerwerem()
{
    ui->statusbar->showMessage("Połączono z serwerem", 5000);
    czyJestPoloczenie = true;
}

void OknoLobby::rozloczonoZSerwerem()
{
    ui->statusbar->showMessage("Rozłączono z serwerem", 5000);
    czyJestPoloczenie = false;
    czy_zalogowano = false;
    biezacyUzytkownik->nick = "";
    aktualizujInterfejs();
    // Po zerwaniu poloczenia i ponownym przyworceniu sprobuk zalogowac
    // tymi samymi danymi co ostatnio ( tak wiec trzeba trzymac dane
    // najlepiej jako wynik logowania )
}

void OknoLobby::nieMoznaPolaczycZSerwerem(int)
{
    ui->statusbar->showMessage("Nie można nawiązać połączenia z serwerem", 5000);
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
