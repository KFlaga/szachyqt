#include "klient.h"
#include <QMessageBox>
#include <QPushButton>
/*
 * Zalożenie jest takie, że tworzmy obiekt Klient przesyłając do konstruktora jako
 * argument nick, ktory bedzie pobrany z bazy danych
 * Wysylanie i odbieranie danych będzie opierać się na podłączeniu odpowiednich slotów i sygnałów
 *
 * Zaraz po stworzeniu obiektu do serwera wysylany jest nick gracza
 * Zaraz po utworzeniu obiektu należy podłączyć odpowiednie sygnały do  slotów:
 * odebranoNickiUserow, odebranoRuch, odebranoZaproszenie, odebranoOdpowiedz
 * Nicki są odbierane w postaci listy QStringList
 *
 * Wysylanie zaproszenia: wywołujemy metodę wyslijZaproszenie przesylajac jako argument nick przeciwnika
 * Odbieranie zaproszenia: po odbraniu zaproszenia nalezy wyslac odpowiednia odpowiedź - zgoda / brak zgody
 * Zgoda:Wysylamy - wywołujemy stworzPojedynek  i jako argument dajemy nasz nick
 * Brak zgody - wywolujemy metodę odmow i jako argument dajemy nasz nick
 * Odbieranie odpowiedzi: nick_gracza-false/true np "max-false" oznacza że gracz o nicku max oodrzucił nasze zaproszenei do gry
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

void Klient::wyslijZaproszenie(QString przeciwnik)
{
    QString t = "zaproszenie:"+przeciwnik;
    const char* data = t.toStdString().c_str();
    socket->write(data);
    socket->flush();
}

/*
 * Wysylanie Zaproszenia uzytkownikowi
*/
void Klient::stworzPojedynek(QString przeciwnik)
{
    QString t = "pojedynek:"+przeciwnik+"-"+nick+"-";
    const char* data = t.toStdString().c_str();
    socket->write(data);
    socket->flush();
}

void Klient::odmow(QString przeciwnik)
{
    QString t = "pojedynek:"+przeciwnik+"-"+nick+"-false";
    const char* data = t.toStdString().c_str();
    socket->write(data);
    socket->flush();
}

void Klient::wyslijRuch(QString ru)
{
    QString data = "move:"+ru;
    socket->write(data.toStdString().c_str());
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

    else if(data.startsWith("OtrzymanoZaproszenie:"))
    {
        emit odebranoZaproszenie(data.mid(21));
    }

    else if(data.startsWith("Odp:"))
    {
        emit odebranoOdpowiedz(data.mid(4));
    }
    else if(data.startsWith("move:"))
    {
         data = data.mid(5);
         emit odebranoRuch(data);
    }
}

