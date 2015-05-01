#ifndef BUFORWIADOMOSCI_H
#define BUFORWIADOMOSCI_H

#include <QObject>
#include <QByteArray>
#include <QString>
#include <QList>

class BuforWiadomosci : public QObject
{
    Q_OBJECT
public:
    BuforWiadomosci(char delim = '.');
    ~BuforWiadomosci();

private:
    void przetwarzajWiadomosci();

public slots:
    void dodajDane(const QByteArray& dane);
    void start();
    void stop();
    void nadajKolejnaWiadomosc();

signals:
    void nowaWiadomosc(QString);
    void log(QString);

private:
    char przerwa;
    QList<QByteArray> bufor;
    QList<QString> wiadomosci;
    QString przetwarzanaWiadomosc;
    bool zatrzymaj;
    bool zakonczonoPrzetwarzanie;
};

#endif // BUFORWIADOMOSCI_H
