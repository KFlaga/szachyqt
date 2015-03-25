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

bool Figura::poprawny_ruch(int id)
{
    return dr.contains(id);
}
