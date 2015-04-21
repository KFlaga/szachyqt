#ifndef KLIENT_H
#define KLIENT_H

#include <QObject>
#include <QString>
#include <QTcpSocket>
#include <QString>
class Klient : public QObject
{
    Q_OBJECT
public:
    explicit Klient(QString nick, QObject *parent = 0);
    ~Klient();
    QTcpSocket *socket;

    QMap<QString,QTcpSocket*> map;

signals:
    void odebranoRuch(QString s);
    void odebranoNickiUserow(QStringList s);
    void showOkno(QString n);
    void odebranoZaproszenie(QString);
    void odebranoOdpowiedz(QString odp);

private slots:
    void connected();
    void disconnected();
    void readyRead();
    void write(QString text);
    void polacz();
    void rozlacz();

public slots:
    void wyslijZaproszenie(QString przeciwnik);
    void stworzPojedynek(QString przeciwnik);
    void odmow(QString przeciwnik);
    void wyslijRuch(QString ru);
private:
    QString nick;
};

#endif // KLIENT_H
