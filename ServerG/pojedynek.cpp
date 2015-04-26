#include "pojedynek.h"

Pojedynek::Pojedynek(QTcpSocket *A, QTcpSocket *B)
{
    this->A=A;
    this->B=B;
}

Pojedynek::~Pojedynek()
{

}

