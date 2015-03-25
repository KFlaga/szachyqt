#include "wieza.h"

Wieza::Wieza(int x, int y, bool strona, QString rodzaj)
{
    this->x=x;
    this->y=y;
    this->strona=strona;
    this->rodzaj=rodzaj;
    this->ruszylSie=false;
    if(strona==0)
        this->ikona=QIcon("szachy/wieza_b.png");
    else
        this->ikona=QIcon("szachy/wieza_c.png");
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
