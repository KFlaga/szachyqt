#ifndef WIEZA_H
#define WIEZA_H
#include "Figury/figura.h"

class Wieza : public Figura
{
public:
    Wieza(int strona, int pole);
    QList<int> dostepneRuchy(QString **tab);
    QList<int> szachowanePola(QString **tab);

    QVector<int> dostepneRuchy(int pole, int* pola, QVector<Figura*>* figury);
};

#endif // WIEZA_H
