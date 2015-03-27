#include "pionek.h"

Pionek::Pionek(int strona, int pole)
{
    UstawPole(pole);
   this->typ = TPionek;
    this->strona = strona;

    if(strona==0)
        this->ikona=QIcon("szachy/pionek_b.png");
    else
        this->ikona=QIcon("szachy/pionek_c.png");
}


QVector<int> Pionek::dostepneRuchy(int* plansza, QVector<Figura*>* figury)
{
    ruchy.clear();
    int p;
    if( strona == 0 ) // Bialy pionek
    {
        // Jesli sie nie ruszyl i przed pionkiem pusto, to moze ruszyc 2 pola
        if(y == 6 && plansza[Pole(x,y-1)] == -1 && plansza[Pole(x,y-2)] == -1)
            ruchy.push_back(Pole(x,y-2));
        if(y >= 1) // Mozliwy dalszy ruch
        {
            // Jesli przed pionkiem pusto, to moze sie ruszyc do przodu
            if( plansza[Pole(x,y-1)] == -1)
                ruchy.push_back(Pole(x,y-1));
            // Jesli na skosach jest przeciwnik, to mozna bic
            if( x+1 < 8 && (p=plansza[Pole(x+1,y-1)]) != -1 && figury->at(p)->strona != strona )
                ruchy.push_back(Pole(x+1,y-1));
            if( x > 0 && (p=plansza[Pole(x-1,y-1)]) != -1 && figury->at(p)->strona != strona )
                ruchy.push_back(Pole(x-1,y-1));
        }
    }
    else // Czarny pionek
    {
            // Jesli sie nie ruszyl i przed pionkiem pusto, to moze ruszyc 2 pola
            if(y == 1 && plansza[Pole(x,y+1)] == -1 && plansza[Pole(x,y+2)] == -1)
                ruchy.push_back(Pole(x,y+2));
            if(y < 7) // Mozliwy dalszy ruch
            {
                // Jesli przed pionkiem pusto, to moze sie ruszyc do przodu
                if( plansza[Pole(x,y+1)] == -1)
                    ruchy.push_back(Pole(x,y+1));
                // Jesli na skosach jest przeciwnik, to mozna bic
                if( x+1 < 8 && (p=plansza[Pole(x+1,y+1)]) != -1 && figury->at(p)->strona != strona )
                    ruchy.push_back(Pole(x+1,y+1));
                if( x > 0 && (p=plansza[Pole(x-1,y+1)]) != -1 && figury->at(p)->strona != strona )
                    ruchy.push_back(Pole(x-1,y+1));
            }
    }

    return ruchy;
}

// W dalszej wersji trzebaby dodac takie tematy jak czy po ruchu bedzie szach




