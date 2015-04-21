#include "klient.h"

/*
 * Zalożenie jest takie, że tworzmy obiekt przesyłając do konstruktora jako
 * argument nick, ktory bedzie pobrany z bazy danych
 * Wysylanie i odbieranie danych będzie opierać się na podłączeniu odpowiednich slotów i sygnałów
 */


Klient::Klient(QString nick, QObject *parent) : QObject(parent)
{
    socket = new QTcpSocket(this);
    this->nick = nick;
    connect(socket,SIGNAL(connected()),this,SLOT(connected()));
    connect(socket,SIGNAL(disconnected()),this,SLOT(disconnected()));
    connect(socket,SIGNAL(readyRead()),this,SLOT(readyRead()));
    polacz();
}

Klient::~Klient()
{

}

void Klient::write(QString text)
{
    const char* data = text.toStdString().c_str();
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

void Klient::stworzPojedynek(QString przeciwnik)
{
    QString t = "pojedynek:"+przeciwnik;
    const char* data = t.toStdString().c_str();
    socket->write(data);
    socket->flush();
}

void Klient::connected()
{
    QString t = "nick:"+nick;
    const char* data = t.toStdString().c_str();
    socket->write(data);
    socket->flush();
}

void Klient::disconnected()
{
    //Rozlaczono
}

void Klient::readyRead()
{
    QString data = socket->readAll();
    if(data.startsWith("users:"))
    {
        data=data.mid(6);
        QStringList list = data.split(" ");
        emit odebranoNickiUserow(list);
    }
    else if(data.startsWith("move:"))
    {
         data = data.mid(5);
         emit odebranoRuch(data);
    }
}

