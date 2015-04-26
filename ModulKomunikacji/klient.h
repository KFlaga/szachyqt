#ifndef KLIENT_H
#define KLIENT_H

#include <QObject>
#include <QString>
#include <QTcpSocket>
#include <QString>
#include "ikomunikator.h"
#include <QList>

class Klient : public QObject
{
    Q_OBJECT
public:
    Klient();

private:
    int pobierzID(QString& dane);

public slots:
    void wysliWiadomosc(QString* tresc, IKomunikator* kom);
    void polacz();
    void rozlacz();

signals:
    void odebranoRuch(QString s);

private slots:
    void connected();
    void disconnected();
    void readyRead();


private:
    QTcpSocket *socket;
    // QMap<QString,QTcpSocket*> map;
    QList<IKomunikator*> komunikatory;
};

#endif // KLIENT_H
