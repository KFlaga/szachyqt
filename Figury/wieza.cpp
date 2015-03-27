#include "wieza.h"
#include <QDebug>
Wieza::Wieza(int strona, int pole)
{
    UstawPole(pole);
    this->strona=strona;
    this->typ = TWieza;
    this->ruszylSie=false;
    if(strona == 0)
        this->ikona=QIcon("szachy/wieza_b.png");
    else
        this->ikona=QIcon("szachy/wieza_c.png");
}

QVector<int> Wieza::dostepneRuchy(int *pola, QVector<Figura *> *figury)
{
    ruchy.clear();
    int tX = x+1;
    int tY = y;

    while( tX <= 7 && pola[Pole(tX,tY)] == -1 ) // dodajemy w lewo az do konca / pionka
    {
        ruchy.push_back(Pole(tX,tY));
        tX++;
    }
    if( tX <= 7 && (*figury)[pola[Pole(tX,tY)]]->strona != strona ) // jak wrog to dodaj
        ruchy.push_back(Pole(tX,tY));

    tX = x-1;
    while( tX >= 0 && pola[Pole(tX,tY)] == -1 ) // dodajemy w prawo az do konca / pionka
    {
        ruchy.push_back(Pole(tX,tY));
        tX--;
    }
    if( tX >= 0 && (*figury)[pola[Pole(tX,tY)]]->strona != strona ) // jak wrog to dodaj
        ruchy.push_back(Pole(tX,tY));

    tX = x;
    tY = y+1;
    while( tY <= 7 && pola[Pole(tX,tY)] == -1 ) // dodajemy w dol az do konca / pionka
    {
        ruchy.push_back(Pole(tX,tY));
        tY++;
    }
    if( tY <= 7 && (*figury)[pola[Pole(tX,tY)]]->strona != strona ) // jak wrog to dodaj
        ruchy.push_back(Pole(tX,tY));

    tY = y-1;
    while( tY >= 0 && pola[Pole(tX,tY)] == -1 ) // dodajemy w gore az do konca / pionka
    {
        ruchy.push_back(Pole(tX,tY));
        tY--;
    }
    if( tY >= 0 && (*figury)[pola[Pole(tX,tY)]]->strona != strona ) // jak wrog to dodaj
        ruchy.push_back(Pole(tX,tY));

    return ruchy;
}



