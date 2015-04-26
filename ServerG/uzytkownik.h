#ifndef UZYTKOWNIK
#define UZYTKOWNIK

#include <QString>

struct Uzytkownik
{
public:
    QString login;
    QString haslo;
    QString nick;
    int ranking;
    int status;
    bool czyZalogowany;
};

#endif // UZYTKOWNIK

