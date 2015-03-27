#include "silnik.h"
#include <QVector>
#include "Figury/figura.h"
#include <QDebug>
#include <QPushButton>
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

}

void Silnik::NowaGra(Opcje* opts)
{
    // Stworz pionki i dla kazdego emituj sygnal
    // Kolejnosc dodawania -> od zera, czyli gora-lewo, dla czarnych
    // potem od konca, czyli dol-prawo, w prawa strone dla bialych
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
}

void Silnik::PoleWcisniete(int nrPola)
{
    if(czy_koniec)return;
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
            emit WykonanoRuch(2);
        }
        else
        emit WykonanoRuch(1);
    }
    else if(sprawdz_czy_pat())
    {
        emit WykonanoRuch(3);
    }
    else
    emit WykonanoRuch(0);
}


void Silnik::Promocja(int strona,int dokad)
{
    msgBox->exec();
    if(msgBox->clickedButton() == hetmanButton)
    {
        emit UsunietoFigureZPola(dokad);
        Figura* fig = new Hetman(strona,dokad);
        figury[pola[dokad]]=fig;
        emit DodanoFigureNaPole(dokad, &(figury[pola[dokad]]->ikona));
    }
    else if (msgBox->clickedButton() == goniecButton)
    {
        emit UsunietoFigureZPola(dokad);
        Figura* fig = new Goniec(strona,dokad);
        figury[pola[dokad]]=fig;
        emit DodanoFigureNaPole(dokad, &(figury[pola[dokad]]->ikona));
    }
    else if (msgBox->clickedButton() == skoczekButton)
    {
        emit UsunietoFigureZPola(dokad);
        Figura* fig = new Skoczek(strona,dokad);
        figury[pola[dokad]]=fig;
        emit DodanoFigureNaPole(dokad, &(figury[pola[dokad]]->ikona));
    }
    else if (msgBox->clickedButton() == wiezaButton)
    {
        emit UsunietoFigureZPola(dokad);
        Figura* fig = new Wieza(strona,dokad);
        figury[pola[dokad]]=fig;
        emit DodanoFigureNaPole(dokad, &(figury[pola[dokad]]->ikona));
    }
}




bool Silnik::Sprawdz_czy_szach()
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

    int ilu_szachujacych=0;

    for(int i = 0;i<64;i++)
    {
        if(pola[i]==-1)continue;
        QVector<int> tmp;
        if(figury[pola[i]]->strona == 1 - aktualnyGracz)
        {
            tmp = figury[pola[i]]->dostepneRuchy(pola, &figury);
            if(tmp.contains(figury[krolID]->Pole()))
            {
                return true;
            }
        }

    }
    return false;
}


bool Silnik::Sprawdz_czy_mat()
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

    for(int i = 0;i<dostepneRuchyDlaKrola.size();i++)  //wykonuję wirtualnie wszystkie możliwe ruchy dla krola, jak przy każdym ruchu jest szach to mamy mata
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

