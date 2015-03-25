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
    opcje = new OknoOpcji(this);
    max_czas_biale=300;
    max_czas_czarne=300;
    ui->setupUi(this);
    pola=new QString*[8];
    for(int i=0;i<8;i++){
        pola[i]=new QString[8];
        for(int j=0;j<8;j++){
            pola[i][j]="0";
        }
    }
    //ustawiam figury na planszy
    //strona: 0 -biale, 1 - czarne
    pola[0][0]="W";
    pola[0][7]="W";
    pola[7][0]="w";
    pola[7][7]="w";
    figury.append(new Wieza(0,0,1,"W"));
    figury.append(new Wieza(7,0,1,"W"));
    figury.append(new Wieza(0,7,0,"w"));
    figury.append(new Wieza(7,7,0,"w"));
    pola[0][1]="S";
    pola[0][6]="S";
    pola[7][1]="s";
    pola[7][6]="s";
    figury.append(new Skoczek(1,0,1,"S"));
    figury.append(new Skoczek(6,0,1,"S"));
    figury.append(new Skoczek(1,7,0,"s"));
    figury.append(new Skoczek(6,7,0,"s"));
    pola[0][2]="G";
    pola[0][5]="G";
    pola[7][2]="g";
    pola[7][5]="g";
    figury.append(new Goniec(2,0,1,"G"));
    figury.append(new Goniec(5,0,1,"G"));
    figury.append(new Goniec(2,7,0,"g"));
    figury.append(new Goniec(5,7,0,"g"));
    pola[0][3]="H";
    pola[7][3]="h";
    figury.append(new Hetman(3,0,1,"H"));
    figury.append(new Hetman(3,7,0,"h"));
    pola[0][4]="K";
    pola[7][4]="k";
    figury.prepend(new Krol(4,0,1,"K"));
    figury.prepend(new Krol(4,7,0,"k"));
    for(int i=0;i<8;i++){
        pola[1][i]="P";
        pola[6][i]="p";
        figury.append(new Pionek(i,1,1,"P"));
        figury.append(new Pionek(i,6,0,"p"));
    }

    int size = 50 ;
    QVBoxLayout *main=new QVBoxLayout();
    main=new QVBoxLayout();
    this->centralWidget()->setLayout(main);

    QHBoxLayout *czas=new QHBoxLayout();
    main->addLayout(czas);

    wiadomosc=new QLabel("Zaczynają biale");
    wiadomosc->setFixedWidth(450);
    wiadomosc->setAlignment(Qt::AlignHCenter);
    QFont f;
    f.setBold(true);
    f.setPixelSize(20);
    wiadomosc->setFont(f);
    timerLabel=new QLabel("Biale czas: "+przelicz_czas(max_czas_biale));
    timerLabel2=new QLabel("Czarne czas: "+przelicz_czas(max_czas_czarne));

    czas->setSpacing(150);
    czas->setContentsMargins(50,0,0,0);
    czas->setAlignment(Qt::AlignLeft);
    czas->addWidget(timerLabel);
    czas->addWidget(timerLabel2);


    //pola czarno - biale
    QGridLayout *grid=new QGridLayout();
    main->addLayout(grid);
    grid->setSpacing(0);
    QLabel *tmp1,*tmp2,*tmp3,*tmp4;                 //obramowanie planszy
    tmp1=new QLabel();
    tmp1->setFixedWidth(size/2);
    tmp1->setFixedHeight(size/2);
    tmp1->setAlignment(Qt::AlignCenter);
    tmp1->setStyleSheet("background-color: #808080");
    grid->addWidget(tmp1,0,0);
    tmp2=new QLabel();
    tmp2->setFixedWidth(size/2);
    tmp2->setFixedHeight(size/2);
    tmp2->setAlignment(Qt::AlignCenter);
    tmp2->setStyleSheet("background-color: #808080");
    grid->addWidget(tmp2,0,9);
    tmp3=new QLabel();
    tmp3->setFixedWidth(size/2);
    tmp3->setFixedHeight(size/2);
    tmp3->setAlignment(Qt::AlignCenter);
    tmp3->setStyleSheet("background-color: #808080");
    grid->addWidget(tmp3,9,0);
    tmp4=new QLabel();
    tmp4->setFixedWidth(size/2);
    tmp4->setFixedHeight(size/2);
    tmp4->setAlignment(Qt::AlignCenter);
    tmp4->setStyleSheet("background-color: #808080");
    grid->addWidget(tmp4,9,9);
    QButtonGroup *bg=new QButtonGroup();
    pb=new QPushButton*[64];
    for(int i=0;i<64;i++){
        pb[i]=new QPushButton();//pola[i/8][i%8]);
        pb[i]->setFixedWidth(size);
        pb[i]->setFixedHeight(size);
        pb[i]->setAutoFillBackground(true);
        pb[i]->setIconSize(QSize(size-10,size-10));
        if(i%2==(i/8)%2)
                pb[i]->setStyleSheet("background-color: rgb(255, 255, 255); color: rgb(0, 0, 0)");
            else
                pb[i]->setStyleSheet("background-color: rgb(0, 0, 0); color: rgb(255, 255, 255)");
        grid->addWidget(pb[i],i/8+1,i%8+1);
        bg->addButton(pb[i],i);
    }
    connect(bg,SIGNAL(buttonClicked(int)),this,SLOT(clicked(int)));



    for(int i=0;i<8;i++){
        grid->setRowMinimumHeight(i+1,size);
        QString tmp;
        switch (i) {
        case 0:
            tmp="A";
            break;
        case 1:
            tmp="B";
            break;
        case 2:
            tmp="C";
            break;
        case 3:
            tmp="D";
            break;
        case 4:
            tmp="E";
            break;
        case 5:
            tmp="F";
            break;
        case 6:
            tmp="G";
            break;
        case 7:
            tmp="H";
            break;
        default:
            break;
        }

        //dodanie liter wokół planszy
        QLabel *l1=new QLabel("<b>"+tmp+"</b>");
        l1->setFixedWidth(size);
        l1->setFixedHeight(size/2);
        l1->setAlignment(Qt::AlignCenter);
        l1->setStyleSheet("background-color: #808080; color: #FFFFFF; ");
        QLabel *l2=new QLabel("<b>"+tmp+"</b>");
        l2->setFixedWidth(size);
        l2->setFixedHeight(size/2);
        l2->setAlignment(Qt::AlignCenter);
        l2->setStyleSheet("background-color: #808080; color: #FFFFFF; ");
        grid->addWidget(l1,0,i+1);
        grid->addWidget(l2,9,i+1);

        //Dodanie cyfr wokół planszy
        QLabel *l3=new QLabel("<b>"+QString::number(8-i)+"</b>");
        l3->setFixedWidth(size/2);
        l3->setFixedHeight(size);
        l3->setAlignment(Qt::AlignCenter);
        l3->setStyleSheet("background-color: #808080; color: #FFFFFF; ");
        QLabel *l4=new QLabel("<b>"+QString::number(8-i)+"</b>");
        l4->setFixedWidth(size/2);
        l4->setFixedHeight(size);
        l4->setAlignment(Qt::AlignCenter);
        l4->setStyleSheet("background-color: #808080; color: #FFFFFF; ");
        grid->addWidget(l3,i+1,0);
        grid->addWidget(l4,i+1,9);
    }
    main->addWidget(wiadomosc);
    timer = new QTimer();

    connect(timer,SIGNAL(timeout()),this,SLOT(timeout()));
    //ustawienie ikon na planszy
    for(int i=0;i<figury.size();i++){
        pb[figury[i]->getId()]->setIcon(figury[i]->ikona);
    }
    zaznaczony = -1;
    tura = false;
    connect(opcje,SIGNAL(nowa_gra()),this,SLOT(nowa_gra()));
    czy_koniec = true;
}

