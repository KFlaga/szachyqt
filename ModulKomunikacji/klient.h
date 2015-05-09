#ifndef KLIENT_H
#define KLIENT_H

#include <QObject>
#include <QString>
#include <QTcpSocket>
#include "ikomunikator.h"
#include <QList>
#include <QTimer>
#include "logger.h"
#include <QThread>

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
    void wyslijRuch(const QString ruch);
    void polacz();
    void rozlacz();

signals:
    void odebranoRuch(QString dane);
    void otrzymanoZaproszenie(QString dane);
    void zacznijPojedynek(QString dane);
    void zakonczonoPojedynek(QString dane);
    void odmowaPojedynku(QString dane);
    void anulujPojedynek(QString dane);
    void buforStart();
    void buforStop();
    void noweDane(const QByteArray&);
    void zakonczonoPrzetwarzanie();

private slots:
    void connected();
    void disconnected();
    void socketError(QAbstractSocket::SocketError);
    void buforujWiadomosc();
    void przetworzWiadomosc(QString);
    void dodajLog(QString);

signals:
    void rozloczono();
    void poloczono();
    void niepowodzeniePoloczenia(int);

private:
    QTcpSocket *socket;
    QList<IKomunikator*> komunikatory;
    QTimer* timerCzekajNaPolaczenie;
    Logger* log;
    char delim;
    QThread* watekBufora;
};

#endif // KLIENT_H
