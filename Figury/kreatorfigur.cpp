#include "kreatorfigur.h"

KreatorFigur::KreatorFigur()
{

}


Figura* KreatorFigur::StwórzFigurę(int pole, int kolor)
{
    // Narazie zapelnimy wszystko pionami
    // Pozniej w zaleznosci od pola samo dobierze typ figury i kolor
    Figura* fig = new Pionek(TPionek, kolor, pole);
    return fig;
}

