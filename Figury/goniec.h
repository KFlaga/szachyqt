#ifndef GONIEC_H
#define GONIEC_H
#include "Figury/figura.h"

class Goniec : public Figura
{
public:
    Goniec(int strona, int pole);

    QVector<int> dostepneRuchy(int* pola, QVector<Figura*>* figury);

};

#endif // GONIEC_H
