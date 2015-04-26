#ifndef POJEDYNEK_H
#define POJEDYNEK_H
#include <QTcpSocket>

class Pojedynek
{
public:
    Pojedynek(QTcpSocket *A, QTcpSocket *B);
    ~Pojedynek();
    QTcpSocket *A;
    QTcpSocket *B;
};

#endif // POJEDYNEK_H
