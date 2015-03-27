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



