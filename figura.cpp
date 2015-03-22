#include "figura.h"


int Figura::getId() //metoda zwracjaca id danej figury
{
    return y*8+x;
}


int Figura::getIdd(int xx, int yy){  //metoda zwracajaca id pola na podstawie x,y
    return yy*8+xx;
}

void Figura::setId(int id)
{
    this->x=id%8;
    this->y=id/8;
}

bool Figura::rozneOdSwoich(QString pole){  //metoda sprawdzająca czy dana figura jest nasza czy przeciwnika, zwraca true jesli figura jest przeciwnika
    if(strona==0){ //biale
        if(pole=="p" || pole=="w" || pole=="s" || pole=="g" || pole=="h" || pole=="k") //biale
            return false;
        else
            return true;
    }else{ //czarne
        if(pole=="P" || pole=="W" || pole=="S" || pole=="G" || pole=="H" || pole=="K") //czarne
            return false;
        else
            return true;
    }
}


bool Figura::poprawny_ruch(int id)
{
    return dr.contains(id);
}
