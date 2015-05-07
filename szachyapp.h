#ifndef SZACHYAPP_H
#define SZACHYAPP_H

#include <QObject>
#include "ModulRozgrywki/oknogry.h"
#include "ModulRozgrywki/silnik.h"
#include "opcje.h"
#include "ModulLobby/oknolobby.h"
#include "uzytkownik.h"
#include "ModulKomunikacji/klient.h"
#include <QTimer>

class SzachyApp : public QObject
{
    Q_OBJECT
private:
    OknoGry* oknoGry;
    Silnik* silnik;
    Uzytkownik* biezacyUzytkownik;
    Uzytkownik* przeciwnik;
    OknoLobby* lobby;
    Klient* lacze;

public:
    SzachyApp();
    ~SzachyApp();

    void Run();

private slots:
    void zalogowano();
    void koniecGry();
    void graLokalnie(Opcje*);
    void graSieciowa(Opcje*);
    void poloczonoZSerwerem();
    void rozloczonoZSerwerem();
    void niepowodzeniePoloczeniaZSerwerem(int);
};

#endif // SZACHYAPP_H
