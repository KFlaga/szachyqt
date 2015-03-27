#include "szachyapp.h"


SzachyApp::SzachyApp()
{
    opts = new Opcje();
    oknoLogowania = new OknoLogowania();
    oknoOpcji = new OknoOpcji();
    oknoGry = new OknoGry();
    silnik = new Silnik();
    opts->CzyGraAI = false;
    opts->MaxCzas = 0;
    opts->PoziomTrudnosci = 0;
}

SzachyApp::~SzachyApp()
{
    delete silnik;
    delete oknoGry;
    delete oknoLogowania;
    delete oknoOpcji;
}

void SzachyApp::Run()
{
    connect(oknoLogowania,SIGNAL(logowanieZakonczone()),this, SLOT(zalogowano()));
    oknoLogowania->show();
}

void SzachyApp::zalogowano()
{
    disconnect(this, SLOT(zalogowano()));
    connect(oknoOpcji, SIGNAL(noweOpcje()), this, SLOT(noweOpcje()));
    connect(oknoOpcji, SIGNAL(stareOpcje()), this, SLOT(stareOpcje()));
    oknoLogowania->hide();
    oknoOpcji->show();
}

void SzachyApp::noweOpcje()
{
    opts->CzyGraAI = oknoOpcji->gra_z_kompem();
    opts->MaxCzas = oknoOpcji->max_czas();
    opts->PoziomTrudnosci = oknoOpcji->poziom_trudnosci();
    wybranoOpcje();
}

void SzachyApp::stareOpcje()
{
    wybranoOpcje();
}

void SzachyApp::wybranoOpcje()
{
    // Podlaczanie sie zaczyna
    connect((QObject*)silnik, SIGNAL(PodswietlicPola(QVector<int>)), oknoGry->WezPlansze(), SLOT(Podswietl(QVector<int>)));
    connect((QObject*)silnik, SIGNAL(DodanoFigureNaPole(int,QIcon*)), oknoGry->WezPlansze(), SLOT(DodajFigureNaPole(int,QIcon*)));
    connect((QObject*)silnik, SIGNAL(UsunietoFigureZPola(int)), oknoGry->WezPlansze(), SLOT(UsunFigureZPola(int)));
    connect(oknoGry->WezPlansze(), SIGNAL(WcisnietoPole(int)), (QObject*)silnik, SLOT(PoleWcisniete(int)));
    connect((QObject*)silnik, SIGNAL(WykonanoRuch()), oknoGry, SLOT(WykonanoRuch()));

    oknoGry->NowaGra(opts);
    silnik->NowaGra(opts);

    oknoOpcji->hide();
    oknoGry->show();
}
