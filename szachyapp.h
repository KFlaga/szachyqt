#ifndef SZACHYAPP_H
#define SZACHYAPP_H

#include <QObject>
#include "ModulRozgrywki/oknogry.h"
#include "ModulRozgrywki/silnik.h"
#include "opcje.h"
#include "ModulLobby/oknolobby.h"
#include "uzytkownik.h"

class SzachyApp : QObject
{
    Q_OBJECT
private:
    //static SzachyApp* instance;

    OknoGry* oknoGry;
    Silnik* silnik;
    Uzytkownik* biezacyUzytkownik;
    Uzytkownik* przeciwnik;
    OknoLobby* lobby;

public:
    SzachyApp();
    ~SzachyApp();

    //static SzachyApp* sApp();

    void Run();

private slots:
    void zalogowano();
    void koniecGry();
    void graLokalnie(Opcje*);

};

#endif // SZACHYAPP_H
