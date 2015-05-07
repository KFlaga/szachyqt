#include "silnik.h"
#include <QVector>
#include "Figury/figura.h"
#include <QDebug>
#include <QPushButton>
#include <QLabel>
Silnik::Silnik()
{
    kreator =  new KreatorFigur();
    for(int i = 0; i < 64; i++)
        pola[i] = -1;


    msgBox=new QMessageBox(QMessageBox::Question,"Promocja pionka","Wybierz figurę:");      //przygotowanie okna z promocją
    hetmanButton = msgBox->addButton("Hetman",QMessageBox::ActionRole);
    goniecButton = msgBox->addButton("Goniec", QMessageBox::ActionRole);
    skoczekButton = msgBox->addButton("Skoczek", QMessageBox::ActionRole);
    wiezaButton = msgBox->addButton("Wieza", QMessageBox::ActionRole);


    czy_koniec = false;
}

Silnik::~Silnik()
{
    if(gra_z_kompem)
    {
        p->write("quit\n");
        p->waitForFinished();
        p->close();
        delete p;
    }
}

void Silnik::NowaGra(Opcje* opts)
{
    // Stworz pionki i dla kazdego emituj sygnal
    // Kolejnosc dodawania -> od zera, czyli gora-lewo, dla czarnych
    // potem od konca, czyli dol-prawo, w prawa strone dla bialych
    gra_z_kompem = false;
    gra_przez_siec = false;
    czyRuchPrzeciwnika = false;
    for(int i = 0; i < 16; i++)
    {
        Figura* fig = kreator->StworzFigure(i,1);
        figury.push_back(fig);
        pola[i] = i;
        emit DodanoFigureNaPole(i,&(fig->ikona));
    }

    for(int i = 16; i < 32; i++)
    {
        Figura* fig = kreator->StworzFigure(63-i+16,0);
        figury.push_back(fig);
        pola[63-i+16] = i;
        emit DodanoFigureNaPole(63-i+16,&(fig->ikona));
    }

    zaznaczonePole = -1;
    aktualnyGracz = 0;

    if(opts->CzyGraAI==true)
    {
        int lvl = opts->PoziomTrudnosci;
        p = new QProcess();
        connect(p,SIGNAL(readyRead()),this,SLOT(read()));
        gra_z_kompem = true;
        p->start("br/Brutus_7_02b_x64.exe");
        if(lvl==0){
            QByteArray ba=QString("sd 1\n").toLatin1();         //ustawiam poziom trudności, przeszukiwanie max 1 wgłąb
            p->write(ba);
        }else if(lvl==1){
            QByteArray ba=QString("sd 5\n").toLatin1();         //----------------------------------------------5------
            p->write(ba);
        }else{
            QByteArray ba=QString("st 3\n").toLatin1();         //--------------------------, przeszukiwanie max 3 sek
            p->write(ba);
        }
    }
    else if(opts->czyPrzezSiec == true)
    {
        gra_przez_siec = true;
        if(opts->Tura)
        {
            czyRuchPrzeciwnika = true;
        }
        connect(opts->klient,SIGNAL(odebranoRuch(QString)),this,SLOT(odebranoRuch(QString)));
        kl = opts->klient;
        przeciwnik = opts->przciwnik;
    }
}

