#ifndef SILNIK_H
#define SILNIK_H

#include <QObject>
#include <QList>
#include "Figury/kreatorfigur.h"
#include "Figury/figura.h"
#include "opcje.h"
#include <QMessageBox>

class Silnik : QObject
{
    Q_OBJECT
private:
    QVector<Figura*> figury;
    KreatorFigur* kreator;
    int pola[64]; // Liczba na polu wskazuje na nr figury na liscie
    int zaznaczonePole;
    int aktualnyGracz;

    QMessageBox *msgBox;   //okno z wyborem figury przy promowaniu pionka
    QPushButton *hetmanButton,*goniecButton,*skoczekButton,*wiezaButton;

    void SprawdzRuchy(int pozPionka, QVector<int> ruchy);
    void ZbijPionek(int pozBijacego, int pozBitego);
    void RuszPionek(int skad, int dokad);
    void Promocja(int strona, int dokad);
    bool Sprawdz_czy_szach();
    bool Sprawdz_czy_mat();
    bool sprawdz_czy_pat();
    void jesli_roszada_to_wykonaj(int dokad);
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
    void WykonanoRuch(int);
};

#endif // SILNIK_H
