#include "szachyapp.h"
#include <QApplication>

SzachyApp::SzachyApp()
{
    biezacyUzytkownik = new Uzytkownik();
    przeciwnik = new Uzytkownik();
}

SzachyApp::~SzachyApp()
{

}

void SzachyApp::Run()
{
    lacze = new Klient();
    lobby = new OknoLobby();
    lobby->ustawUzytkownika(biezacyUzytkownik);
    qRegisterMetaType<Opcje>("Opcje");
    connect(lobby, SIGNAL(graLokalnie(Opcje*)), this, SLOT(graLokalnie(Opcje*)));
    connect(lobby, SIGNAL(sygZalogowano()), this, SLOT(zalogowano()));
    lobby->podlaczLacze(lacze);
    lobby->show();
}

void SzachyApp::zalogowano()
{

}

void SzachyApp::graLokalnie(Opcje* opts)
{
    oknoGry = new OknoGry();
    silnik = new Silnik();

    // Podlaczanie sie zaczyna
    connect((QObject*)silnik, SIGNAL(PodswietlicPola(QVector<int>)), oknoGry->WezPlansze(), SLOT(Podswietl(QVector<int>)));
    connect((QObject*)silnik, SIGNAL(DodanoFigureNaPole(int,QIcon*)), oknoGry->WezPlansze(), SLOT(DodajFigureNaPole(int,QIcon*)));
    connect((QObject*)silnik, SIGNAL(UsunietoFigureZPola(int)), oknoGry->WezPlansze(), SLOT(UsunFigureZPola(int)));
    connect(oknoGry->WezPlansze(), SIGNAL(WcisnietoPole(int)), (QObject*)silnik, SLOT(PoleWcisniete(int)));
    connect((QObject*)silnik, SIGNAL(WykonanoRuch(int)), oknoGry, SLOT(WykonanoRuch(int)));
    connect(oknoGry, SIGNAL(zamknietoOkno()), this, SLOT(koniecGry()));

    oknoGry->NowaGra(opts);
    silnik->NowaGra(opts);

    lobby->hide();
    oknoGry->show();
}

void SzachyApp::koniecGry()
{
    oknoGry->hide();
    oknoGry->close();
    lobby->show();

    delete oknoGry;
    delete silnik;
}
