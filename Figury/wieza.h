#ifndef WIEZA_H
#define WIEZA_H
#include "Figury/figura.h"

class Wieza : public Figura
{
public:
    Wieza(int x, int y, bool strona, QString rodzaj);
    QList<int> dostepneRuchy(QString **tab);
    QList<int> szachowanePola(QString **tab);
};

#endif // WIEZA_H
