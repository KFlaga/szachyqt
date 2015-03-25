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
#include "ModulRozgrywki/plansza.h"
#include "opcje.h"

namespace Ui
{
class OknoGry;
}

class OknoGry : public QMainWindow
{
    Q_OBJECT

public:
    explicit OknoGry(QWidget *parent = 0);
    ~OknoGry();

    Plansza* WezPlansze();
    void NowaGra(Opcje* opts);

private:
    void inicjalizujUI();
    void inicjalizujRamke();

public slots:
    void WykonanoRuch();

signals:


private slots:
    void closeEvent(QCloseEvent *);
    void timeout();

private:
    Ui::OknoGry *ui;
    Plansza *plansza;
    QLabel *wiadomoscTura,*labTimerBiale,*labTimerCzarne;
    QTimer *timer;
    bool tura; //false biale true czarne
    bool czy_koniec;
    int max_czas_biale;
    int max_czas_czarne;
    QString przelicz_czas(int t);
};

#endif // OKNOGRY_H
