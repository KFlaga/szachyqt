#ifndef SILNIK_H
#define SILNIK_H

#include <QObject>
#include <QList>
#include "Figury/kreatorfigur.h"
#include "Figury/figura.h"
#include "opcje.h"
#include <QMessageBox>
#include <QProcess>
#include "ModulKomunikacji/klient.h"


class Silnik : QObject
{
    Q_OBJECT
private:
    QVector<Figura*> figury;
    KreatorFigur* kreator;
    int pola[64]; // Liczba na polu wskazuje na nr figury na liscie
    int ilu_szachujacych;
    QVector<int> indexy_szachujacych;
    int zaznaczonePole;
    int aktualnyGracz;
    bool gra_z_kompem;
    bool gra_przez_siec;
    bool czy_koniec;
    QString zwrot;
    QProcess *p;                                                                //proces do komunikacji z Brutusem
    QString promocja;    //do odebrania od AI
    QString nasza_promocja; //do wyslania AI jesli gramy z komputerem i promujemy pionka

    QMessageBox *msgBox;   //okno z wyborem figury przy promowaniu pionka
    QPushButton *hetmanButton,*goniecButton,*skoczekButton,*wiezaButton;

    QString przeciwnik;
    bool czyRuchPrzeciwnika;

    void SprawdzRuchy(int pozPionka, QVector<int> ruchy);
    void ZbijPionek(int pozBijacego, int pozBitego);
    void RuszPionek(int skad, int dokad);
    void Promocja(int strona, int dokad);
    bool Sprawdz_czy_szach();
    bool Sprawdz_czy_mat();
    bool sprawdz_czy_pat();
    void jesli_roszada_to_wykonaj(int dokad);
    bool sprawdz_czy_ruch(QString s);
    void wyslij(int id_1, int id_2,QString promocja);
    void RuchAI(int nrPolaStartowego, int nrPolaDocelowego);
    void RuchSiec(int nrPolaStartowego, int nrPolaDocelowego);

public:
    Silnik();
    ~Silnik();

    void NowaGra(Opcje* opts);

public slots:
    void PoleWcisniete(int nrPola);
    void read();
    void odebranoRuch(QString ruch);

signals:
    void PodswietlicPola(QVector<int> pola);
    void UsunietoFigureZPola(int nrPola);
    void DodanoFigureNaPole(int nrPola, QIcon* figura);
    void WykonanoRuch(int);
    void wyslijRuch(QString);
};

#endif // SILNIK_H
