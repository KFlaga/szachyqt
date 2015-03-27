#include "oknogry.h"
#include "ui_oknogry.h"
#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>
#include <QGridLayout>
#include <QMessageBox>

OknoGry::OknoGry(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::OknoGry)
{
    max_czas_biale=300;
    max_czas_czarne=300;
    ui->setupUi(this);

    inicjalizujUI();
    plansza = new Plansza(this);
    inicjalizujRamke();

    timer = new QTimer();
    connect(timer,SIGNAL(timeout()),this,SLOT(timeout()));
    tura = false;
}

void OknoGry::inicjalizujUI()
{
    mainLayout = new QVBoxLayout();

    this->centralWidget()->setLayout(mainLayout);

    QHBoxLayout *czasLayout=new QHBoxLayout();
    mainLayout->addLayout(czasLayout);

    wiadomoscTura = new QLabel("Zaczynają biale");
  //  wiadomoscTura->setFixedWidth(00);
    wiadomoscTura->setAlignment(Qt::AlignHCenter);
    QFont f;
    f.setBold(true);
    f.setPixelSize(20);
    wiadomoscTura->setFont(f);
    labTimerBiale = new QLabel("Biale czas: "+przelicz_czas(max_czas_biale));
    labTimerCzarne = new QLabel("Czarne czas: "+przelicz_czas(max_czas_czarne));

    czasLayout->setSpacing(150);
    czasLayout->setContentsMargins(50,0,0,0);
    czasLayout->setAlignment(Qt::AlignLeft);
    czasLayout->addWidget(labTimerBiale);
    czasLayout->addWidget(labTimerCzarne);
}

void OknoGry::inicjalizujRamke()
{
    QGridLayout *ramka = new QGridLayout();
    mainLayout->addLayout(ramka);
    ramka->setSpacing(0);
    int size = 50; // rozmiar pol ramki
    // Ustawienie szarych rogow planszy
    QLabel *rog;
    for(int r = 0; r < 4; r++)
    {
        rog = new QLabel();
        rog->setFixedWidth(size/2);
        rog->setFixedHeight(size/2);
        rog->setAlignment(Qt::AlignCenter);
        rog->setStyleSheet("background-color: #808080");
        ramka->addWidget(rog,(r/2)*9,(r%2)*9);
    }

    // Dodanie liter / cyfr wokol planszy
    for(int i=0;i<8;i++)
    {
        ramka->setRowMinimumHeight(i+1,size);
        QString litera = (QChar)('A'+i);

        //dodanie liter wokół planszy
        QLabel *l1 = new QLabel("<b>"+litera+"</b>");
        l1->setFixedWidth(size);
        l1->setFixedHeight(size/2);
        l1->setAlignment(Qt::AlignCenter);
        l1->setStyleSheet("background-color: #808080; color: #FFFFFF; ");
        QLabel *l2 = new QLabel("<b>"+litera+"</b>");
        l2->setFixedWidth(size);
        l2->setFixedHeight(size/2);
        l2->setAlignment(Qt::AlignCenter);
        l2->setStyleSheet("background-color: #808080; color: #FFFFFF; ");
        ramka->addWidget(l1,0,i+1);
        ramka->addWidget(l2,9,i+1);

        //Dodanie cyfr wokół planszy
        QLabel *l3 = new QLabel("<b>"+QString::number(8-i)+"</b>");
        l3->setFixedWidth(size/2);
        l3->setFixedHeight(size);
        l3->setAlignment(Qt::AlignCenter);
        l3->setStyleSheet("background-color: #808080; color: #FFFFFF; ");
        QLabel *l4 = new QLabel("<b>"+QString::number(8-i)+"</b>");
        l4->setFixedWidth(size/2);
        l4->setFixedHeight(size);
        l4->setAlignment(Qt::AlignCenter);
        l4->setStyleSheet("background-color: #808080; color: #FFFFFF; ");
        ramka->addWidget(l3,i+1,0);
        ramka->addWidget(l4,i+1,9);
    }
   // niech sie ramka z plansza nie rozszerzaja, a plansza zajmuje 8x8 komorek
    ramka->setSizeConstraint(QLayout::SetFixedSize);
    ramka->addWidget(plansza,1,1,8,8);
    mainLayout->addWidget(wiadomoscTura);
}

OknoGry::~OknoGry()
{
    delete ui;
    delete plansza;
}

Plansza* OknoGry::WezPlansze()
{
    return plansza;
}

void OknoGry::WykonanoRuch(int i)
{
    if(i==0)
    {
        if( tura == 0 )
        {
            tura = 1;
            wiadomoscTura->setText("Ruch czarnych");
        }
        else
        {
            tura = 0;
            wiadomoscTura->setText("Ruch bialych");
        }
    }
    else  if(i==1)
    {
        if( tura == 0 )
        {
            tura = 1;
            wiadomoscTura->setText("Szach! Ruch czarnych");
        }
        else
        {
            tura = 0;
            wiadomoscTura->setText("Szach! Ruch bialych");
        }
    }


    else if(i==2)
    {
        if( tura == 0 )
        {
            tura = 1;
            wiadomoscTura->setText("Szach mat! Wygrywaja biale");
            czy_koniec = true;
        }
        else
        {
            tura = 0;
            wiadomoscTura->setText("Szach mat! Wygrywaja czarne");
            czy_koniec = true;
        }
    }
    else if(i==3)
    {
        wiadomoscTura->setText("Pat! Remis");
        czy_koniec = true;
    }

}

void OknoGry::timeout() //do odliczania pozsotałego czasu
{
    if(czy_koniec) return;
    if(tura == 0)
    {
        max_czas_biale--;
        labTimerBiale->setText("Biale czas: "+przelicz_czas(max_czas_biale));
        if(max_czas_biale == 0)
        {
            czy_koniec=true;
            wiadomoscTura->setText("Koniec czasu!");
            //wyswietlenie wiadomosci o koncu partii
            QMessageBox::warning(this, "Szachy","Koniec czasu. Wygrywają: czarne",QMessageBox::Ok);
        }
    }
    else
    {
        max_czas_czarne--;
        labTimerCzarne->setText("Czarne czas: "+przelicz_czas(max_czas_czarne));
        if(max_czas_czarne==0)
        {
            czy_koniec=true;
            wiadomoscTura->setText("Koniec czasu!");
            QMessageBox::warning(this, "Szachy","Koniec czasu. Wygrywają: biale",QMessageBox::Ok);
        }
    }
}

void OknoGry::NowaGra(Opcje* opts)
{
    timer->stop();
    tura = 0;
    czy_koniec = false;
    int maxCzas = (opts->MaxCzas+1)*300;
    max_czas_biale = maxCzas+1;
    max_czas_czarne = maxCzas;

    wiadomoscTura->setText("Zaczynają biale");

    labTimerBiale->setText("Biale czas: "+przelicz_czas(max_czas_biale));
    labTimerCzarne->setText("Czarne czas: "+przelicz_czas(max_czas_czarne));

    plansza->Reset();

    timer->start(1000);
}

QString OknoGry::przelicz_czas(int t)
{
    QString tmp="";
    if(t>60){
        tmp=QString::number(t/60)+" m. ";
    }
    t=t-t/60*60;
    tmp+=QString::number(t)+" s.";
    return tmp;
}

void OknoGry::closeEvent(QCloseEvent *)
{
    qApp->closeAllWindows();
}

void OknoGry::on_actionNowa_Gra_triggered()
{

}
