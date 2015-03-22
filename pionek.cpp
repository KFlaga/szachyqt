#include "pionek.h"

Pionek::Pionek(int x, int y, bool strona, QString rodzaj)
{
    this->x=x;
    this->y=y;
    this->strona=strona;
    this->rodzaj=rodzaj;
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