void Silnik::PoleWcisniete(int nrPola)
{
    if(czy_koniec)return; //jesli koniec partii to nie pozwalam na ruuch
    if(gra_z_kompem && aktualnyGracz==1)return; //nie pozwalam na ruch kiedy gramy z kompem i nie jest nasza kolej
    if(czyRuchPrzeciwnika)return; //jesli jest ruch przeciwnika to nie pozwalam na ruch

    static QVector<int> zaznaczonePola;
    if( zaznaczonePole == nrPola ) // Odznaczamy
    {
        zaznaczonePole = -1;
        zaznaczonePola.clear();
    }
    else if( zaznaczonePole == -1) // zadne pole nie zaznaczone
    {
        // jak zaznaczony pionek swoj to go zaznacz i podswietl ruchy
        if( pola[nrPola] != -1 && figury[pola[nrPola]]->strona == aktualnyGracz )
        {
            zaznaczonePola.clear();
            zaznaczonePola = figury[pola[nrPola]]->dostepneRuchy(pola, &figury);
            zaznaczonePola.push_front(nrPola);
            zaznaczonePole = nrPola;
        }
        // jak inne pola to nic nie rob
    }
    else // klikniete pole inne niz zaznaczona figura
    {
        // Pierw sprawdzenie czy klikniecie na mozliwy ruch
        if( zaznaczonePola.contains(nrPola) )
        {
            // potem czy jest tam pionek - jak jest to bedzie to pionek przeciwnika
            if( pola[nrPola] != -1 )
            {
                // pionek przeciwnika - zbij i odznacz
                ZbijPionek(zaznaczonePole, nrPola); // arg: atakujacy / atakowany
                zaznaczonePole = -1;
                zaznaczonePola.clear();
            }
            else
            {
                // jak nie ma pionka to rusz tam zaznaczony i odznacz
                RuszPionek(zaznaczonePole, nrPola);
                zaznaczonePole = -1;
                zaznaczonePola.clear();
            }
        }
        else
        {
            // Ruch nie dobry, to sprawdzamy czy jest tam nasz pionek i go zaznaczamy
            if( pola[nrPola] != -1 && figury[pola[nrPola]]->strona == aktualnyGracz )
            {
                zaznaczonePola.clear();
                zaznaczonePola = figury[pola[nrPola]]->dostepneRuchy(pola, &figury);
                zaznaczonePola.push_front(nrPola);
                zaznaczonePole = nrPola;
            }
            else // jak nie - odznaczamy wszystko
            {
                zaznaczonePole = -1;
                zaznaczonePola.clear();
            }
        }
    }

    emit PodswietlicPola(zaznaczonePola);
}


void Silnik::RuchAI(int nrPolaStartowego, int nrPolaDocelowego) //zasada dzialania analgo
{
    //qDebug()<<nrPolaStartowego<<" "<<nrPolaDocelowego;
    if(czy_koniec)return; //jesli koniec partii to nie pozwalam na ruuch


    if( pola[nrPolaDocelowego] != -1 )
    {
        ZbijPionek(nrPolaStartowego, nrPolaDocelowego); // arg: atakujacy / atakowany
    }
    else
    {
        RuszPionek(nrPolaStartowego, nrPolaDocelowego);
    }
}
//na razie tak samo jak RuchAI
void Silnik::RuchSiec(int nrPolaStartowego, int nrPolaDocelowego)
{
    if(czy_koniec)return; //jesli koniec partii to nie pozwalam na ruuch


    if( pola[nrPolaDocelowego] != -1 )
    {
        ZbijPionek(nrPolaStartowego, nrPolaDocelowego); // arg: atakujacy / atakowany
    }
    else
    {
        RuszPionek(nrPolaStartowego, nrPolaDocelowego);
    }
}



// Narazie bicie i ruch wygladaja analogicznie, ale pewnie sie to zmieni
void Silnik::ZbijPionek(int pozBijacego, int pozBitego)
{
    int tmpBitego;
    tmpBitego = pola[pozBitego];
    pola[pozBitego] = pola[pozBijacego];
    pola[pozBijacego] = -1;
    figury[pola[pozBitego]]->UstawPole(pozBitego);

    if(Sprawdz_czy_szach()) //jesli nasz ruch powoduje szacha na naszym krolu nie pozwalam na taki ruch
    {
        pola[pozBijacego] = pola[pozBitego];
        pola[pozBitego] = tmpBitego;
        figury[pola[pozBijacego]]->UstawPole(pozBijacego);
        return;
    }

    emit UsunietoFigureZPola(pozBitego);
    emit UsunietoFigureZPola(pozBijacego);
    emit DodanoFigureNaPole(pozBitego, &(figury[pola[pozBitego]]->ikona));


    nasza_promocja="";
    if(figury[pola[pozBitego]]->typ==TPionek && figury[pola[pozBitego]]->strona==0 && pozBitego < 8) //jesli bialy pionek doszedl do konca planszy
    {
        Promocja(0,pozBitego);

    }

    if(figury[pola[pozBitego]]->typ==TPionek && figury[pola[pozBitego]]->strona==1 &&  pozBitego > 55) //jesli czaarny pionek doszedl do konca planszy
    {
        Promocja(1,pozBitego);
    }



    aktualnyGracz = -aktualnyGracz + 1; // zmienia 0 na 1 i 1 na 0



    if(Sprawdz_czy_szach())
    {
        if(Sprawdz_czy_mat())
        {
            czy_koniec = true;
            emit WykonanoRuch(2);
        }
        else
            emit WykonanoRuch(1);
    }
    else if(sprawdz_czy_pat())
    {
        czy_koniec = true;
        emit WykonanoRuch(3);
    }
    else
        emit WykonanoRuch(0);


    //jesli gramy z AI to musimy wysłać  ruch do Brutusa
    if(gra_z_kompem && aktualnyGracz == 1)
    {
        wyslij(pozBijacego,pozBitego,nasza_promocja);
    }
    if(gra_przez_siec && czyRuchPrzeciwnika == false)
    {
        QString tmp = nasza_promocja;
        if(tmp!="")
        {
            tmp="-"+tmp;
        }
        kl->wyslijRuch(QByteArray::fromStdString(QString("ruch:%1-%2-%3%4:200.").arg(przeciwnik).arg(pozBijacego).arg(pozBitego).arg(tmp).toStdString()));
        czyRuchPrzeciwnika = true;
    }
}

