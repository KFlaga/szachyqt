#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLabel>
#include <QMessageBox>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    plikListaUzytkownikow("D:\\DaneUzytkownikow")
{
    ui->setupUi(this);
    server = new QTcpServer(this);
    connect(server,SIGNAL(newConnection()),this,SLOT(newConnection()));
    l = new QLabel();
    ui->statusBar->addWidget(l);
    ui->pushButton_2->setDisabled(true);
    ile = 0;
    ui->lineEdit->setText(QString::number(ile));

    plikListaUzytkownikow.open(QFile::ReadWrite | QFile::Append);
    if( plikListaUzytkownikow.isOpen() )
        ui->logger->append("Otwarto liste uzytkownikow");
    else
        ui->logger->append("Niepowodzenie otwarcia listy uzytkownikow");

    odczytajListeUzytkownikow();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    if(!server->listen(QHostAddress::Any,2222))
    {
        l->setText("Serwer nie wystartował");
    }
    else
    {
        l->setText("Serwer wystartował");
        ui->pushButton->setDisabled(true);
        ui->pushButton_2->setDisabled(false);
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    server->close();
    l->setText(tr("Serwer wyłączony"));
    ui->pushButton->setDisabled(false);
    ui->pushButton_2->setDisabled(true);
}

void MainWindow::on_pushButton_3_clicked()
{
    server->close();
    close();
}

void MainWindow::newConnection()
{
    ui->logger->append("Nowe polaczenie");
    QTcpSocket *s = server->nextPendingConnection();
    poloczenia.insert(s,NULL);
    connect(s,SIGNAL(readyRead()),this,SLOT(readyRead()));
    connect(s, SIGNAL(disconnected()), this, SLOT(removeClient()));
    ile++;
    ui->lineEdit->setText(QString::number(ile));
}

void MainWindow::removeClient()
{
    QTcpSocket *tt = (QTcpSocket*)sender();
    poloczenia.remove(tt);
    ile--;
    ui->lineEdit->setText(QString::number(ile));
    sendUsersList();

    //trzeba bedzie jeszcze usunac Pojedynek z listy jesli uzytkownik gral i wyszedl
}

void MainWindow::readyRead()
{
    ui->logger->append("Otrzymano wiadomosc:");

    QTcpSocket* nadawca = (QTcpSocket*)sender();
    QString data = nadawca->readAll();
    ui->logger->append(data);

    int id = pobierzID(data);
    if(data.startsWith("register:"))
    {
        data = data.mid(9);
        QString odpowiedz = "register:";
        bool czyLoginOk = true, czyNickOk = true;
        QStringList daneUzytkownika = data.split("-");
        for(int u = 0; u < uzytkownicy.size(); u++)
        {
            if( uzytkownicy[u]->login == daneUzytkownika[0] )
                czyLoginOk = false;
            if( uzytkownicy[u]->nick == daneUzytkownika[2] )
                czyNickOk = false;
        }
        if( czyLoginOk == true )
            odpowiedz.append("true-");
        else
            odpowiedz.append("false-");
        if( czyNickOk == true )
            odpowiedz.append("true");
        else
            odpowiedz.append("false");

        if( czyLoginOk && czyNickOk )
        {
            Uzytkownik* nowy = new Uzytkownik();
            nowy->login = daneUzytkownika[0];
            nowy->haslo = daneUzytkownika[1];
            nowy->nick = daneUzytkownika[2];
            nowy->ranking = 0;

            zapiszUzytkownika(nowy);
            uzytkownicy.append(nowy);
        }

        odpowiedz.append(":");
        odpowiedz.append(QString::number(id));
        ui->logger->append("Odpowiedz: " + odpowiedz);
        nadawca->write(odpowiedz.toStdString().c_str());
    }
    else if(data.startsWith("login:"))
    {
        data = data.mid(6);
        QString odpowiedz = "login:";
        bool czyDaneOk = false;
        Uzytkownik* logujacy;
        QStringList daneLogowania = data.split("-");
        for(int u = 0; u < uzytkownicy.size(); u++)
        {
            if( uzytkownicy[u]->login == daneLogowania[0] &&
                uzytkownicy[u]->haslo == daneLogowania[1] )
            {
                czyDaneOk = true;
                logujacy = uzytkownicy[u];
                break;
            }
        }
        if( czyDaneOk == false )
            odpowiedz.append("false");
        else
        {
            odpowiedz.append("true-");
            odpowiedz.append(logujacy->nick + '-');
            odpowiedz.append(QString::number(logujacy->ranking) + '-');
            if( logujacy->czyZalogowany == true )
                odpowiedz.append("true");
            else
                odpowiedz.append("false");

            // Zalogowanie - przypisanie nowego uzytkownika do socketa
            logujacy->czyZalogowany = true;
            QMap<QTcpSocket*,Uzytkownik*>::iterator polaczenie = poloczenia.find(nadawca);
            polaczenie.value() = logujacy;
            ui->logger->append("Zalogowano: " + logujacy->nick);
        }
        odpowiedz.append(":");
        odpowiedz.append(QString::number(id));
        ui->logger->append("Odpowiedz: " + odpowiedz);
        nadawca->write(odpowiedz.toStdString().c_str());
    }
    else if(data.startsWith("logout:"))
    {
        QMap<QTcpSocket*,Uzytkownik*>::iterator polaczenie = poloczenia.find(nadawca);
        polaczenie.value() = NULL;
    }
/*
    //jesi jest pojedynek:nick-nick to oznacza to nawe polaczenie miedzy userami
    else if(data.startsWith("pojedynek:"))
    {
        data = data.mid(10);
        if(data=="false")return;
        QStringList q = data.split("-");
        QString A = q[0];
        QString B = q[1];
        QTcpSocket *AA;
        QTcpSocket *BB;

        foreach(QTcpSocket *nam,users.keys())
        {
            if(users[nam] == A)
            {
                AA = nam;
            }
            else if(users[nam] == B)
            {
                BB=nam;
            }
        }
        if(q[2]!="")
        {
            AA->write(QString("Odp:false-"+users[BB]).toStdString().c_str());
            return;
        }
        AA->write(QString("Odp:true-"+users[BB]).toStdString().c_str());
        Pojedynek *poj = new Pojedynek(AA,BB);
        pojedynki.push_back(poj);
    }*/

    /*
    else if(data.startsWith("zaproszenie:"))
    {
        data = data.mid(12);
        foreach(QTcpSocket *nam,users.keys())
        {
            if(users[nam] == data)
            {
                QString o = "OtrzymanoZaproszenie:"+users[tt];qDebug()<<o;
                nam->write(o.toStdString().c_str());
                break;
            }
        }
    }
    */

    //oznacza ruch
    //move:id1-id2-prom
    /*
    else if(data.startsWith("move:"))
    {
        foreach(Pojedynek *p,pojedynki)
        {
            if(p->A==tt)
            {
                p->B->write(data.toStdString().c_str());
                p->B->flush();
            }
            else if(p->B==tt)
            {
                p->A->write(data.toStdString().c_str());
                p->A->flush();
            }
        }
    }*/

    else
        nadawca->write("empty::100");
}


void MainWindow::odczytajListeUzytkownikow()
{
    plikListaUzytkownikow.seek(0);
    QTextStream streamUzytkownicy(&plikListaUzytkownikow);
    while (!streamUzytkownicy.atEnd())
    {
       Uzytkownik* nowy = new Uzytkownik();
       streamUzytkownicy>> nowy->login >> nowy->haslo >>
               nowy->nick >> nowy->ranking;
       nowy->czyZalogowany = false;
       // nowy.status = 0; // statusy do zrobienia
       uzytkownicy.append(nowy);
       ui->logger->append("Wczytany uzytkownik: " + nowy->nick);
    }
}

void MainWindow::zapiszUzytkownika(Uzytkownik* nowy)
{
    plikListaUzytkownikow.seek(plikListaUzytkownikow.size());
    QTextStream streamUzytkownicy(&plikListaUzytkownikow);
       streamUzytkownicy << nowy->login << ' ' << nowy->haslo << ' ' <<
               nowy->nick << ' ' << nowy->ranking << '\r\n';
}

void MainWindow::sendUsersList()
{
    /*
    QStringList userList;
    foreach(QString user, users.values())
    {
         userList << user;
    }

    foreach(QTcpSocket *user, users.keys())
    {
       user->write(QString("users:" + userList.join(" ")).toStdString().c_str());
       user->flush();
    }
    */
}

int MainWindow::pobierzID(QString& data)
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
