#ifndef GONIEC_H
#define GONIEC_H
#include "figura.h"

class Goniec : public Figura
{
public:
    Goniec(int x, int y, bool strona, QString rodzaj);
    QList<int> dostepneRuchy(QString **tab);
    QList<int> szachowanePola(QString **tab);
};

#endif // GONIEC_H
