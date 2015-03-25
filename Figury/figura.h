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
    int X(int pole) { return pole%8; }
    int Y(int pole) { return pole/8; }
    int Pole(int x, int y) { return y*8+x; }

    int x,y;
    int strona;    //0 - białe, 1- czarne
    QIcon ikona;
    QString rodzaj;
    TypFigury typ;
    bool ruszylSie; //oznnaczenie czy wieza, krol wykonali ruch, potrzebne do roszady
    QList<int> dr;                                              //lista ruchów z metody 'dostepneRuchy'
    QList<int> polaSzachowane;

    int getId();
    int getIdd(int xx, int yy);   //podajemy x i y i dostajemy id
    void setId(int id);   //metoda ustawiwajaca polozenia na podstawie id;
    virtual QList<int> dostepneRuchy(QString** pola) = 0;
    virtual QList<int> szachowanePola(QString** pola) = 0;
    virtual QVector<int> dostepneRuchy(int pole, int* pola, QVector<Figura*>* figury) = 0;
    bool poprawny_ruch(int id);                                 //metoda sprawdzająca czy ruch jest poprawny
    bool rozneOdSwoich(QString tab) { return false; }
};

#endif // FIGURA_H
