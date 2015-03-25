#include "krol.h"

Krol::Krol(int x, int y, bool strona, QString rodzaj)
{
    this->x=x;
    this->y=y;
    this->strona=strona;
    this->rodzaj=rodzaj;
    this->ruszylSie=false;
    if(strona==0)
        this->ikona=QIcon("szachy/krol_b.png");
    else
        this->ikona=QIcon("szachy/krol_c.png");
}



QList<int> Krol::dostepneRuchy(QString **tab)
{
    dr.clear();
    int tmpx=x;
    int tmpy=y;
    if(tmpx+1<8 && tmpy+1<8 && (tab[tmpy+1][tmpx+1]=="0" || rozneOdSwoich(tab[tmpy+1][tmpx+1]))){
        tmpx++;
        tmpy++;
        dr.append(this->getIdd(tmpx,tmpy));
    }
    tmpx=x;
    tmpy=y;
    if(tmpx-1>=0 && tmpy+1<8 && (tab[tmpy+1][tmpx-1]=="0" || rozneOdSwoich(tab[tmpy+1][tmpx-1]))){
        tmpx--;
        tmpy++;
        dr.append(this->getIdd(tmpx,tmpy));
    }
    tmpy=y;
    tmpx=x;
    if(tmpx-1>=0 && tmpy-1>=0 && (tab[tmpy-1][tmpx-1]=="0" || rozneOdSwoich(tab[tmpy-1][tmpx-1]))){
        tmpx--;
        tmpy--;
        dr.append(this->getIdd(tmpx,tmpy));
    }
    tmpy=y;
    tmpx=x;
    if(tmpx+1<8 && tmpy-1>=0 && (tab[tmpy-1][tmpx+1]=="0" || rozneOdSwoich(tab[tmpy-1][tmpx+1]))){
        tmpx++;
        tmpy--;
        dr.append(this->getIdd(tmpx,tmpy));
    }
    tmpx=x;
    if(tmpx+1<8 && (tab[y][tmpx+1]=="0" || rozneOdSwoich(tab[y][tmpx+1]))){
        tmpx++;
        dr.append(this->getIdd(tmpx,y));
    }
    tmpx=x;
    if(tmpx-1>=0 && (tab[y][tmpx-1]=="0" || rozneOdSwoich(tab[y][tmpx-1]))){
        tmpx--;
        dr.append(this->getIdd(tmpx,y));
    }
    tmpy=y;
    if(tmpy+1<8 && (tab[tmpy+1][x]=="0" || rozneOdSwoich(tab[tmpy+1][x]))){
        tmpy++;
        dr.append(this->getIdd(x,tmpy));
    }
    tmpy=y;
    if(tmpy-1>=0 && (tab[tmpy-1][x]=="0" || rozneOdSwoich(tab[tmpy-1][x]))){
        tmpy--;
        dr.append(this->getIdd(x,tmpy));
    }
    return dr;
}


QList<int> Krol::szachowanePola(QString **tab)
{
    polaSzachowane.clear();

    return polaSzachowane;
}
