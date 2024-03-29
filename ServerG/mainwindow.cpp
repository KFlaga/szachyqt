#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLabel>
#include <QMessageBox>
#include <QtSql>

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

    delim = '.';

    //BAZA
    /*db = QSqlDatabase::addDatabase("QODBC");
    QString dsn = QString("DRIVER={SQL Server};SERVER=serwer1504606.home.pl;DATABASE=17499764_szachy;PWD=szachyonline123;UID=17499764_szachy");
    db.setDatabaseName(dsn);
    if(db.open())
    {
        ui->logger->append("Otwarto liste uzytkownikow");
    }
    else
    {
        ui->logger->append("Niepowodzenie otwarcia listy uzytkownikow");
    }*/

    //PLIK
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
    connect(s,SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketError(QAbstractSocket::SocketError)));
    ile++;
    ui->lineEdit->setText(QString::number(ile));
}

void MainWindow::removeClient()
{
    QTcpSocket *nadawca = (QTcpSocket*)sender();
    QMap<QTcpSocket*,Uzytkownik*>::iterator klient = poloczenia.find(nadawca);
    if( klient.value() != NULL)
    {
        klient.value()->czyZalogowany = false;
        klient.key()->abort();
        ui->logger->append("Rozloczono: " + klient.value()->nick);
    }
    poloczenia.erase(klient);

    ile--;
    ui->lineEdit->setText(QString::number(ile));
}

void MainWindow::readyRead()
{
    ui->logger->append("Otrzymano wiadomosc:");

    QTcpSocket* nadawca = (QTcpSocket*)sender();
    QString data = nadawca->readAll();
    ui->logger->append(data);
    int id;

    id = pobierzID(data);

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
        odpowiedz.append(delim);
        ui->logger->append("Odpowiedz: " + odpowiedz);
        nadawca->write(QByteArray::fromStdString(odpowiedz.toStdString()));
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
            poloczenia.find(nadawca).value() = logujacy;
            ui->logger->append("Zalogowano: " + logujacy->nick);
        }
        odpowiedz.append(":");
        odpowiedz.append(QString::number(id));
        odpowiedz.append(delim);
        ui->logger->append("Odpowiedz: " + odpowiedz);
        nadawca->write(QByteArray::fromStdString(odpowiedz.toStdString()));
    }
    else if(data.startsWith("logout:"))
    {
        QMap<QTcpSocket*,Uzytkownik*>::iterator klient = poloczenia.find(nadawca);
        ui->logger->append("Wylogowano: " + klient.value()->nick);
        klient.value()->czyZalogowany = false;
        klient.value() = NULL;
    }
    else if(data.startsWith("zadajlisty:"))
    {
        wyslijListeUzytkownikow(poloczenia.find(nadawca),id);
    }
    else if(data.startsWith("zaproszenie:"))
    {
        data = data.mid(12);
        QString odpowiedz = "zaproszenie:";
        // nick-czas
        QStringList daneZapr = data.split('-');
        // wyszukaj uzytkownika o podanym nicku wśród połączonych
        QMap<QTcpSocket*,Uzytkownik*>::iterator klient = poloczenia.begin();
        while( klient != poloczenia.end() )
        {
           if( klient.value() != NULL && klient.value()->nick == daneZapr[0] )
           {
                 // + if( status = wolny )
               odpowiedz.append("true-ok");
               ui->logger->append("Wysylanie zaproszenia");
               klient.key()->write(QByteArray::fromStdString( QString("otrzymanozapr:" +
                                    poloczenia.find(nadawca).value()->nick
                                    + '-' + daneZapr[1] +":90.").toStdString()));
               break;
           }
           klient++;
        }
        if( klient == poloczenia.end() ) // nie znalazlo uzytkownika
        {
            odpowiedz.append("false-Użytkownik niezalogowany");
        }
        odpowiedz.append(":");
        odpowiedz.append(QString::number(id));
        odpowiedz.append(delim);
        ui->logger->append("Odpowiedz: " + odpowiedz);
        nadawca->write(QByteArray::fromStdString(odpowiedz.toStdString()));
    }
    else if(data.startsWith("zapro_odp:"))
    {
        data = data.mid(10);
        QStringList daneOdp = data.split('-');
        if(daneOdp[2] == "true")
        {
            // wyslij do obu wiadomosc o pojedynku
            QMap<QTcpSocket*,Uzytkownik*>::iterator klient = poloczenia.begin();
            while( klient != poloczenia.end() )
            {
               if( klient.value() != NULL && klient.value()->nick == daneOdp[0] )
               {
                   QString odp = QString("pojedynek:" + poloczenia.find(nadawca).value()->nick
                                         + '-' +daneOdp[0] + '-' + daneOdp[1] + ":90.");
                   klient.key()->write(QByteArray::fromStdString(odp.toStdString()));
                   break;
               }
               klient++;
            }
            if( klient != poloczenia.end() )
            {
                QString odp = QString("pojedynek:" + daneOdp[0] + '-' + daneOdp[0] + '-' + daneOdp[1] + ":90.");
                ui->logger->append("Pojedynek: " + daneOdp[0] + "-" + poloczenia.find(nadawca).value()->nick);
                nadawca->write(QByteArray::fromStdString(odp.toStdString()));
                // W TYM MIEJSCU ZACZYNAMY POJEDYNEK
            }
            else
            {
                // w miedzy czasie sie wylogowal
                nadawca->write("anulujpojedynek:Wylogowany:90."); // mozna dodac powod
            }
        }
        else
        {
            // wyslij do nadawcy pojedynku wiadomosc o odrzuceniu zaproszenia
            QMap<QTcpSocket*,Uzytkownik*>::iterator klient = poloczenia.begin();
            while( klient != poloczenia.end() )
            {
               if( klient.value() != NULL && klient.value()->nick == daneOdp[0] )
               {
                   ui->logger->append("Wysylanie odmowy");
                   klient.key()->write("odmowa::90.");
                   break;
               }
               klient++;
            }
        }
    }
    //foramt ruch:nick-idz-iddo-PROM(ew)
    else if(data.startsWith("ruch:"))
    {
        data = data.mid(5);
        QStringList ruchy = data.split('-');
        // nick-czas
        // wyszukaj uzytkownika o podanym nicku wśród połączonych
        QMap<QTcpSocket*,Uzytkownik*>::iterator klient = poloczenia.begin();
        while( klient != poloczenia.end() )
        {
           if( klient.value() != NULL && klient.value()->nick == ruchy[0] )
           {
               ui->logger->append("Wysylano Ruch");
               if(ruchy.size() == 3)
               {
                   klient.key()->write(QByteArray::fromStdString(QString("ruch:"+ruchy[1]+"-"+ruchy[2]).toStdString()+":80."));
               }
               else
               {
                    klient.key()->write(QByteArray::fromStdString(QString("ruch:"+ruchy[1]+"-"+ruchy[2]+"-"+ruchy[3]).toStdString()+":80."));
               }
               klient.key()->flush();
               break;
           }
           klient++;
        }
    }
    else if(data.startsWith("wynik:"))
    {
        data = data.mid(6);
        QStringList wynik = data.split('-');
        // nick-czas
        // wyszukaj uzytkownika o podanym nicku wśród połączonych
        QMessageBox::information(this,wynik[0],wynik[1]);
    }
    else
    {
        nadawca->write("empty::100.");
    }
}

