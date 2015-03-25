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

QList<int> Goniec::dostepneRuchy(QString **tab)
{
    //4 petle, goniec moze poruszyc sie w 4 kierunkacah, do momentu napotkaniu jakiejs figury
    dr.clear();
    int tmpx=this->x;
    int tmpy=this->y;

    while(tmpx+1<8 && tmpy+1<8 && (tab[tmpy+1][tmpx+1]=="0" || rozneOdSwoich(tab[tmpy+1][tmpx+1]))){
        tmpx++;
        tmpy++;
        dr.append(this->getIdd(tmpx,tmpy));
        if(tab[tmpy][tmpx]!="0") break;
    }

    tmpx=this->x;
    tmpy=this->y;
    while(tmpx-1>=0 && tmpy+1<8 && (tab[tmpy+1][tmpx-1]=="0" || rozneOdSwoich(tab[tmpy+1][tmpx-1]))){
        tmpx--;
        tmpy++;
        dr.append(this->getIdd(tmpx,tmpy));
        if(tab[tmpy][tmpx]!="0") break;
    }

    tmpy=this->y;
    tmpx=this->x;
    while(tmpx-1>=0 && tmpy-1>=0 && (tab[tmpy-1][tmpx-1]=="0" || rozneOdSwoich(tab[tmpy-1][tmpx-1]))){
        tmpx--;
        tmpy--;
        dr.append(this->getIdd(tmpx,tmpy));
        if(tab[tmpy][tmpx]!="0") break;
    }

    tmpy=this->y;
    tmpx=this->x;
    while(tmpx+1<8 && tmpy-1>=0 && (tab[tmpy-1][tmpx+1]=="0" || rozneOdSwoich(tab[tmpy-1][tmpx+1]))){
        tmpx++;
        tmpy--;
        dr.append(this->getIdd(tmpx,tmpy));
        if(tab[tmpy][tmpx]!="0") break;
    }
    return dr;
}


QList<int> Goniec::szachowanePola(QString **tab)
{
    polaSzachowane.clear();

    return polaSzachowane;
}
