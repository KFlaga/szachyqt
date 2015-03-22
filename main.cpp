#include "oknologowania.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    OknoLogowania w;
    w.show();

    return a.exec();
}
