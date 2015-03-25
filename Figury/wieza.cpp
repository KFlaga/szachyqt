#include "wieza.h"

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
    int p;
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
    while( tY < 7 && pola[Pole(tX,tY)] == -1 ) // dodajemy w dol az do konca / pionka
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

QList<int> Wieza::dostepneRuchy(QString **tab)
{
    dr.clear();
    int tmpx=x;
    int tmpy=y;
    while(tmpx+1<8 && (tab[y][tmpx+1]=="0" || rozneOdSwoich(tab[y][tmpx+1]))){
        tmpx++;
        dr.append(this->getIdd(tmpx,y));
        if(tab[y][tmpx]!="0") break;
    }
    tmpx=x;
    while(tmpx-1>=0 && (tab[y][tmpx-1]=="0" || rozneOdSwoich(tab[y][tmpx-1]))){
        tmpx--;
        dr.append(this->getIdd(tmpx,y));
        if(tab[y][tmpx]!="0") break;
    }
    while(tmpy+1<8 && (tab[tmpy+1][x]=="0" || rozneOdSwoich(tab[tmpy+1][x]))){
        tmpy++;
        dr.append(this->getIdd(x,tmpy));
        if(tab[tmpy][x]!="0") break;
    }
    tmpy=y;
    while(tmpy-1>=0 && (tab[tmpy-1][x]=="0" || rozneOdSwoich(tab[tmpy-1][x]))){
        tmpy--;
        dr.append(this->getIdd(x,tmpy));
        if(tab[tmpy][x]!="0") break;
    }

    return dr;
}



QList<int> Wieza::szachowanePola(QString **tab)
{
    polaSzachowane.clear();

    return polaSzachowane;
}
