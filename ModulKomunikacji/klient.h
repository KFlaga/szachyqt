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
private slots:
    void connected();
    void disconnected();
    void readyRead();
    void write(QString text);
    void polacz();
    void rozlacz();

    void stworzPojedynek(QString przeciwnik);
private:
    QString nick;
};

#endif // KLIENT_H
