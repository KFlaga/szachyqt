#include "klient.h"
#include <QMessageBox>
#include <QPushButton>

Klient::Klient() : QObject()
{
    socket = new QTcpSocket(this);
    connect(socket,SIGNAL(connected()),this,SLOT(connected()));
    connect(socket,SIGNAL(disconnected()),this,SLOT(disconnected()));
    connect(socket,SIGNAL(readyRead()),this,SLOT(readyRead()));
  //  polacz();

    timerCzekajNaPolaczenie = new QTimer(this);
    timerCzekajNaPolaczenie->setInterval(5000);
    timerCzekajNaPolaczenie->setSingleShot(true);
    timerCzekajNaPolaczenie->setTimerType(Qt::CoarseTimer);
    connect(timerCzekajNaPolaczenie, SIGNAL(timeout()),
           this,SLOT(polacz()));
}

void Klient::wyslijWiadomosc(QString *text, IKomunikator* kom)
{
    if( socket->state() != QTcpSocket::ConnectedState )
    {
        return;
    }
    komunikatory.append(kom);
    // Na koncu wiadomosci dodajemy ID komunikatora
    // Jesli w przyszlosci bedziemy wysylac wiadomosci z
    // roznych miejsc programu, mozliwe, ze bedzie wiecej niz
    // jeden komunikator oczekujacy, wiec ID potrzebne]
    // by zwrocic odpowiedz do odpowiedniego nadawcy
    text->append(":");
    text->append(QString::number(kom->wezID()));
    const char* data = text->toStdString().c_str();
    socket->write(data);
    socket->flush();
}

void Klient::wyslijWiadomosc(QString *text, int id)
{
    if( socket->state() != QTcpSocket::ConnectedState )
    {
        return;
    }
    text->append(":");
    text->append(QString::number(id));
    const char* data = text->toStdString().c_str();
    socket->write(data);
    socket->flush();
}

void Klient::polacz()
{
    // do ew. zmiany na bardziej szczegółowe sprawdzenie
    // stanu oraz powodu niepołączenia
    if( socket->state() != QTcpSocket::ConnectedState )
    {
        socket->close();
        socket->connectToHost("127.0.0.1",2222);
        timerCzekajNaPolaczenie->start();
    }

}

void Klient::rozlacz()
{
    socket->close();
}

void Klient::connected()
{
    timerCzekajNaPolaczenie->stop();
    emit poloczono();
}

void Klient::disconnected()
{
    timerCzekajNaPolaczenie->start();
    emit rozloczono();
}

inline bool Klient::czyPoloczony()
{
    if( socket->state() == QTcpSocket::ConnectedState )
        return true;
    return false;
}

void Klient::readyRead()
{
    QString data = socket->readAll();
    int id = pobierzID(data);

    if( id <= 100 ) {} // wiadomosci 'systemowe' - moze sie przyda

    // znajdz id
    for(QList<IKomunikator*>::iterator it = komunikatory.begin(); it != komunikatory.end(); it++)
    {
        if( (*it)->wezID() == id )
        {
            (*it)->odbierzWiadomosc(&data);
            komunikatory.erase(it);
            break;
        }
    }
}

inline int Klient::pobierzID(QString& data)
{
    QString id_s;
    int ncount = 1;
    QString::iterator dit = data.end();
    dit--;
    while( *dit != ':' )
    {
        ncount++;
        id_s.prepend(*dit);
        dit--;
    }
    data = data.left(data.count() - ncount);
    return id_s.toInt();
}
