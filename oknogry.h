#ifndef OKNOGRY_H
#define OKNOGRY_H

#include <QMainWindow>
#include <QList>
#include <QVBoxLayout>
#include <QPushButton>
#include <QWidget>
#include <QComboBox>
#include <QString>
#include <QDebug>
#include <QCloseEvent>
#include <QStringList>
#include <QLabel>
#include <QTimer>
#include "figura.h"
#include "goniec.h"
#include "hetman.h"
#include "krol.h"
#include "pionek.h"
#include "skoczek.h"
#include "wieza.h"
#include "oknoopcji.h"

namespace Ui {
class OknoGry;
}

class OknoGry : public QMainWindow
{
    Q_OBJECT

public:
    explicit OknoGry(QWidget *parent = 0);
    ~OknoGry();
private slots:
    void closeEvent(QCloseEvent *);
    void clicked(int id);
    void timeout();
    void on_actionNowa_Gra_triggered();
    void nowa_gra();

private:
    Ui::OknoGry *ui;
    OknoOpcji *opcje;
    QString **pola;
    QList<Figura*> figury;
    QPushButton **pb;
    QLabel *wiadomosc,*timerLabel,*timerLabel2;
    QTimer *timer;
    int zaznaczony; //okresla czy wybrano jakies pole
    bool czy_koniec; //okresla czy zakonczono gre
    Figura *pomocnicza;
    bool tura; //false biale true czarne
    int max_czas_biale;
    int max_czas_czarne;
    QString przelicz_czas(int t);
};

#endif // OKNOGRY_H
