#ifndef HETMAN_H
#define HETMAN_H
#include "Figury/figura.h"

class Hetman : public Figura
{
public:
    Hetman(int strona, int pole);
    QList<int> dostepneRuchy(QString **tab);
    QList<int> szachowanePola(QString **tab);

    QVector<int> dostepneRuchy(int* pola, QVector<Figura*>* figury);
};

#endif // HETMAN_H
