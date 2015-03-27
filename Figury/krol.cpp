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




