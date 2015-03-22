#ifndef SKOCZEK_H
#define SKOCZEK_H
#include "figura.h"

class Skoczek : public Figura
{
public:
    Skoczek(int x, int y, bool strona, QString rodzaj);
    QList<int> dostepneRuchy(QString **tab);
    QList<int> szachowanePola(QString **tab);
};

#endif // SKOCZEK_H
