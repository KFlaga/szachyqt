#ifndef SKOCZEK_H
#define SKOCZEK_H
#include "Figury/figura.h"

class Skoczek : public Figura
{
public:
    Skoczek(int strona, int pole);
    QList<int> dostepneRuchy(QString **tab);
    QList<int> szachowanePola(QString **tab);

    QVector<int> dostepneRuchy(int* pola, QVector<Figura*>* figury);
};

#endif // SKOCZEK_H
