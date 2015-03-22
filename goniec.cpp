#include "goniec.h"

Goniec::Goniec(int x, int y, bool strona, QString rodzaj)
{
    this->x=x;
    this->y=y;
    this->strona=strona;
    this->rodzaj=rodzaj;
    if(strona==0)
        this->ikona=QIcon("szachy/goniec_b.png");
    else
        this->ikona=QIcon("szachy/goniec_c.png");
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
