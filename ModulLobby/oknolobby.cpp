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

QRegExp znakiZarezerwowane("[-,;:'\"\\\\ ]");

OknoLobby::OknoLobby(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::OknoLobby)
{
    ui->setupUi(this);
    czy_zalogowano = false;
    this->statusBar()->showMessage("Nie ma połączenia z serwerem");
    komunikator = new KomunikatorLobbySerwer(this);
    czyJestPoloczenie = NULL;

    Uzytkownik u;
    u.nick = "test";
    u.ranking = 999;
    ui->lista->dodajUzytkownika(u);
    ui->lista->dodajUzytkownika(u);

    connect(ui->lista, SIGNAL(zaproszono(QString)), this, SLOT(zaprosGracza(QString)));
    connect(ui->lista, SIGNAL(odswiez()), this, SLOT(zadajListyUzytkownikow()));

    timerOdswiezListe.setInterval(10000);
    timerOdswiezListe.setTimerType(Qt::VeryCoarseTimer);
    timerOdswiezListe.setSingleShot(true);
  //  connect(&timerOdswiezListe, SIGNAL(timeout()), this, SLOT(zadajListyUzytkownikow()));
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
    zadajListyUzytkownikow();
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
    Opcje* opts = new Opcje();
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
    wyslijWiadomosc(zaproszenie);

    if( zaproszenie->czyZaproszenieWyslane )
    {
        oczekujNaOdpowiedz();
    }
    else if( zaproszenie->czyPoprawnieOdebrane() )
    {
        wyswietlInformacje("Niepowodzenie", "Nie wyslano zaproszenia: "
                            + zaproszenie->powodNiepowodzenia);
    }

    delete zaproszenie;
}

void OknoLobby::oczekujNaOdpowiedz()
{
    oczekiwanieNaOdpowiedz = true;
    zaproszenieOdrzucone = false;
    powodzeniePojedynku = false;
    PopupOczekiwanieNaSerwer* oczekiwanie = new PopupOczekiwanieNaSerwer(this);
    oczekiwanie->setWindowModality(Qt::WindowModal);
    oczekiwanie->ustawTekst("Oczekiwanie na odpowiedź");
    oczekiwanie->show();
    QTimer::singleShot(40000,this, SLOT(anulujPojedynek()));

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
        // ZACZNIJ POJEDYNEK - czyli ew. czyszczenie czego trzeba
        // np. brak zadan o odswiezenie listy userow
    }
    else
    {
        wyswietlInformacje("Niepowodzenie", "Nie udało się stworzyć gry");
    }
    delete oczekiwanie;
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
    komunikator->wyslijWiadomosc(wiadomosc);
    delete wiadomosc;

    if(result == QDialog::Accepted)
    {
        PopupOczekiwanieNaSerwer* oczekiwanie = new PopupOczekiwanieNaSerwer(this);
        oczekiwanie->open();
        QTimer::singleShot(5000, this, SLOT(anulujPojedynek()));

        while (oczekiwanieNaOdpowiedz)
        {
             qApp->processEvents(QEventLoop::AllEvents,200);
        }
        oczekiwanie->close();
        if( powodzeniePojedynku )
        {
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

void OknoLobby::zacznijPojedynek(QString r)
{
   ustawStatus("IN: zacznij pojedynek", 2000);
   oczekiwanieNaOdpowiedz = false;
   powodzeniePojedynku = true;
   zaproszenieOdrzucone = false;
    // biezacyUzytkownik->status = Rozgrywka;
   // kl->wyslijRuch("ruch:1-B2-B4:200.");
   Opcje* opts = new Opcje();
   opts->CzyGraAI = false;
   QStringList t = r.split("-");  // 0 - przeciwnik 2 - czas
   opts->MaxCzas = t[2].toInt();

   if(getUzytkownik()->nick == t[1])
   {
       opts->Tura = 0;
   }
   else
   {
       opts->Tura = 1;
   }
   opts->czyPrzezSiec = true;
   opts->przciwnik = t[0];
   opts->klient = kl;
   emit graSieciowa(opts);

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

void OknoLobby::odebranoRuch(QString ruch)
{
    QMessageBox::information(this,"dsa1111111111111111111111d",ruch);
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
    connect(lacze, SIGNAL(otrzymanoZaproszenie(QString)), this, SLOT(otrzymanoZaproszenie(QString)));
    connect(lacze, SIGNAL(zacznijPojedynek(QString)), this, SLOT(zacznijPojedynek(QString)));
    connect(lacze, SIGNAL(anulujPojedynek(QString)), this, SLOT(anulujPojedynek(QString)));
    connect(lacze, SIGNAL(odmowaPojedynku(QString)), this, SLOT(odmowaPojedynku(QString)));


    kl = lacze;
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
        wyslijWiadomosc(wiadomosc);
        if( wiadomosc->czyPoprawnieOdebrane() )
        {
            ustawStatus("Nowa lista",2000);
            ui->lista->czysc();
            for(int u = 0; u < wiadomosc->uzytkownicy.size(); u++)
            {
                ui->lista->dodajUzytkownika(wiadomosc->uzytkownicy[u]);
            }
        }
        delete wiadomosc;
    }
    timerOdswiezListe.start();
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