void Silnik::RuszPionek(int skad, int dokad)
{
    pola[dokad] = pola[skad];
    pola[skad] = -1;
    figury[pola[dokad]]->UstawPole(dokad);


    if(Sprawdz_czy_szach()) //jesli nasz ruch powoduje szacha na naszym krolu nie pozwalam na taki ruch
    {
        pola[skad] = pola[dokad];
        pola[dokad] = -1;
        figury[pola[skad]]->UstawPole(skad);
        return;
    }

    emit UsunietoFigureZPola(skad);
    emit DodanoFigureNaPole(dokad, &(figury[pola[dokad]]->ikona));



    nasza_promocja="";
    //jesli zaznaczana figura byl  pionek sprawdzamy czy doszedl do konca  planszy
    if(figury[pola[dokad]]->typ==TPionek && figury[pola[dokad]]->strona==0 && dokad < 8) //jesli bialy pionek doszedl do konca planszy
    {
        Promocja(0,dokad);

    }

    if(figury[pola[dokad]]->typ==TPionek && figury[pola[dokad]]->strona==1 &&  dokad > 55) //jesli czaarny pionek doszedl do konca planszy
    {
        Promocja(1,dokad);
    }

    //jesli zaznaczona figura byla wieza trzeba zapisac, ze sie ruszyla  (wiedza potrzebna do roszady)
    if(figury[pola[dokad]]->typ == TWieza)
    {
        figury[pola[dokad]]->ruszylSie=true;
    }



    //jesli zaznaczona figura byl krol to muusimy sprawdzic czy rucuhem byla roszada
    if(figury[pola[dokad]]->typ==TKrol)
    {
        if(figury[pola[dokad]]->ruszylSie==false)
        {
            jesli_roszada_to_wykonaj(dokad);
        }
        figury[pola[dokad]]->ruszylSie=true;
    }




    aktualnyGracz = -aktualnyGracz + 1;

    if(Sprawdz_czy_szach())
    {
        if(Sprawdz_czy_mat())
        {
            czy_koniec = true;
            emit WykonanoRuch(2);
        }
        else
            emit WykonanoRuch(1);
    }
    else if(sprawdz_czy_pat())
    {
        czy_koniec = true;
        emit WykonanoRuch(3);
    }
    else
        emit WykonanoRuch(0);


    //jesli gramy z AI to musimy wysłać  ruch do Brutusa
    if(gra_z_kompem && aktualnyGracz == 1)
    {
        wyslij(skad,dokad,nasza_promocja);
    }
    if(gra_przez_siec && czyRuchPrzeciwnika == false)
    {
        QString tmp = nasza_promocja;
        if(tmp!="")
        {
            tmp="-"+tmp;
        }
        kl->wyslijRuch(QByteArray::fromStdString(QString("ruch:%1-%2-%3%4:200.").arg(przeciwnik).arg(skad).arg(dokad).arg(tmp).toStdString()));
        czyRuchPrzeciwnika = true;
    }
}


