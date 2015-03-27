#ifndef KROL_H
#define KROL_H
#include "Figury/figura.h"

class Krol : public Figura
{
public:
    Krol(int strona, int pole);

    QVector<int> dostepneRuchy(int* pola, QVector<Figura*>* figury);

};

#endif // KROL_H
