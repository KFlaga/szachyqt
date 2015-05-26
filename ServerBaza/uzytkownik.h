#ifndef UZYTKOWNIK
#define UZYTKOWNIK

#include <QString>

struct Uzytkownik
{
public:
    int id;
    QString login;
    QString haslo;
    QString nick;
    int ile_wqyg;
    int ile_przeg;
    int ile_rem;
    int ranking;
    int status;
    bool czyZalogowany;
    QString aktualnyPrzeciwnik;
};

#endif // UZYTKOWNIK