void Silnik::Promocja(int strona,int dokad)
{
    if((!gra_z_kompem  && !gra_przez_siec) || (gra_z_kompem && aktualnyGracz == 0) || (gra_przez_siec && czyRuchPrzeciwnika  == false)) //okno pojawia się jesli nie ma gry z kompem  a jak jest  to pojawia się tylko przy naszym ruchu
    {
        msgBox->exec();
        if(msgBox->clickedButton() == hetmanButton)
        {
            emit UsunietoFigureZPola(dokad);
            Figura* fig = new Hetman(strona,dokad);
            figury[pola[dokad]]=fig;
            emit DodanoFigureNaPole(dokad, &(figury[pola[dokad]]->ikona));
            nasza_promocja = "Q";
        }
        else if (msgBox->clickedButton() == goniecButton)
        {
            emit UsunietoFigureZPola(dokad);
            Figura* fig = new Goniec(strona,dokad);
            figury[pola[dokad]]=fig;
            emit DodanoFigureNaPole(dokad, &(figury[pola[dokad]]->ikona));
            nasza_promocja = "B";
        }
        else if (msgBox->clickedButton() == skoczekButton)
        {
            emit UsunietoFigureZPola(dokad);
            Figura* fig = new Skoczek(strona,dokad);
            figury[pola[dokad]]=fig;
            emit DodanoFigureNaPole(dokad, &(figury[pola[dokad]]->ikona));
            nasza_promocja = "N";
        }
        else if (msgBox->clickedButton() == wiezaButton)
        {
            emit UsunietoFigureZPola(dokad);
            Figura* fig = new Wieza(strona,dokad);
            figury[pola[dokad]]=fig;
            emit DodanoFigureNaPole(dokad, &(figury[pola[dokad]]->ikona));
            nasza_promocja = "R";
        }
    }
    else
    {
        if(promocja=="Q")
        {
            emit UsunietoFigureZPola(dokad);
            Figura* fig = new Hetman(strona,dokad);
            figury[pola[dokad]]=fig;
            emit DodanoFigureNaPole(dokad, &(figury[pola[dokad]]->ikona));
        }
        else if(promocja=="B")
        {
            emit UsunietoFigureZPola(dokad);
            Figura* fig = new Goniec(strona,dokad);
            figury[pola[dokad]]=fig;
            emit DodanoFigureNaPole(dokad, &(figury[pola[dokad]]->ikona));
        }
        else if(promocja=="N")
        {
            emit UsunietoFigureZPola(dokad);
            Figura* fig = new Skoczek(strona,dokad);
            figury[pola[dokad]]=fig;
            emit DodanoFigureNaPole(dokad, &(figury[pola[dokad]]->ikona));
        }
        else if(promocja=="R")
        {
            emit UsunietoFigureZPola(dokad);
            Figura* fig = new Wieza(strona,dokad);
            figury[pola[dokad]]=fig;
            emit DodanoFigureNaPole(dokad, &(figury[pola[dokad]]->ikona));
        }
    }
}




bool Silnik::Sprawdz_czy_szach()
{
    ilu_szachujacych = 0;
    indexy_szachujacych.clear();
    int krolID;
    //szukam naszego króla
    for(int i = 0;i<figury.size();i++)
    {
        if(figury[i]->typ==TKrol && figury[i]->strona==aktualnyGracz)
        {
            krolID = i;
        }
    }

    //sprawdzamy czy jakas figura przeciwnika ma naszego krola na celowniku
    for(int i = 0;i<64;i++)
    {
        if(pola[i]==-1)continue;
        QVector<int> tmp;
        if(figury[pola[i]]->strona == 1 - aktualnyGracz)
        {
            tmp = figury[pola[i]]->dostepneRuchy(pola, &figury);
            if(tmp.contains(figury[krolID]->Pole()))
            {
                ilu_szachujacych++;
                indexy_szachujacych.append(i);
                //return true;
            }
        }

    }

    if(ilu_szachujacych>0)return true;
    else
        return false;
    // return false;
}