OknoGry::~OknoGry()
{
    delete ui;
}



void OknoGry::clicked(int id)
{
    if(czy_koniec) return;
    if(zaznaczony==-1)
    {                     //sprawdzamy czy jakaś figura jest zaznaczona
        for(int i=0;i<figury.size();i++)   //szukamy zaznaczonej figury
        {
            if(figury[i]->getId()==id)
            {
                //qDebug()<<"ZNALEZIONA FIGURA: "<<figury[i]->rodzaj;
                if(figury[i]->strona!=tura) return;
                pomocnicza=figury[i];
                if(id%2==(id/8)%2)              //ustawiamy jej inny kolor podświetlenia
                        pb[id]->setStyleSheet("border:8px solid #00ff00;background-color: rgb(255, 255, 255);");
                else
                        pb[id]->setStyleSheet("border:8px solid #00ff00;background-color: rgb(0, 0, 0);");

                QList<int> tmp;
                tmp=figury[i]->dostepneRuchy(pola);

                for(int j=0;j<tmp.length();j++)  //zaznaczamy na planszy wszystkie dostępne ruchy dla danej figury
                {
                    if(tmp.at(j)%2==(tmp.at(j)/8)%2)
                        pb[tmp.at(j)]->setStyleSheet("border:5px solid #ff0000;background-color: rgb(255, 255, 255);");
                    else
                        pb[tmp.at(j)]->setStyleSheet("border:5px solid #ff0000;background-color: rgb(0, 0, 0);");
                }
                zaznaczony = id;
                break;
            }
        }
    }
    else
    {
        bool rozne=(zaznaczony==id);
        if(!rozne) //jesli zaznaczone pola są różne
        {
            if(pomocnicza->poprawny_ruch(id))
            {
                pola[id/8][id%8]=pola[zaznaczony/8][zaznaczony%8];          //w tablicy z polami zamieniamy pola z klikniętego napierw na ten następny
                pola[zaznaczony/8][zaznaczony%8]="0";
                pb[zaznaczony]->setIcon(QIcon());
                pb[id]->setIcon(pomocnicza->ikona);
                pomocnicza->setId(id);
                tura = !tura;
            }

        }
        for(int i=0;i<pomocnicza->dr.length();i++){
            if(pomocnicza->dr.at(i)%2==(pomocnicza->dr.at(i)/8)%2)
                    pb[pomocnicza->dr.at(i)]->setStyleSheet("border:0px solid #ff0000; background-color: rgb(255, 255, 255); color: rgb(0, 0, 0)");
                else
                    pb[pomocnicza->dr.at(i)]->setStyleSheet("border:0px solid #ff0000; background-color: rgb(0, 0, 0); color: rgb(255, 255, 255)");
        }
        //musimy przywrócić kolor pola bez figury
    if(zaznaczony%2==(zaznaczony/8)%2)
            pb[zaznaczony]->setStyleSheet("border:0px dotted #ff0000; background-color: rgb(255, 255, 255); color: rgb(0, 0, 0)");
        else
            pb[zaznaczony]->setStyleSheet("border:0px dotted #ff0000; background-color: rgb(0, 0, 0); color: rgb(255, 255, 255)");

          QString t="biale";
          if(tura==1) t="czarne";
          wiadomosc->setText("Tura: "+t);
          zaznaczony = -1;
    }
}



