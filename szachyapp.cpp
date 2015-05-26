#include "szachyapp.h"
#include <QApplication>

SzachyApp::SzachyApp()
{
    biezacyUzytkownik = new Uzytkownik();
    przeciwnik = new Uzytkownik();
}

SzachyApp::~SzachyApp()
{
    if( lacze != NULL)
        delete lacze;
}

void SzachyApp::Run()
{
    lobby = new OknoLobby();
    lacze = new Klient();
    connect(lacze,SIGNAL(poloczono()), this, SLOT(poloczonoZSerwerem()));
    connect(lacze,SIGNAL(rozloczono()), this, SLOT(rozloczonoZSerwerem()));
    connect(lacze,SIGNAL(niepowodzeniePoloczenia(int)), this, SLOT(niepowodzeniePoloczeniaZSerwerem(int)));

    lobby->podlaczLacze(lacze);
    lacze->polacz();

    lobby->ustawUzytkownika(biezacyUzytkownik);

    qRegisterMetaType<Opcje>("Opcje");
    connect(lobby, SIGNAL(graLokalnie(Opcje*)), this, SLOT(graLokalnie(Opcje*)));
    connect(lobby, SIGNAL(sygZalogowano()), this, SLOT(zalogowano()));
   // connect(lobby,SIGNAL(graSieciowa(Opcje*)),this,SLOT(graSieciowa(Opcje*)));
    connect(lobby,SIGNAL(graSieciowa(Opcje*)),this,SLOT(graSieciowa(Opcje*)),Qt::QueuedConnection);

    connect(lobby,SIGNAL(szukajPrzeciwnika(QString)),lacze,SLOT(wyslijInfOSzukaniu(QString)));

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

//na razie takie samo jako graLokalna
void SzachyApp::graSieciowa(Opcje * opts)
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
    connect(lacze,SIGNAL(odebranoRuch(QString)),(QObject*)silnik,SLOT(odebranoRuch(QString)));
    connect((QObject*)silnik,SIGNAL(wyslijRuch(QString)), lacze, SLOT(wyslijRuch(QString)));
    connect((QObject*)silnik,SIGNAL(wyslijWynik(QString)),lacze,SLOT(wyslijWynik(QString)));
    connect(lacze,SIGNAL(wygranaPrzezWalkower()),(QObject*)silnik,SLOT(zakonczPrzezWalkower()));
    connect((QObject*)silnik,SIGNAL(przegrajPrzezWalkower(QString)),lacze,SLOT(wyslijWynik(QString)));
    connect((QObject*)silnik,SIGNAL(zmienStatusNaWolny(QString)),lacze,SLOT(wyslijWynik(QString)));

    oknoGry->NowaGra(opts);
    silnik->NowaGra(opts);

    lobby->hide();
    oknoGry->show();
}


void SzachyApp::koniecGry()
{
    oknoGry->hide();
    oknoGry->close();
    lobby->koniecGry();
    lobby->show();

    delete oknoGry;
    delete silnik;
}

void SzachyApp::poloczonoZSerwerem()
{

}

void SzachyApp::rozloczonoZSerwerem()
{

}

void SzachyApp::niepowodzeniePoloczeniaZSerwerem(int)
{

}
