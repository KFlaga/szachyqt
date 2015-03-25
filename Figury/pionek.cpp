#include "pionek.h"

Pionek::Pionek(TypFigury typ, int strona, int pole)
{
   this->typ = typ;
    this->strona = strona;
    this->x = X(pole);
    this->y = Y(pole);

    if(strona==0)
        this->ikona=QIcon("szachy/pionek_b.png");
    else
        this->ikona=QIcon("szachy/pionek_c.png");
}



QList<int> Pionek::dostepneRuchy(QString **tab)
{
    dr.clear();
    if(strona==0){  //0 - białe, 1- czarne
        if(y==6){ //jesli pionek bialy jeszcze sie nie ruszyl
            if(tab[y-1][x]=="0" && tab[y-2][x]=="0")
                dr.append(this->getIdd(x,y-2));
        }
        if(y-1>=0 && tab[y-1][x]=="0")
            dr.append(this->getIdd(x,y-1));
        if(y-1>=0 && x+1<8 && tab[y-1][x+1]!="0" && rozneOdSwoich(tab[y-1][x+1]))
            dr.append(this->getIdd(x+1,y-1));
        if(y-1>=0 && x-1>=0 && tab[y-1][x-1]!="0" && rozneOdSwoich(tab[y-1][x-1]))
            dr.append(this->getIdd(x-1,y-1));
    }else{
        if(y==1){ //jesli pionek czarny jeszcze sie nie ruszyl
            if(tab[y+1][x]=="0" && tab[y+2][x]=="0")
                dr.append(this->getIdd(x,y+2));
        }
        if(y+1<8 && tab[y+1][x]=="0")
            dr.append(this->getIdd(x,y+1));
        if(y+1<8 && x+1<8 && tab[y+1][x+1]!="0" && rozneOdSwoich(tab[y+1][x+1]))
            dr.append(this->getIdd(x+1,y+1));
        if(y+1<8 && x-1>=0 && tab[y+1][x-1]!="0" && rozneOdSwoich(tab[y+1][x-1]))
            dr.append(this->getIdd(x-1,y+1));
    }
    return dr;
}


QList<int> Pionek::szachowanePola(QString **tab)
{
    polaSzachowane.clear();

    return polaSzachowane;
}

QVector<int> Pionek::dostepneRuchy(int pole, int* plansza, QVector<Figura*>* figury)
{
    QVector<int> ruchy;
    int x = X(pole);
    int y = Y(pole);
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