bool Silnik::Sprawdz_czy_mat()
{
    //jesli jesst wieccej niz jeden szachujacy to na pewno jest mat
    if(ilu_szachujacych>1)return true;

    int krolID;  //ID krola na liscie figur
    //szukam  króla
    for(int i = 0;i<figury.size();i++)
    {
        if(figury[i]->typ==TKrol && figury[i]->strona==aktualnyGracz)
        {
            krolID = i;
        }
    }



    //jesli jakas nasza figura moze stanac pomiedzy to nie ma mata , jesli figura szchujaca jest skoczek to mozna  pominac
    if(figury[pola[indexy_szachujacych[0]]]->typ!=TSkoczek)
    {
        QVector<int> ind_pomiedzy;
        //trzeba okreslic kierunek - z ktorej strony jest atak

        //poziom
        if(figury[krolID]->y == figury[pola[indexy_szachujacych[0]]]->y)
        {
            if(figury[krolID]->Pole() - indexy_szachujacych[0]<0)
            {
                for(int i = figury[krolID]->Pole() + 1 ;i<indexy_szachujacych[0];i++)
                {
                    ind_pomiedzy.append(i);
                }
            }
            else
            {
                for(int i = figury[krolID]->Pole() - 1 ;i>indexy_szachujacych[0];i--)
                {
                    ind_pomiedzy.append(i);
                }
            }
        }

        //pion

        else if(figury[krolID]->x == figury[pola[indexy_szachujacych[0]]]->x)
        {
            if(figury[krolID]->Pole() - indexy_szachujacych[0]<0)
            {
                for(int i = figury[krolID]->Pole() + 8 ;i<indexy_szachujacych[0];i=i+8)
                {
                    ind_pomiedzy.append(i);
                }
            }
            else
            {
                for(int i = figury[krolID]->Pole() - 8 ;i>indexy_szachujacych[0];i=i-8)
                {
                    ind_pomiedzy.append(i);
                }
            }

        }

        //jak nie pion ani poziom to pola pomiedzy sa po skosie

        else
        {
            QVector<int> pom;
            if(figury[krolID]->Pole() - indexy_szachujacych[0]<0)
            {
                int i;
                for(i = figury[krolID]->Pole() + 9 ;i<indexy_szachujacych[0];i=i+9)
                {
                    pom.append(i);
                }
                if(i==indexy_szachujacych[0])ind_pomiedzy = pom;


                for(i = figury[krolID]->Pole() + 7 ;i<indexy_szachujacych[0];i=i+7)
                {
                    pom.append(i);
                }
                if(i==indexy_szachujacych[0])ind_pomiedzy = pom;
            }
            else
            {
                int i;
                for(i = figury[krolID]->Pole() - 9 ;i>indexy_szachujacych[0];i=i-9)
                {
                    pom.append(i);
                }
                if(i==indexy_szachujacych[0])ind_pomiedzy = pom;


                for(i = figury[krolID]->Pole() - 7 ;i>indexy_szachujacych[0];i=i-7)
                {
                    pom.append(i);
                }
                if(i==indexy_szachujacych[0])ind_pomiedzy = pom;
            }
        }


        for(int i = 0;i<64;i++)
        {
            if(pola[i]==-1)continue;
            QVector<int> tmp;
            if(figury[pola[i]]->strona == aktualnyGracz)
            {
                if(figury[pola[i]]->typ==TKrol)continue;
                tmp = figury[pola[i]]->dostepneRuchy(pola, &figury);
                for(int o = 0 ; o < ind_pomiedzy.size();o++)
                {
                    if(tmp.contains(ind_pomiedzy[o]))
                    {
                        return false;

                    }
                }
            }
        }

    }

    //jesli jakas nasza  figura figura moze zbic, nie ma mata
    for(int i = 0;i<64;i++)
    {
        if(pola[i]==-1)continue;
        QVector<int> tmp;
        if(figury[pola[i]]->strona == aktualnyGracz)
        {
            tmp = figury[pola[i]]->dostepneRuchy(pola, &figury);
            if(tmp.contains(indexy_szachujacych[0]))
            {
                if(figury[pola[i]]->typ == TKrol)
                {
                    int pozBitego = indexy_szachujacych[0];
                    int pozBijacego = figury[pola[i]]->Pole();
                    int tmpBitego = pola[pozBitego];
                    pola[pozBitego] = pola[pozBijacego];
                    pola[pozBijacego] = -1;
                    figury[pola[pozBitego]]->UstawPole(pozBitego);
                    bool pom = Sprawdz_czy_szach();
                    pola[pozBijacego] = pola[pozBitego];
                    pola[pozBitego] = tmpBitego;
                    figury[pola[pozBijacego]]->UstawPole(pozBijacego);
                    if(pom == false)
                    {
                        return false;
                    }
                }
                else
                {
                    return false;
                }
            }
        }
    }


    //jesli krol moze uniec to nie ma mata
    QVector<int> dostepneRuchyDlaKrola = figury[krolID]->dostepneRuchy(pola,&figury);
    for(int i = 0;i<dostepneRuchyDlaKrola.size();i++)
    {

        int pozBitego = dostepneRuchyDlaKrola[i];
        int pozBijacego = figury[krolID]->Pole();

        int tmpBitego;
        tmpBitego = pola[pozBitego];
        pola[pozBitego] = pola[pozBijacego];
        pola[pozBijacego] = -1;
        figury[pola[pozBitego]]->UstawPole(pozBitego);

        bool czySzach = Sprawdz_czy_szach();

        pola[pozBijacego] = pola[pozBitego];
        pola[pozBitego] = tmpBitego;
        figury[pola[pozBijacego]]->UstawPole(pozBijacego);

        if(!czySzach)return false;

    }
    return true;
}

