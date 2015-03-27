#include "kreatorfigur.h"

KreatorFigur::KreatorFigur()
{

}


Figura* KreatorFigur::StworzFigure(int pole, int kolor)
{
    Figura* fig;
    if( pole == 0 || pole == 56 || pole == 7 || pole == 63)
    {
        fig = new Wieza(kolor,pole);
    }

    else if( pole == 1 || pole == 6 || pole == 57 || pole == 62 )
    {
        fig = new Skoczek(kolor, pole);
    }
    else if( pole == 2 || pole == 5 || pole == 58 || pole == 61)
    {
        fig = new Goniec(kolor, pole);
    }
    else if( pole == 3 || pole == 59 )
    {
        fig = new Hetman(kolor, pole);
    }
    else if( pole == 4 || pole == 60 )
    {
        fig = new Krol(kolor, pole);
    }
    else
    {
        fig = new Pionek(kolor, pole);
    }
    return fig;
}

