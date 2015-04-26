#include "klient.h"
#include <QMessageBox>
#include <QPushButton>

Klient::Klient() : QObject()
{
    socket = new QTcpSocket(this);
    connect(socket,SIGNAL(connected()),this,SLOT(connected()));
    connect(socket,SIGNAL(disconnected()),this,SLOT(disconnected()));
    connect(socket,SIGNAL(readyRead()),this,SLOT(readyRead()));
    polacz();
}

void Klient::wyslijWiadomosc(QString *text, IKomunikator* kom)
{
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
    text->append(":");
    text->append(QString::number(id));
    const char* data = text->toStdString().c_str();
    socket->write(data);
    socket->flush();
}

void Klient::polacz()
{
    //Łączenie...;
    socket->connectToHost("127.0.0.1",2222);

    if(!socket->waitForConnected(5000))
    {
        //Brak poloczenia
    }
    else
    {
        //Poloczono;
    }
}

void Klient::rozlacz()
{
    socket->close();
}

void Klient::connected()
{
    //QString t = "nick:"+nick;
    //const char* data = t.toStdString().c_str();
   // socket->write(data);
   // socket->flush();
}

void Klient::disconnected()
{
    //Rozlaczono
}

void Klient::readyRead()
{
    QString data = socket->readAll();
    int id = pobierzID(data);

    if( id <= 100 ); // wiadomosci 'systemowe' - moze sie przyda

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
    return data.toInt();
}