//nie mozna wykonać żadnego ruchu
bool Silnik::sprawdz_czy_pat()
{
    int krolID;
    //szukam naszego króla
    for(int i = 0;i<figury.size();i++)
    {
        if(figury[i]->typ==TKrol && figury[i]->strona==aktualnyGracz)
        {
            krolID = i;
        }
    }
    QVector<int> dostepneRuchyDlaKrola = figury[krolID]->dostepneRuchy(pola,&figury);

    for(int i = 0;i<dostepneRuchyDlaKrola.size();i++)  //wykonuję wirtualnie wszystkie możliwe ruchy dla krola, jak moze sie ruszyc to nie ma pata
    {

        int pozBitego = dostepneRuchyDlaKrola[i];
        int pozBijacego = figury[krolID]->Pole();

        int tmpBitego;
        tmpBitego = pola[pozBitego];
        pola[pozBitego] = pola[pozBijacego];
        pola[pozBijacego] = -1;
        figury[pola[pozBitego]]->UstawPole(pozBitego);

        bool czySzach = Sprawdz_czy_szach();

        pola[pozBijacego] = pola[pozBitego];
        pola[pozBitego] = tmpBitego;
        figury[pola[pozBijacego]]->UstawPole(pozBijacego);


        if(!czySzach)return false;

    }
    QVector<int> tmp; //jestli krol nie moze sie ruszyc to sprawdzam  czy inne figury moga sie ruszyc
    for(int i = 0;i<64;i++)
    {
        if(pola[i]==-1)continue;
        if(figury[pola[i]]->typ!=TKrol && figury[pola[i]]->strona==aktualnyGracz)
        {
            tmp = figury[pola[i]]->dostepneRuchy(pola,&figury);
            if(tmp.size()>0)return false;
        }
    }
    return true;
}


void Silnik::jesli_roszada_to_wykonaj(int dokad)
{
    if(dokad == 2)
    {
        pola[3] = pola[0];
        pola[0] = -1;
        figury[pola[3]]->UstawPole(3);
        emit UsunietoFigureZPola(0);
        emit DodanoFigureNaPole(3, &(figury[pola[3]]->ikona));
    }
    else if(dokad ==  6)
    {
        pola[5] = pola[7];
        pola[7] = -1;
        figury[pola[5]]->UstawPole(5);
        emit UsunietoFigureZPola(7);
        emit DodanoFigureNaPole(5, &(figury[pola[5]]->ikona));
    }
    else if(dokad == 58)
    {
        pola[59] = pola[56];
        pola[56] = -1;
        figury[pola[59]]->UstawPole(59);
        emit UsunietoFigureZPola(56);
        emit DodanoFigureNaPole(59, &(figury[pola[59]]->ikona));
    }
    else if(dokad == 62)
    {
        pola[61] = pola[63];
        pola[63] = -1;
        figury[pola[61]]->UstawPole(61);
        emit UsunietoFigureZPola(63);
        emit DodanoFigureNaPole(61, &(figury[pola[61]]->ikona));
    }
}

