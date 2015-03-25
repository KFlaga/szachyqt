#include "silnik.h"
#include <QVector>

Silnik::Silnik()
{
    kreator =  new KreatorFigur();
    for(int i = 0; i < 64; i++)
        pola[i] = -1;
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
        Figura* fig = kreator->StwórzFigurę(i,1);
        figury.push_back(fig);
        pola[i] = i;
        emit DodanoFigureNaPole(i,&(fig->ikona));
    }

    for(int i = 16; i < 32; i++)
    {
        Figura* fig = kreator->StwórzFigurę(i,0);
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
            zaznaczonePola = figury[pola[nrPola]]->dostepneRuchy(nrPola, pola, &figury);
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
                zaznaczonePola = figury[pola[nrPola]]->dostepneRuchy(nrPola, pola, &figury);
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

    aktualnyGracz = -aktualnyGracz + 1;
    emit WykonanoRuch();
}
