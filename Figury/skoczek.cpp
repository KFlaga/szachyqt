#include "skoczek.h"

Skoczek::Skoczek(int strona, int pole)
{
    UstawPole(pole);
    this->strona=strona;
    this->typ = TSkoczek;
    if(strona==0)
        this->ikona=QIcon("szachy/skoczek_b.png");
    else
        this->ikona=QIcon("szachy/skoczek_c.png");
}

QVector<int> Skoczek::dostepneRuchy(int *pola, QVector<Figura *> *figury)
{
    ruchy.clear();

    // Pierw dodaj wszystkie 8 ruchow jesli mieszcza sie na planszy
    if(x+1<8 && y+2<8)
        ruchy.push_back(Pole(x+1,y+2));
    if(x+1<8 && y-2>=0)
        ruchy.push_back(Pole(x+1,y-2));
    if(x+2<8 && y+1<8)
        ruchy.push_back(Pole(x+2,y+1));
    if(x+2<8 && y-1>=0)
        ruchy.push_back(Pole(x+2,y-1));
    if(x-1>=0 && y+2<8)
        ruchy.push_back(Pole(x-1,y+2));
    if(x-1>=0 && y-2>=0)
        ruchy.push_back(Pole(x-1,y-2));
    if(x-2>=0 && y+1<8)
        ruchy.push_back(Pole(x-2,y+1));
    if(x-2>=0 && y-1>=0)
        ruchy.push_back(Pole(x-2,y-1));

    for( int i = 0; i < ruchy.size(); i++ )
    {
        // A potem usun jesli stoi tam swoj pionek
        if(pola[ruchy[i]] != -1 && (*figury)[pola[ruchy[i]]]->strona == strona )
        {
            ruchy.removeAt(i);
            i--;
        }
    }

    return ruchy;
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
