#ifndef WIEZA_H
#define WIEZA_H
#include "Figury/figura.h"

class Wieza : public Figura
{
public:
    Wieza(int strona, int pole);

    QVector<int> dostepneRuchy(int* pola, QVector<Figura*>* figury);

};

#endif // WIEZA_H
