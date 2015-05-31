#ifndef BUFORWIADOMOSCI_H
#define BUFORWIADOMOSCI_H


#include <QObject>
#include <QByteArray>
#include <QString>
#include <QList>
#include <QTcpSocket>

class BuforWiadomosci : public QObject
{
    Q_OBJECT
public:
    BuforWiadomosci(QTcpSocket* klient, char delim = '.');
    ~BuforWiadomosci();

private:
    void przetwarzajWiadomosci();

public slots:
    void wczytajDane();
    void start();
    void stop();

private slots:
    void sprawdzBufor();
    void sprawdzWiadomosci();

signals:
    void nowaWiadomosc(QString, QTcpSocket*);

private:
    QTcpSocket* socket;
    char przerwa;
    QList<QByteArray> bufor;
    QList<QString> wiadomosci;
    QString przetwarzanaWiadomosc;
    bool zatrzymaj;
    bool zakonczonoPrzetwarzanie;
};


#endif // BUFORWIADOMOSCI_H