void OknoGry::timeout() //do odliczania pozsotałego czasu
{
    if(czy_koniec) return;
    if(tura==0)
    {
        max_czas_biale--;
        timerLabel->setText("Biale czas: "+przelicz_czas(max_czas_biale));
        if(max_czas_biale==0)
        {
            czy_koniec=true;
            wiadomosc->setText("Koniec czasu!");
            //wyswietlenie wiadomosci o koncu partii
            QMessageBox::warning(this, "Szachy","Koniec czasu. Wygrywają: czarne",QMessageBox::Ok);
        }
    }
    else
    {
        max_czas_czarne--;
        timerLabel2->setText("Czarne czas: "+przelicz_czas(max_czas_czarne));
        if(max_czas_czarne==0)
        {
            czy_koniec=true;
            wiadomosc->setText("Koniec czasu!");
            QMessageBox::warning(this, "Szachy","Koniec czasu. Wygrywają: biale",QMessageBox::Ok);
        }
    }
}




void OknoGry::nowa_gra()
{
    if(opcje->gra_z_kompem())return;  //w chwili obecnej nie pozwalamy na gre z kompem
    timer->stop();
    tura=0;
    czy_koniec=false;
    int max=opcje->max_czas();
    if(max==0) max=300;
    else if(max==1) max=600;
    else if(max==2) max=900;
    else max=1800;
    max_czas_biale=max;
    max_czas_czarne=max;

    figury.clear();
    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){
            pola[i][j]="0";
            pb[i*8+j]->setIcon(QIcon());
        }
    }
    pola[0][0]="W";
    pola[0][7]="W";
    pola[7][0]="w";
    pola[7][7]="w";
    figury.append(new Wieza(0,0,1,"W"));
    figury.append(new Wieza(7,0,1,"W"));
    figury.append(new Wieza(0,7,0,"w"));
    figury.append(new Wieza(7,7,0,"w"));
    pola[0][1]="S";
    pola[0][6]="S";
    pola[7][1]="s";
    pola[7][6]="s";
    figury.append(new Skoczek(1,0,1,"S"));
    figury.append(new Skoczek(6,0,1,"S"));
    figury.append(new Skoczek(1,7,0,"s"));
    figury.append(new Skoczek(6,7,0,"s"));
    pola[0][2]="G";
    pola[0][5]="G";
    pola[7][2]="g";
    pola[7][5]="g";
    figury.append(new Goniec(2,0,1,"G"));
    figury.append(new Goniec(5,0,1,"G"));
    figury.append(new Goniec(2,7,0,"g"));
    figury.append(new Goniec(5,7,0,"g"));
    pola[0][3]="H";
    pola[7][3]="h";
    figury.append(new Hetman(3,0,1,"H"));
    figury.append(new Hetman(3,7,0,"h"));
    pola[0][4]="K";
    pola[7][4]="k";
    figury.prepend(new Krol(4,0,1,"K"));
    figury.prepend(new Krol(4,7,0,"k"));
    for(int i=0;i<8;i++){
        pola[1][i]="P";
        pola[6][i]="p";
        figury.append(new Pionek(i,1,1,"P"));
        figury.append(new Pionek(i,6,0,"p"));
    }

    wiadomosc->setText("Zaczynają biale");

    timerLabel->setText("Biale czas: "+przelicz_czas(max_czas_biale));
    timerLabel2->setText("Czarne czas: "+przelicz_czas(max_czas_czarne));

    for(int i=0;i<64;i++)
    {
        if(i%2==(i/8)%2)
                pb[i]->setStyleSheet("background-color: rgb(255, 255, 255); color: rgb(0, 0, 0)");
        else
                pb[i]->setStyleSheet("background-color: rgb(0, 0, 0); color: rgb(255, 255, 255)");
    }
    zaznaczony=-1;

    for(int i=0;i<figury.size();i++)
    {
        pb[figury[i]->getId()]->setIcon(figury[i]->ikona);
    }
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
    opcje->open();
}
