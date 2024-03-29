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


    //ruchy dla roszady:
    if(ruszylSie==false) //jesli krol sie nie ruszyl
    {
        if(strona==0)
        {
            if(pola[62]==-1 && pola[61]==-1)
            {
                //teraz musimy jeszcze sprawdzić czy odpowiednia wieza wykonala ruch
                if(pola[63]!=-1 && (*figury)[pola[63]]->typ==TWieza && (*figury)[pola[63]]->ruszylSie==false)
                {
                ruchy.push_back(62);
                }
            }
            if(pola[59]==-1 && pola[58]==-1 && pola[57]==-1)
            {
                if(pola[56]!=-1 && (*figury)[pola[56]]->typ==TWieza && (*figury)[pola[56]]->ruszylSie==false)
                {
                ruchy.push_back(58);
                }
            }
        }

        else if(strona==1)
        {
            if(pola[5]==-1 && pola[6]==-1)
            {
                if(pola[7]!=-1 && (*figury)[pola[7]]->typ==TWieza && (*figury)[pola[7]]->ruszylSie==false)
                {
                ruchy.push_back(6);
                }
            }
            if(pola[1]==-1 && pola[2]==-1 && pola[3]==-1)
            {
                if(pola[0]!=-1 && (*figury)[pola[0]]->typ==TWieza && (*figury)[pola[0]]->ruszylSie==false)
                {
                ruchy.push_back(2);
                }
            }
        }

    }
    return ruchy;
}