void MainWindow::socketError(QAbstractSocket::SocketError)
{
    ui->logger->append("Blad: " + ((QTcpSocket*)sender())->errorString());
}


void MainWindow::odczytajListeUzytkownikow()
{
    //PLIK
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

    //BAZA
   /* QSqlQuery qry;

        if(qry.exec("SELECT * FROM Gracze2"))
        {
            while(qry.next())
            {
                Uzytkownik* nowy = new Uzytkownik();
                nowy->login = qry.value(1).toString();
                nowy->haslo = qry.value(2).toString();
                nowy->nick = qry.value(3).toString();
                nowy->ranking = qry.value(7).toInt();
                nowy->czyZalogowany = false;
                uzytkownicy.append(nowy);
                ui->logger->append("Wczytany uzytkownik: " + nowy->nick);
            }
        }
    db.close();*/
}

void MainWindow::zapiszUzytkownika(Uzytkownik* nowy)
{
    //PLIK
    plikListaUzytkownikow.seek(plikListaUzytkownikow.size());
    QTextStream streamUzytkownicy(&plikListaUzytkownikow);
       streamUzytkownicy << nowy->login << ' ' << nowy->haslo << ' ' <<
               nowy->nick << ' ' << nowy->ranking << '\n';


    //BAZA
    /*QSqlQuery qry;

    if(db.open())
    {
       qry.exec(QString("INSERT INTO Gracze2 VALUES ('%1','%2','%3',0,0,0,100)").arg(nowy->login).arg(nowy->haslo).arg(nowy->nick));
    }
    db.close();*/
}

void MainWindow::wyslijListeUzytkownikow(QMap<QTcpSocket*,Uzytkownik*>::iterator nadawca, int id)
{
    QString odpowiedz = "listazwrot:";
    int liczbaU = 0;
    QString dane;
    foreach(Uzytkownik* user, poloczenia.values())
    {
         if( user != NULL && user != nadawca.value() )
         {
              dane.append('-'+user->nick);
              dane.append('-'+QString::number(user->ranking));
              dane.append("-status");
              liczbaU++;
         }
    }
    odpowiedz.append(QString::number(liczbaU));
    odpowiedz.append(dane);
    odpowiedz.append(':');
    odpowiedz.append(QString::number(id));
    odpowiedz.append(delim);
    ui->logger->append("Odpowiedz: " + odpowiedz);
    nadawca.key()->write(QByteArray::fromStdString(odpowiedz.toStdString()));
    nadawca.key()->flush();
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

void MainWindow::testPoloczen()
{
    QMap<QTcpSocket*,Uzytkownik*>::iterator klient = poloczenia.begin();
    while( klient != poloczenia.end() )
    {
       klient.key()->write("test::90.");
       klient.key()->flush();
       klient++;
    }
}
