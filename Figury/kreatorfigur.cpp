#include "kreatorfigur.h"

KreatorFigur::KreatorFigur()
{

}


Figura* KreatorFigur::StwórzFigurę(int pole, int kolor)
{
    // Narazie zapelnimy wszystko pionami
    // Pozniej w zaleznosci od pola samo dobierze typ figury i kolor
    Figura* fig;
    if( pole == 0 || pole == 7 || pole == 56 || pole == 63 )
    {
        fig = new Wieza(kolor,pole);
    }
    else
    {
        fig = new Pionek(kolor, pole);
    }
    return fig;
}

