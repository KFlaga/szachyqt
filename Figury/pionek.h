#ifndef PIONEK_H
#define PIONEK_H
#include "Figury/figura.h"

class Pionek : public Figura
{
public:
    Pionek(int strona, int pole);

    QVector<int> dostepneRuchy(int* pola, QVector<Figura*>* figury);
    bool promocja_pionka(int id);
};

#endif // PIONEK_H