bool Silnik::sprawdz_czy_ruch(QString s)
{
    //sprawdzamy czy ruch np. e2e3
    if(s.at(0)=='a' || s.at(0)=='b' || s.at(0)=='c' || s.at(0)=='d' || s.at(0)=='e' || s.at(0)=='f' || s.at(0)=='g' || s.at(0)=='h'){
        if(s.at(1)=='1' || s.at(1)=='2' || s.at(1)=='3' || s.at(1)=='4' || s.at(1)=='5' || s.at(1)=='6' || s.at(1)=='7' || s.at(1)=='8'){
            if(s.at(2)=='a' || s.at(2)=='b' || s.at(2)=='c' || s.at(2)=='d' || s.at(2)=='e' || s.at(2)=='f' || s.at(2)=='g' || s.at(2)=='h'){
                if(s.at(3)=='1' || s.at(3)=='2' || s.at(3)=='3' || s.at(3)=='4' || s.at(3)=='5' || s.at(3)=='6' || s.at(3)=='7' || s.at(3)=='8'){
                    return true;
                }
            }
        }
    }
    return false;
}

void Silnik::wyslij(int id_1, int id_2, QString promocja)
{
    QString tmp="";
    switch (id_1%8) {
    case 0:
        tmp+="a";
        break;
    case 1:
        tmp+="b";
        break;
    case 2:
        tmp+="c";
        break;
    case 3:
        tmp+="d";
        break;
    case 4:
        tmp+="e";
        break;
    case 5:
        tmp+="f";
        break;
    case 6:
        tmp+="g";
        break;
    case 7:
        tmp+="h";
        break;
    default:
        break;
    }
    tmp+=QString::number(8-id_1/8);

    switch (id_2%8) {
    case 0:
        tmp+="a";
        break;
    case 1:
        tmp+="b";
        break;
    case 2:
        tmp+="c";
        break;
    case 3:
        tmp+="d";
        break;
    case 4:
        tmp+="e";
        break;
    case 5:
        tmp+="f";
        break;
    case 6:
        tmp+="g";
        break;
    case 7:
        tmp+="h";
        break;
    default:
        break;
    }
    tmp+=QString::number(8-id_2/8);
    if(promocja!="") tmp+=promocja;
    //qDebug()<<"WYSYŁAM: "<<tmp;
    QByteArray ba=QString(tmp+"\n").toLatin1();
    p->write(ba);
    //wysyłamy ruch do procesu z Brutusem np. e2e3
}

void Silnik::read()  //metoda uruchamiana gdy komputer coś zwraca do konsoli
{
    if(p==NULL) return;
    promocja="";
    zwrot+=p->readAllStandardOutput();              //czytamy co proces wysyła
    //qDebug()<<zwrot;
    if(!czy_koniec && zwrot.lastIndexOf("move ")!=-1 && zwrot.lastIndexOf("move ")+10<zwrot.length() && sprawdz_czy_ruch(zwrot.mid(zwrot.lastIndexOf("move ")+5,5)) )
    {
        QString ruch=zwrot.mid(zwrot.lastIndexOf("move ")+5,5);

        //wycinam konkretny ruch i zamieniam na liczbowy
        int id_1=0;

        QChar id_11=ruch.at(0);
        if(id_11=='a')
            id_1=0;
        else if(id_11=='b')
            id_1=1;
        else if(id_11=='c')
            id_1=2;
        else if(id_11=='d')
            id_1=3;
        else if(id_11=='e')
            id_1=4;
        else if(id_11=='f')
            id_1=5;
        else if(id_11=='g')
            id_1=6;
        else if(id_11=='h')
            id_1=7;
        id_1=id_1+(8-ruch.mid(1,1).toInt())*8;

        int id_2=0;

        id_11=ruch.at(2);
        if(id_11=='a')
            id_2=0;
        else if(id_11=='b')
            id_2=1;
        else if(id_11=='c')
            id_2=2;
        else if(id_11=='d')
            id_2=3;
        else if(id_11=='e')
            id_2=4;
        else if(id_11=='f')
            id_2=5;
        else if(id_11=='g')
            id_2=6;
        else if(id_11=='h')
            id_2=7;
        id_2=id_2+(8-ruch.mid(3,1).toInt())*8;

        promocja=ruch.mid(4,1);
        zwrot="";
        RuchAI(id_1, id_2); //skad - dokad

    }
}

void Silnik::odebranoRuch(QString ruch)
{
    QStringList list = ruch.split("-");
    if(list.size() == 3)
    {
        promocja = list[2];
    }
    RuchSiec(list[0].toInt(),list[1].toInt());
    czyRuchPrzeciwnika = false;

}

