#include "szachyapp.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SzachyApp sapp;
    sapp.Run();

    return a.exec();
}
