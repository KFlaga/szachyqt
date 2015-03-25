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


    // STARY INTERFEJS dla kompatybilnosci narazie zostaje
public:
    QList<int> dr;     //lista ruchów z metody 'dostepneRuchy'
    QList<int> polaSzachowane;

    QString rodzaj;
    bool ruszylSie;
    int getId();
    int getIdd(int xx, int yy);   //podajemy x i y i dostajemy id
    void setId(int id);   //metoda ustawiwajaca polozenia na podstawie id;
    virtual QList<int> dostepneRuchy(QString** pola) = 0;
    virtual QList<int> szachowanePola(QString** pola) = 0;

    bool poprawny_ruch(int id);                                 //metoda sprawdzająca czy ruch jest poprawny
    bool rozneOdSwoich(QString tab) { return false; }
};

#endif // FIGURA_H
