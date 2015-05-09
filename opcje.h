#ifndef OPCJE
#define OPCJE
#include <QString>
#include "ModulKomunikacji/klient.h"
struct Opcje
{
    bool CzyGraAI;
    int MaxCzas;
    int PoziomTrudnosci;
    int Tura;  //aby określić kto zaczyna
    bool czyPrzezSiec;
    QString przciwnik;
};

#endif // OPCJE

