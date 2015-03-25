#include "hetman.h"

Hetman::Hetman(int strona, int pole)
{
    UstawPole(pole);
    this->strona=strona;
    this->typ = THetman;
    if(strona==0)
        this->ikona=QIcon("szachy/hetman_b.png");
    else
        this->ikona=QIcon("szachy/hetman_c.png");
}

QVector<int> Hetman::dostepneRuchy(int *pola, QVector<Figura *> *figury)
{
    ruchy.clear();

    // Identyko jak wieza, a potem goniec
    int tX = x+1;
    int tY = y;
    int p;

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

QList<int> Hetman::dostepneRuchy(QString **tab)
{
    dr.clear();
    int tmpx=x;
    int tmpy=y;

    while(tmpx+1<8 && tmpy+1<8 && (tab[tmpy+1][tmpx+1]=="0" || rozneOdSwoich(tab[tmpy+1][tmpx+1]))){
        tmpx++;
        tmpy++;
        dr.append(this->getIdd(tmpx,tmpy));
        if(tab[tmpy][tmpx]!="0") break;
    }
    tmpx=x;
    tmpy=y;
    while(tmpx-1>=0 && tmpy+1<8 && (tab[tmpy+1][tmpx-1]=="0" || rozneOdSwoich(tab[tmpy+1][tmpx-1]))){
        tmpx--;
        tmpy++;
        dr.append(this->getIdd(tmpx,tmpy));
        if(tab[tmpy][tmpx]!="0") break;
    }
    tmpy=y;
    tmpx=x;
    while(tmpx-1>=0 && tmpy-1>=0 && (tab[tmpy-1][tmpx-1]=="0" || rozneOdSwoich(tab[tmpy-1][tmpx-1]))){
        tmpx--;
        tmpy--;
        dr.append(this->getIdd(tmpx,tmpy));
        if(tab[tmpy][tmpx]!="0") break;
    }
    tmpy=y;
    tmpx=x;
    while(tmpx+1<8 && tmpy-1>=0 && (tab[tmpy-1][tmpx+1]=="0" || rozneOdSwoich(tab[tmpy-1][tmpx+1]))){
        tmpx++;
        tmpy--;
        dr.append(this->getIdd(tmpx,tmpy));
        if(tab[tmpy][tmpx]!="0") break;
    }
    tmpx=x;
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
    tmpy=y;
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


QList<int> Hetman::szachowanePola(QString **tab)
{
    polaSzachowane.clear();

    return polaSzachowane;
}
