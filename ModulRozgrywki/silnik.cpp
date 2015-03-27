#include "silnik.h"
#include <QVector>
#include "Figury/figura.h"
#include <QDebug>
#include <QPushButton>
Silnik::Silnik()
{
    kreator =  new KreatorFigur();
    for(int i = 0; i < 64; i++)
        pola[i] = -1;


    msgBox=new QMessageBox(QMessageBox::Question,"Promocja pionka","Wybierz figurę:");      //przygotowanie okna z promocją
    hetmanButton = msgBox->addButton("Hetman",QMessageBox::ActionRole);
    goniecButton = msgBox->addButton("Goniec", QMessageBox::ActionRole);
    skoczekButton = msgBox->addButton("Skoczek", QMessageBox::ActionRole);
    wiezaButton = msgBox->addButton("Wieza", QMessageBox::ActionRole);
}

Silnik::~Silnik()
{

}

void Silnik::NowaGra(Opcje* opts)
{
    // Stworz pionki i dla kazdego emituj sygnal
    // Kolejnosc dodawania -> od zera, czyli gora-lewo, dla czarnych
    // potem od konca, czyli dol-prawo, w prawa strone dla bialych
    for(int i = 0; i < 16; i++)
    {
        Figura* fig = kreator->StworzFigure(i,1);
        figury.push_back(fig);
        pola[i] = i;
        emit DodanoFigureNaPole(i,&(fig->ikona));
    }

    for(int i = 16; i < 32; i++)
    {
        Figura* fig = kreator->StworzFigure(63-i+16,0);
        figury.push_back(fig);
        pola[63-i+16] = i;
        emit DodanoFigureNaPole(63-i+16,&(fig->ikona));
    }

    zaznaczonePole = -1;
    aktualnyGracz = 0;
}

void Silnik::PoleWcisniete(int nrPola)
{
    static QVector<int> zaznaczonePola;
    if( zaznaczonePole == nrPola ) // Odznaczamy
    {
        zaznaczonePole = -1;
        zaznaczonePola.clear();
    }
    else if( zaznaczonePole == -1) // zadne pole nie zaznaczone
    {
        // jak zaznaczony pionek swoj to go zaznacz i podswietl ruchy
        if( pola[nrPola] != -1 && figury[pola[nrPola]]->strona == aktualnyGracz )
        {
            zaznaczonePola.clear();
            zaznaczonePola = figury[pola[nrPola]]->dostepneRuchy(pola, &figury);
            zaznaczonePola.push_front(nrPola);
            zaznaczonePole = nrPola;
        }
        // jak inne pola to nic nie rob
    }
    else // klikniete pole inne niz zaznaczona figura
    {
        // Pierw sprawdzenie czy klikniecie na mozliwy ruch
        if( zaznaczonePola.contains(nrPola) )
        {
            // potem czy jest tam pionek - jak jest to bedzie to pionek przeciwnika
            if( pola[nrPola] != -1 )
            {
                // pionek przeciwnika - zbij i odznacz
                ZbijPionek(zaznaczonePole, nrPola); // arg: atakujacy / atakowany
                zaznaczonePole = -1;
                zaznaczonePola.clear();
            }
            else
            {
                // jak nie ma pionka to rusz tam zaznaczony i odznacz
                RuszPionek(zaznaczonePole, nrPola);
                zaznaczonePole = -1;
                zaznaczonePola.clear();
            }
        }
        else
        {
            // Ruch nie dobry, to sprawdzamy czy jest tam nasz pionek i go zaznaczamy
            if( pola[nrPola] != -1 && figury[pola[nrPola]]->strona == aktualnyGracz )
            {
                zaznaczonePola.clear();
                zaznaczonePola = figury[pola[nrPola]]->dostepneRuchy(pola, &figury);
                zaznaczonePola.push_front(nrPola);
                zaznaczonePole = nrPola;
            }
            else // jak nie - odznaczamy wszystko
            {
                zaznaczonePole = -1;
                zaznaczonePola.clear();
            }
        }
    }

    emit PodswietlicPola(zaznaczonePola);
}

// Narazie bicie i ruch wygladaja analogicznie, ale pewnie sie to zmieni
void Silnik::ZbijPionek(int pozBijacego, int pozBitego)
{
    pola[pozBitego] = pola[pozBijacego];
    pola[pozBijacego] = -1;
    figury[pola[pozBitego]]->UstawPole(pozBitego);

    emit UsunietoFigureZPola(pozBitego);
    emit UsunietoFigureZPola(pozBijacego);
    emit DodanoFigureNaPole(pozBitego, &(figury[pola[pozBitego]]->ikona));


    if(figury[pola[pozBitego]]->typ==TPionek && figury[pola[pozBitego]]->strona==0 && pozBitego < 8) //jesli bialy pionek doszedl do konca planszy
    {
       Promocja(0,pozBitego);

    }

    if(figury[pola[pozBitego]]->typ==TPionek && figury[pola[pozBitego]]->strona==1 &&  pozBitego > 56) //jesli czaarny pionek doszedl do konca planszy
    {
        Promocja(1,pozBitego);
    }


    aktualnyGracz = -aktualnyGracz + 1; // zmienia 0 na 1 i 1 na 0
    emit WykonanoRuch();
}

void Silnik::RuszPionek(int skad, int dokad)
{
    pola[dokad] = pola[skad];
    pola[skad] = -1;
    figury[pola[dokad]]->UstawPole(dokad);

    emit UsunietoFigureZPola(skad);
    emit DodanoFigureNaPole(dokad, &(figury[pola[dokad]]->ikona));


    if(figury[pola[dokad]]->typ==TPionek && figury[pola[dokad]]->strona==0 && dokad < 8) //jesli bialy pionek doszedl do konca planszy
    {
       Promocja(0,dokad);

    }

    if(figury[pola[dokad]]->typ==TPionek && figury[pola[dokad]]->strona==1 &&  dokad > 56) //jesli czaarny pionek doszedl do konca planszy
    {
        Promocja(1,dokad);
    }

    aktualnyGracz = -aktualnyGracz + 1;
    emit WykonanoRuch();
}


void Silnik::Promocja(int strona,int dokad)
{
    msgBox->exec();
    if(msgBox->clickedButton() == hetmanButton)
    {
        emit UsunietoFigureZPola(dokad);
        Figura* fig = new Hetman(strona,dokad);
        figury[pola[dokad]]=fig;
        emit DodanoFigureNaPole(dokad, &(figury[pola[dokad]]->ikona));
    }
    else if (msgBox->clickedButton() == goniecButton)
    {
        emit UsunietoFigureZPola(dokad);
        Figura* fig = new Goniec(strona,dokad);
        figury[pola[dokad]]=fig;
        emit DodanoFigureNaPole(dokad, &(figury[pola[dokad]]->ikona));
    }
    else if (msgBox->clickedButton() == skoczekButton)
    {
        emit UsunietoFigureZPola(dokad);
        Figura* fig = new Skoczek(strona,dokad);
        figury[pola[dokad]]=fig;
        emit DodanoFigureNaPole(dokad, &(figury[pola[dokad]]->ikona));
    }
    else if (msgBox->clickedButton() == wiezaButton)
    {
        emit UsunietoFigureZPola(dokad);
        Figura* fig = new Wieza(strona,dokad);
        figury[pola[dokad]]=fig;
        emit DodanoFigureNaPole(dokad, &(figury[pola[dokad]]->ikona));
    }
}
