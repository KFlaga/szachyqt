#ifndef FIGURA_H
#define FIGURA_H
#include <QIcon>
#include <QVector>
#include <QList>
#include <QString>

enum TypFigury { TPionek, TGoniec, TWieza, TSkoczek, THetman, TKrol };


class Figura
{
public:
    static int X(int pole) { return pole%8; }
    static int Y(int pole) { return pole/8; }
    static int Pole(int xx, int yy) { return yy*8+xx; }
    int Pole() { return y*8+x; }
    void UstawPole(int pole)
    {
        x = X(pole);
        y = Y(pole);
    }
    virtual QVector<int> dostepneRuchy(int* pola, QVector<Figura*>* figury) = 0;

    int x,y;
    int strona;
    QIcon ikona;
    TypFigury typ;
    QVector<int> ruchy;
    bool ruszylSie;
};

#endif // FIGURA_H
