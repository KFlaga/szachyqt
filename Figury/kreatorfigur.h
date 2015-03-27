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
private:
    int polaWiez[4];
    int polaSkoczkow[4];
    int polaGoncow[4];
    int polaKroli[2];
    int polaHetmanow[2];

public:
    KreatorFigur();

    Figura* StworzFigure(int pole, int kolor);
};

#endif // KREATORFIGUR_H
