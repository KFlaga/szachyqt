#ifndef PIONEK_H
#define PIONEK_H
#include "figura.h"

class Pionek : public Figura
{
public:
    Pionek(int x, int y, bool strona, QString rodzaj);
    QList<int> dostepneRuchy(QString **tab);
    QList<int> szachowanePola(QString **tab);
};

#endif // PIONEK_H
