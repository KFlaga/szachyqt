#ifndef KREATORFIGUR_H
#define KREATORFIGUR_H

#include "Figury/figura.h"
#include "Figury/goniec.h"
#include "Figury/hetman.h"
#include "Figury/krol.h"
#include "Figury/pionek.h"
#include "Figury/skoczek.h"
#include "Figury/wieza.h"


class KreatorFigur
{
public:
    KreatorFigur();

    Figura* StwórzFigurę(int pole, int kolor);
};

#endif // KREATORFIGUR_H
