#include "mainwindow.h"
#include <QApplication>
#include <QtSql>
#include <QTimer>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    MainWindow w;
    w.show();
    QTimer::singleShot(1000, &w, SLOT(zacznijPrzetwarzanieWiadomosci()));

    return a.exec();
}
