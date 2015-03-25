#include "skoczek.h"

Skoczek::Skoczek(int x, int y, bool strona, QString rodzaj)
{
    this->x=x;
    this->y=y;
    this->strona=strona;
    this->rodzaj=rodzaj;
    if(strona==0)
        this->ikona=QIcon("szachy/skoczek_b.png");
    else
        this->ikona=QIcon("szachy/skoczek_c.png");
}


QList<int> Skoczek::dostepneRuchy(QString **tab)
{
    dr.clear();
    if(x+1<8 && y+2<8 && (tab[y+2][x+1]=="0" || rozneOdSwoich(tab[y+2][x+1])))
        dr.append(this->getIdd(x+1,y+2));
    if(x+1<8 && y-2>=0 && (tab[y-2][x+1]=="0" || rozneOdSwoich(tab[y-2][x+1])))
        dr.append(this->getIdd(x+1,y-2));
    if(x+2<8 && y+1<8 && (tab[y+1][x+2]=="0" || rozneOdSwoich(tab[y+1][x+2])))
        dr.append(this->getIdd(x+2,y+1));
    if(x+2<8 && y-1>=0 && (tab[y-1][x+2]=="0" || rozneOdSwoich(tab[y-1][x+2])))
        dr.append(this->getIdd(x+2,y-1));

    if(x-1>=0 && y+2<8 && (tab[y+2][x-1]=="0" || rozneOdSwoich(tab[y+2][x-1])))
        dr.append(this->getIdd(x-1,y+2));
    if(x-1>=0 && y-2>=0 && (tab[y-2][x-1]=="0" || rozneOdSwoich(tab[y-2][x-1])))
        dr.append(this->getIdd(x-1,y-2));
    if(x-2>=0 && y+1<8 && (tab[y+1][x-2]=="0" || rozneOdSwoich(tab[y+1][x-2])))
        dr.append(this->getIdd(x-2,y+1));
    if(x-2>=0 && y-1>=0 && (tab[y-1][x-2]=="0" || rozneOdSwoich(tab[y-1][x-2])))
        dr.append(this->getIdd(x-2,y-1));

    return dr;
}


QList<int> Skoczek::szachowanePola(QString **tab)
{
    polaSzachowane.clear();

    return polaSzachowane;
}
