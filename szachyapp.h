#ifndef SZACHYAPP_H
#define SZACHYAPP_H

#include <QObject>
#include "oknogry.h"
#include "ModulRozgrywki/silnik.h"
#include "oknologowania.h"
#include "oknoopcji.h"
#include "opcje.h"

class SzachyApp : QObject
{
    Q_OBJECT
private:
    //static SzachyApp* instance;

    OknoGry* oknoGry;
    OknoLogowania* oknoLogowania;
    OknoOpcji* oknoOpcji;
    Silnik* silnik;
    Opcje* opts;

public:
    SzachyApp();
    ~SzachyApp();

    //static SzachyApp* sApp();

    void Run();
    Opcje* WezOpcje() { return opts; }

private slots:
    void noweOpcje();
    void stareOpcje();
    void zalogowano();

private:
    void wybranoOpcje();
};

#endif // SZACHYAPP_H
