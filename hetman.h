#ifndef HETMAN_H
#define HETMAN_H
#include "figura.h"

class Hetman : public Figura
{
public:
    Hetman(int x, int y, bool strona, QString rodzaj);
    QList<int> dostepneRuchy(QString **tab);
    QList<int> szachowanePola(QString **tab);
};

#endif // HETMAN_H
