#include "plansza.h"

#include <QGridLayout>
#include <QButtonGroup>

Plansza::Plansza(QWidget *parent) : QWidget(parent)
{
    QGridLayout* mainLayout = new QGridLayout(this);
    int size = 50;
    QButtonGroup *bg = new QButtonGroup();
    pola = new QPushButton*[64];
    for(int i=0;i<64;i++)
    {
        pola[i] = new QPushButton();//pola[i/8][i%8]);
        pola[i]->setFixedWidth(size);
        pola[i]->setFixedHeight(size);
        pola[i]->setAutoFillBackground(true);
        pola[i]->setIconSize(QSize(size-10,size-10));
        if(i%2 == (i/8)%2 )
            pola[i]->setStyleSheet("background-color: rgb(255, 255, 255); color: rgb(0, 0, 0)");
        else
            pola[i]->setStyleSheet("background-color: rgb(100, 100, 100); color: rgb(255, 255, 255)");
        mainLayout->addWidget(pola[i],i/8+1,i%8+1);
        bg->addButton(pola[i],i);
    }
    connect(bg,SIGNAL(buttonClicked(int)),this,SLOT(wcisnijPole(int)));

    iconEmpty = new QIcon();

    this->setLayout(mainLayout);
}

void Plansza::wcisnijPole(int nrPola)
{
    emit WcisnietoPole(nrPola);
}

void Plansza::Podswietl(QVector<int> polaPodsw)
{
    // Pierw odswietl wszystkie, potem poswietl nowe
    for(int i = 0; i < 64; i++)
    {
        if(i%2 == (i/8)%2 )
            pola[i]->setStyleSheet("background-color: rgb(255, 255, 255); color: rgb(0, 0, 0)");
        else
            pola[i]->setStyleSheet("background-color: rgb(100, 100, 100); color: rgb(255, 255, 255)");
    }
    if( polaPodsw.size() > 0 )
    {
    // Na zerowej pozycji bedzie klikniety pionek
    int pion = polaPodsw[0];
    if(pion%2 == (pion/8)%2 )
        pola[pion]->setStyleSheet("background-color: rgb(100, 255, 150); color: rgb(0, 0, 0)");
    else
        pola[pion]->setStyleSheet("background-color: rgb(80, 200, 130); color: rgb(255, 255, 255)");
    // Podswietlamy reszte pol
    for(int p = 1; p < polaPodsw.size(); p++)
    {
        int i = polaPodsw[p];
        if(i%2 == (i/8)%2 )
            pola[i]->setStyleSheet("background-color: rgb(100, 155, 255); color: rgb(0, 0, 0)");
        else
            pola[i]->setStyleSheet("background-color: rgb(80, 130, 200); color: rgb(255, 255, 255)");
    }
    }
}

void Plansza::UsunFigureZPola(int nrPola)
{
    pola[nrPola]->setIcon(*iconEmpty);
}

void Plansza::DodajFigureNaPole(int nrPola, QIcon *figura)
{
    pola[nrPola]->setIcon(*figura);
}

void Plansza::Reset()
{
    for(int i = 0; i < 64; i++)
    {
        if(i%2 == (i/8)%2 )
            pola[i]->setStyleSheet("background-color: rgb(255, 255, 255); color: rgb(0, 0, 0)");
        else
            pola[i]->setStyleSheet("background-color: rgb(100, 100, 100); color: rgb(255, 255, 255)");
        pola[i]->setIcon(*iconEmpty);
    }
}

Plansza::~Plansza()
{
    delete iconEmpty;
    delete[] pola;
}

