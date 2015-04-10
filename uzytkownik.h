#ifndef UZYTKOWNIK_H
#define UZYTKOWNIK_H

#include <QString>

class Uzytkownik
{
public:
    Uzytkownik()
    {
        id = -1;
    }
    ~Uzytkownik() {}

public:
    int id;
    QString nick;
    int ranking; // jakis system rankingowy wprowadzic
    bool czy_gosc;
    bool czy_zalogowany;
};

#endif // UZYTKOWNIK_H
