#ifndef KLIENT_H
#define KLIENT_H

#include <QObject>
#include <QString>
#include <QTcpSocket>
#include <QString>
#include "ikomunikator.h"
#include <QList>
#include <QTimer>
#include "logger.h"

class Klient : public QObject
{
    Q_OBJECT
public:
    Klient();
    Klient::~Klient();

    bool czyPoloczony();

private:
    int pobierzID(QString& dane);
    void odbierzWiadomoscWewnatrz(QString& dane);

public slots:
    void wyslijWiadomosc(const QString &tresc, IKomunikator* kom);
    void wyslijWiadomosc(const QString& tresc, int id);
    void polacz();
    void rozlacz();

signals:
    void odebranoRuch(QString& dane);
    void otrzymanoZaproszenie(QString& dane);
    void zacznijPojedynek(QString& dane);
    void zakonczonoPojedynek(QString& dane);
    void odmowaPojedynku(QString& dane);
    void anulujPojedynek(QString& dane);

private slots:
    void connected();
    void disconnected();
    void readyRead();
    void socketError(QAbstractSocket::SocketError);

signals:
    void rozloczono();
    void poloczono();
    void niepowodzeniePoloczenia(int);

private:
    QTcpSocket *socket;
    // QMap<QString,QTcpSocket*> map;
    QList<IKomunikator*> komunikatory;
    QTimer* timerCzekajNaPolaczenie;
    Logger* log;
};

#endif // KLIENT_H
