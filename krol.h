#ifndef KROL_H
#define KROL_H
#include "figura.h"

class Krol : public Figura
{
public:
    Krol(int x, int y, bool strona, QString rodzaj);
    QList<int> dostepneRuchy(QString **tab);
    QList<int> szachowanePola(QString **tab);
};

#endif // KROL_H
