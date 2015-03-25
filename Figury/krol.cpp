#include "krol.h"

Krol::Krol(int strona, int pole)
{
    UstawPole(pole);
    this->strona=strona;
    this->typ = TKrol;
    this->ruszylSie=false;
    if(strona==0)
        this->ikona=QIcon("szachy/krol_b.png");
    else
        this->ikona=QIcon("szachy/krol_c.png");
}

QVector<int> Krol::dostepneRuchy(int *pola, QVector<Figura *> *figury)
{
    ruchy.clear();

    for(int dx = -1; dx <= 1; dx++) // Sprawdzamy caly kwadrat 3x3 czy miesci sie na planszy
    {
        if( x+dx >= 0 && x+dx <= 7)
        {
            for(int dy = -1; dy <= 1; dy++)
            {
                if( y+dy >= 0 && y+dy <= 7)
                {
                    // Ruch ok jak jest tam puste badz przeciwnik
                    if(pola[Pole(x+dx,y+dy)] == -1 || (*figury)[pola[Pole(x+dx,y+dy)]]->strona != strona)
                        ruchy.push_back(Pole(x+dx,y+dy));
                }
            }
        }
    }

    return ruchy;
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
