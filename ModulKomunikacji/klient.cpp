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
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(socketError(QAbstractSocket::SocketError)));

    log = new Logger();
    log->show();
}

Klient::~Klient()
{
    socket->close();
    socket->deleteLater();
}

void Klient::wyslijWiadomosc(const QString& text, IKomunikator* kom)
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
    QString tmp = text;
    socket->write( QByteArray::fromStdString(tmp.append(':').
                                             append(QString::number(kom->wezID()))
                                             .toStdString()) );
    socket->flush();
    log->dodajLog("Wyslano wiadomosc (kom): ");
    log->dodajLog(text + ':' + kom->wezID());
}

void Klient::wyslijWiadomosc(const QString &text, int id)
{
    if( socket->state() != QTcpSocket::ConnectedState )
    {
        return;
    }
    QString tmp = text;
    socket->write( QByteArray::fromStdString(tmp.append(':').
                                             append(QString::number(id))
                                             .toStdString()) );
    socket->flush();
    log->dodajLog("Wyslano wiadomosc: ");
    log->dodajLog(text);
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
        log->dodajLog("Laczenie");
    }
}

void Klient::rozlacz()
{
    socket->close();
}

void Klient::connected()
{
    timerCzekajNaPolaczenie->stop();
    log->dodajLog("Poloczono");
    emit poloczono();
}

void Klient::disconnected()
{
    timerCzekajNaPolaczenie->start();
    log->dodajLog("Rozlaczono");
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
    log->dodajLog("Otrzymano wiadomosc: " + data);
    int id = pobierzID(data);

    if( id < 100 ) // 100 = pusta wiadomosc
    {
        // Dla mniejszych od 100 przesyłamy to dalej
        odbierzWiadomoscWewnatrz(data);
        return;
    }

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

int Klient::pobierzID(QString& data)
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

void Klient::odbierzWiadomoscWewnatrz(QString& dane)
{
    if( dane.startsWith("otrzymanozapr:") )
    {
        emit otrzymanoZaproszenie(dane.mid(14));
    }
    else if( dane.startsWith("pojedynek:") )
    {
        emit zacznijPojedynek(dane.mid(10));
    }
    else if( dane.startsWith("odmowa:") )
    {
        emit odmowaPojedynku(dane.mid(7));
    }
    else if( dane.startsWith("anulujpojedynek:"))
    {
        emit anulujPojedynek(dane.mid(16));
    }
    else if( dane.startsWith("ruch:") )
    {
        //emit otrzymanoRuch(dane.mid(5));
    }
    else if( dane.startsWith("koniecpojedynku:"))
    {
        emit zakonczonoPojedynek(dane.mid(16));
    }
    else if( dane.startsWith("test:"))
    {
        log->dodajLog("TEST");
    }
}

void Klient::socketError(QAbstractSocket::SocketError e)
{
    log->dodajLog("Blad: " + socket->errorString());
}
