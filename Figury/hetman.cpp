#include "hetman.h"

Hetman::Hetman(int x, int y, bool strona, QString rodzaj)
{
    this->x=x;
    this->y=y;
    this->strona=strona;
    this->rodzaj=rodzaj;
    if(strona==0)
        this->ikona=QIcon("szachy/hetman_b.png");
    else
        this->ikona=QIcon("szachy/hetman_c.png");
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
