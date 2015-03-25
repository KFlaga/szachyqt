#include "goniec.h"

Goniec::Goniec(int strona, int pole)
{
    UstawPole(pole);
    this->strona=strona;
    this->typ = TGoniec;

    if(strona==0)
        this->ikona=QIcon("szachy/goniec_b.png");
    else
        this->ikona=QIcon("szachy/goniec_c.png");
}

QVector<int> Goniec::dostepneRuchy(int *pola, QVector<Figura *> *figury)
{
    ruchy.clear();
    int tX = x;
    int tY = y;
    int p;

    tX = x; tY = y;
    while( tX+1 < 8 && tY+1 < 8 && ((p=pola[Pole(tX+1,tY+1)]) == -1 || (*figury)[p]->strona != strona ))
    {
        tX++;// ruch dol-prawo
        tY++;
        ruchy.push_back(Pole(tX,tY));
        if(p != -1)
            break;
    }
    tX = x; tY = y;
    while( tX-1 >= 0 && tY+1 < 8 && ((p=pola[Pole(tX-1,tY+1)]) == -1 || (*figury)[p]->strona != strona ))
    {
        tX--; // ruch dol-lewo
        tY++;
        ruchy.push_back(Pole(tX,tY));
        if(p != -1)
            break;
    }
    tX = x; tY = y;
    while( tX-1 >= 0 && tY-1 >= 0 && ((p=pola[Pole(tX-1,tY-1)]) == -1 || (*figury)[p]->strona != strona ))
    {
        tX--; // ruch gora-lewo
        tY--;
        ruchy.push_back(Pole(tX,tY));
        if(p != -1)
            break;
    }
    tX = x; tY = y;
    while( tX+1 < 8 && tY-1 >= 0 && ((p=pola[Pole(tX+1,tY-1)]) == -1 || (*figury)[p]->strona != strona ))
    {
        tX++; // ruch gora-prawo
        tY--;
        ruchy.push_back(Pole(tX,tY));
        if(p != -1)
            break;
    }

    return ruchy;
}

