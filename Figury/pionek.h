#ifndef PIONEK_H
#define PIONEK_H
#include "Figury/figura.h"

class Pionek : public Figura
{
public:
    Pionek(TypFigury typ, int strona, int pole);
    QList<int> dostepneRuchy(QString **tab);
    QList<int> szachowanePola(QString **tab);

    QVector<int> dostepneRuchy(int pole, int* pola, QVector<Figura*>* figury);
    QVector<int> szachowanePola(int pole) { return QVector<int>(); }
};

#endif // PIONEK_H
