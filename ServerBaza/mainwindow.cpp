#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLabel>
#include <QMessageBox>
#include <QtSql>
#include <QtCore/qmath.h>
#include  <QNetworkInterface>

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

    db = QSqlDatabase::addDatabase("QODBC");
    //QString dsn = QString("DRIVER={SQL Server};SERVER=serwer1504606.home.pl;DATABASE=17499764_szachy;PWD=szachyonline123;UID=17499764_szachy");
    QString dsn = QString("DRIVER={SQL Server};SERVER=M4800;DATABASE=SzachyOnline");
    db.setDatabaseName(dsn);
    if(db.open())
    {
        ui->logger->append("Otwarto liste uzytkownikow");
    }
    else
    {
        ui->logger->append("Niepowodzenie otwarcia listy uzytkownikow");
    }



    odczytajListeUzytkownikow();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QList<QHostAddress> ipAddressesList =
            QNetworkInterface::allAddresses();

    for(int i = 0;i<ipAddressesList.size();i++)
    {
        qDebug() << ipAddressesList[i];
    }
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
    QMap<QTcpSocket*,Uzytkownik*>::iterator klient = poloczenia.find(nadawca); //uciekinier;

    if( klient.value() != NULL)
    {
        if(klient.value()->aktualnyPrzeciwnik != "")
        {
            QMap<QTcpSocket*,Uzytkownik*>::iterator kl = poloczenia.begin();
            while( kl != poloczenia.end() )
            {
                if( kl.value() != NULL && kl.value()->nick == klient.value()->aktualnyPrzeciwnik )
                {
                    //                qDebug() << "przeciwik: " << kl.value()->nick;
                    kl.key()->write(QByteArray::fromStdString( QString("walkower::20.").toStdString()));
                    kl.value()->aktualnyPrzeciwnik = "";
                   // kl.value()->status = 0;
                    break;
                }
                kl++;
            }

            // qDebug() <<  "Wyszedł: " <<  klient.value()->nick;  //uciekinier
            klient.value()->aktualnyPrzeciwnik = "";

            //aktaualizacja  rankingów
            QSqlQuery qry;
            db.open();
            klient.value()->ile_przeg++;
            kl.value()->ile_wqyg++;
            int xnew = wyznaczNowyRanking(kl.value()->ranking,klient.value()->ranking,1.0); //ranking opuszczonego gracza
            int ynew = wyznaczNowyRanking(klient.value()->ranking,kl.value()->ranking,0.0); //ranking uciekiniera
            qry.exec(QString("UPDATE Users SET ilosc_przegranych=%1,ranking=%2 WHERE id=%3;").arg(klient.value()->ile_przeg).arg(ynew).arg(klient.value()->id));
            qry.exec(QString("UPDATE Users SET ilosc_wygranych=%1,ranking=%2 WHERE id=%3;").arg(kl.value()->ile_wqyg).arg(xnew).arg(kl.value()->id));

            nadawca->write(QByteArray::fromStdString(QString("rank:"+QString::number(ynew) + ":40.").toStdString()));
            kl.value()->ranking = xnew;
            klient.value()->ranking = ynew;
            kl.key()->write(QByteArray::fromStdString(QString("rank:"+QString::number(xnew) + ":40.").toStdString()));

            qry.exec(QString("INSERT INTO Historia VALUES (%1,%2,%3)").arg(kl.value()->id).arg(klient.value()->id).arg(kl.value()->id));
            klient.value()->status = 0;
            db.close();


        }

        klient.value()->czyZalogowany = false;
        klient.value()->status = 0;
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
                if(klient.value()->status == 0 ) //wolny
                {
                    odpowiedz.append("true-ok");
                    ui->logger->append("Wysylanie zaproszenia");
                    klient.key()->write(QByteArray::fromStdString( QString("otrzymanozapr:" +
                                                                           poloczenia.find(nadawca).value()->nick
                                                                           + '-' + daneZapr[1] +":90.").toStdString()));
                }
                else if(klient.value()->status == 1)  //zajety
                {
                    odpowiedz.append("false-Użytkownik aktualnie przebywa w innej grze");
                }
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
                    klient.value()->status = 1;
                    klient.value()->aktualnyPrzeciwnik = poloczenia[nadawca]->nick;
                    break;
                }
                klient++;
            }
            if( klient != poloczenia.end() )
            {
                QString odp = QString("pojedynek:" + daneOdp[0] + '-' + daneOdp[0] + '-' + daneOdp[1] + ":90.");
                ui->logger->append("Pojedynek: " + daneOdp[0] + "-" + poloczenia.find(nadawca).value()->nick);
                nadawca->write(QByteArray::fromStdString(odp.toStdString()));
                poloczenia[nadawca]->status = 1;
                poloczenia[nadawca]->aktualnyPrzeciwnik = klient.value()->nick;
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
        // QMessageBox::information(this,wynik[0],nadawca);
        int id = poloczenia[nadawca]->id;
        int id2;
        QMap<QTcpSocket*,Uzytkownik*>::iterator klient = poloczenia.begin();
        while( klient != poloczenia.end() )
        {
            if( klient.value() != NULL && klient.value()->nick == wynik[0] )
            {
                id2 = klient.value()->id;
                break;
            }
            klient++;
        }
        QSqlQuery qry;

        if(db.open())
        {
            int w;
            if(wynik[1] == "1")
            {
                w = id;
            }
            else if(wynik[1] == "2")
            {
                w = -1;  //remis
            }
            int xnew;
            int ynew;
            qry.exec(QString("INSERT INTO Historia VALUES (%1,%2,%3)").arg(id).arg(id2).arg(w));
            if(w==-1)
            {
                klient.value()->ile_rem++;
                poloczenia[nadawca]->ile_rem++;
                xnew = wyznaczNowyRanking(poloczenia[nadawca]->ranking,klient.value()->ranking,0.5);
                ynew = wyznaczNowyRanking(klient.value()->ranking,poloczenia[nadawca]->ranking,0.5);
                qry.exec(QString("UPDATE Users SET ilosc_remisow=%1,ranking=%2 WHERE id=%3;").arg(klient.value()->ile_rem).arg(ynew).arg(id2));
                qry.exec(QString("UPDATE Users SET ilosc_remisow=%1,ranking=%2 WHERE id=%3;").arg(poloczenia[nadawca]->ile_rem).arg(xnew).arg(id));
            }
            else
            {
                klient.value()->ile_przeg++;
                poloczenia[nadawca]->ile_wqyg++;
                xnew = wyznaczNowyRanking(poloczenia[nadawca]->ranking,klient.value()->ranking,1.0);
                ynew = wyznaczNowyRanking(klient.value()->ranking,poloczenia[nadawca]->ranking,0.0);
                qry.exec(QString("UPDATE Users SET ilosc_przegranych=%1,ranking=%2 WHERE id=%3;").arg(klient.value()->ile_przeg).arg(ynew).arg(id2));
                qry.exec(QString("UPDATE Users SET ilosc_wygranych=%1,ranking=%2 WHERE id=%3;").arg(poloczenia[nadawca]->ile_wqyg).arg(xnew).arg(id));
            }
            klient.key()->write(QByteArray::fromStdString(QString("rank:"+QString::number(ynew) + ":40.").toStdString()));
            nadawca->write(QByteArray::fromStdString(QString("rank:"+QString::number(xnew) + ":40.").toStdString()));
            poloczenia[nadawca]->ranking = xnew;
            klient.value()->ranking = ynew;
           // klient.value()->status = 0;
           //poloczenia[nadawca]->status = 0;
            klient.value()->aktualnyPrzeciwnik = "";
            poloczenia[nadawca]->aktualnyPrzeciwnik = "";
        }
        db.close();
    }
    else if(data.startsWith("walkower:"))
    {
        QMap<QTcpSocket*,Uzytkownik*>::iterator klient = poloczenia.find(nadawca); //uciekinier;
        QMap<QTcpSocket*,Uzytkownik*>::iterator kl = poloczenia.begin();
        while( kl != poloczenia.end() )
        {
            if( kl.value() != NULL && kl.value()->nick == klient.value()->aktualnyPrzeciwnik )
            {
                kl.key()->write(QByteArray::fromStdString( QString("walkower::20.").toStdString()));
                kl.value()->aktualnyPrzeciwnik = "";
                //.value()->status = 0;
                break;
            }
            kl++;
        }

        klient.value()->aktualnyPrzeciwnik = "";
        klient.value()->status = 0;
       // klient.value()->status = 0;

        //aktaualizacja  rankingów
        QSqlQuery qry;
        db.open();
        klient.value()->ile_przeg++;
        kl.value()->ile_wqyg++;
        int xnew = wyznaczNowyRanking(kl.value()->ranking,klient.value()->ranking,1.0); //ranking opuszczonego gracza
        int ynew = wyznaczNowyRanking(klient.value()->ranking,kl.value()->ranking,0.0); //ranking uciekiniera
        qry.exec(QString("UPDATE Users SET ilosc_przegranych=%1,ranking=%2 WHERE id=%3;").arg(klient.value()->ile_przeg).arg(ynew).arg(klient.value()->id));
        qry.exec(QString("UPDATE Users SET ilosc_wygranych=%1,ranking=%2 WHERE id=%3;").arg(kl.value()->ile_wqyg).arg(xnew).arg(kl.value()->id));

        nadawca->write(QByteArray::fromStdString(QString("rank:"+QString::number(ynew) + ":40.").toStdString()));
        kl.value()->ranking = xnew;
        klient.value()->ranking = ynew;
        kl.key()->write(QByteArray::fromStdString(QString("rank:"+QString::number(xnew) + ":40.").toStdString()));


        qry.exec(QString("INSERT INTO Historia VALUES (%1,%2,%3)").arg(kl.value()->id).arg(klient.value()->id).arg(kl.value()->id));

        db.close();
    }
    else if(data.startsWith("zakonczono:"))
    {
        poloczenia[nadawca]->status = 0;
    }
    else if(data.startsWith("szukaj:"))
    {
        data = data.mid(7);
        QStringList list = data.split("-");
        if(list[0] == "0")
        {
          poloczenia[nadawca]->status = 2;
          poloczenia[nadawca]->jakiCzas =  list[1].toInt();
          QMap<QTcpSocket*,Uzytkownik*>::iterator klient = poloczenia.begin();
          while( klient != poloczenia.end() )
          {
              if( klient.value() != NULL)
              {
                  if(klient.value()->status == 2 && (klient.value()->nick != poloczenia[nadawca]->nick) && poloczenia[nadawca]->jakiCzas == klient.value()->jakiCzas) //jesli tez szuka
                  {
                      QString odp = QString("pojedynek:" + poloczenia.find(nadawca).value()->nick
                                            + '-' +klient.value()->nick + '-' + "5" + ":90.");
                      klient.key()->write(QByteArray::fromStdString(odp.toStdString()));
                      klient.value()->status = 1;
                      klient.value()->aktualnyPrzeciwnik = poloczenia[nadawca]->nick;


                      odp = QString("pojedynek:" + klient.value()->nick + '-' + klient.value()->nick + '-' + "10" + ":90.");
                      nadawca->write(QByteArray::fromStdString(odp.toStdString()));
                      poloczenia[nadawca]->status = 1;
                      poloczenia[nadawca]->aktualnyPrzeciwnik = klient.value()->nick;
                     // qDebug() << "Start!";
                    break;
                  }
              }
              klient++;
          }

        }
        else
        {
          poloczenia[nadawca]->status = 0;
        }
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
    QSqlQuery qry;
    if(qry.exec("SELECT * FROM Users"))
    {
        while(qry.next())
        {
            Uzytkownik* nowy = new Uzytkownik();
            nowy->id = qry.value(0).toInt();
            nowy->login = qry.value(1).toString();
            nowy->haslo = qry.value(2).toString();
            nowy->nick = qry.value(3).toString();
            nowy->ile_wqyg = qry.value(4).toInt();
            nowy->ile_przeg = qry.value(5).toInt();
            nowy->ile_rem = qry.value(6).toInt();
            nowy->ranking = qry.value(7).toInt();
            nowy->czyZalogowany = false;
            nowy->status = 0;
            uzytkownicy.append(nowy);
            ui->logger->append("Wczytany uzytkownik: " + nowy->nick);
        }
    }
    db.close();
}

void MainWindow::zapiszUzytkownika(Uzytkownik* nowy)
{
    QSqlQuery qry;

    if(db.open())
    {
        qry.exec(QString("INSERT INTO Users VALUES ('%1','%2','%3',0,0,0,1000)").arg(nowy->login).arg(nowy->haslo).arg(nowy->nick));
    }
    db.close();
}

int MainWindow::wyznaczNowyRanking(int xr, int yr, double wynik) //gracz, przeciwnik, wynik (0 jeśli gracz X przegrał, 0.5 jeśli zremisował, 1 jeśli wygrał )
{
    int d = yr - xr;
    double We = 1/(1+qPow(10,(d/400.0)));
    double Diff = wynik - We;
    int k; //wsp rozwoju
    if(xr < 2100)
    {
        k = 32;
    }
    else if(xr < 2400)
    {
        k = 24;
    }
    else
    {
        k = 16;
    }
    return xr + (k*Diff);
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
            dane.append("-"+QString::number(user->status));
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
