#ifndef PIONEK_H
#define PIONEK_H
#include "Figury/figura.h"

class Pionek : public Figura
{
public:
    Pionek(int strona, int pole);

    QVector<int> dostepneRuchy(int* pola, QVector<Figura*>* figury);

};

#endif // PIONEK_H
