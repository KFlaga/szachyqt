#ifndef FIGURA_H
#define FIGURA_H
#include <QIcon>
#include <QList>
#include <QString>

class Figura
{
public:
    int x,y;
    int strona;    //0 - białe, 1- czarne
    QIcon ikona;
    QString rodzaj;     //W,S,G,H,K,P
    bool ruszylSie; //oznnaczenie czy wieza, krol wykonali ruch, potrzebne do roszady
    QList<int> dr;                                              //lista ruchów z metody 'dostepneRuchy'
    QList<int> polaSzachowane;
    int getId();
    int getIdd(int xx, int yy);   //podajemy x i y i dostajemy id
    void setId(int id);   //metoda ustawiwajaca polozenia na podstawie id;
    bool rozneOdSwoich(QString pole);                           //metoda sprawdzająca czy dana figura jest nasza czy przeciwnika
    virtual QList<int> dostepneRuchy(QString **tab)=0;
    virtual QList<int> szachowanePola(QString **tab) = 0;  //metoda zwracająca listę pól, które dana figura może zbić
    bool poprawny_ruch(int id);                                 //metoda sprawdzająca czy ruch jest poprawny
};

#endif // FIGURA_H
