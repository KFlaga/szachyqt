#ifndef PLANSZA_H
#define PLANSZA_H

#include <QWidget>
#include <QVector>
#include <QPushButton>
#include <QIcon>

class Plansza : public QWidget
{
    Q_OBJECT
public:
    explicit Plansza(QWidget *parent = 0);
    ~Plansza();

    void Reset();

signals:
    void WcisnietoPole(int nrPola);

public slots:
    void Podswietl(QVector<int> pola);
    void UsunFigureZPola(int nrPola);
    void DodajFigureNaPole(int nrPola, QIcon* figura);

private slots:
    void wcisnijPole(int nrPola);

private:
    QPushButton** pola;
    QIcon* iconEmpty;
};

#endif // PLANSZA_H
