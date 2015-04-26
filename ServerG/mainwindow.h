#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QTcpServer>
#include <QVector>
#include <QList>
#include <QLabel>
#include <QMap>
#include <QFile>
#include "pojedynek.h"
#include "uzytkownik.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    int pobierzID(QString& data);
    void sendUsersList();
    void odczytajListeUzytkownikow();
    void zapiszUzytkownika(Uzytkownik*);

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();

    void newConnection();
    void removeClient();
    void readyRead();


private:
    Ui::MainWindow *ui;
    QTcpServer *server;

    QFile plikListaUzytkownikow; // plik z lista zarejestrownych uzytkownikow - pozniej bedzie to w bazie
    QVector<Uzytkownik*> uzytkownicy; // zaladowane dane uzytkownikow
    QMap<QTcpSocket*, Uzytkownik*> poloczenia; //mapa przechowująca zalogowanych użytkowników wraz z ich socketami
    QVector<Pojedynek*> pojedynki; //wektor z pojedynkami
    int ile;
    QLabel *l;
};

#endif // MAINWINDOW_H
