#ifndef SILNIK_H
#define SILNIK_H

#include <QObject>
#include <QList>
#include "Figury/kreatorfigur.h"
#include "Figury/figura.h"
#include "opcje.h"

class Silnik : QObject
{
    Q_OBJECT
private:
    QVector<Figura*> figury;
    KreatorFigur* kreator;
    int pola[64]; // Liczba na polu wskazuje na nr figury na liscie
    int zaznaczonePole;
    int aktualnyGracz;

    void SprawdzRuchy(int pozPionka, QVector<int> ruchy);
    void ZbijPionek(int pozBijacego, int pozBitego);
    void RuszPionek(int skad, int dokad);
public:
    Silnik();
    ~Silnik();

    void NowaGra(Opcje* opts);

public slots:
    void PoleWcisniete(int nrPola);

signals:
    void PodswietlicPola(QVector<int> pola);
    void UsunietoFigureZPola(int nrPola);
    void DodanoFigureNaPole(int nrPola, QIcon* figura);
    void WykonanoRuch();
};

#endif // SILNIK_H
