#ifndef OKNOLOGOWANIA_H
#define OKNOLOGOWANIA_H

#include <QDialog>
#include "ModulLobby/oknorejestracji.h"
#include "uzytkownik.h"

namespace Ui {
class OknoLogowania;
}

enum WynikLogowania { Poprawne, UzytkownikZalogowany, ZleDane };

class OknoLogowania : public QDialog
{
    Q_OBJECT

public:
    explicit OknoLogowania(QWidget *parent = 0);
    ~OknoLogowania();

private:
    Uzytkownik *sprobujZalogowac();

private slots:
    void on_buttonLogin_clicked();
    void on_buttonRegister_clicked();
    void on_leHaslo_textChanged(const QString &nHaslo);
    void on_leLogin_textChanged(const QString &nLogin);

signals:
    void zalogowano(Uzytkownik*);

private:
    Ui::OknoLogowania *ui;
    OknoRejestracji* oknoRejestracji;

};

#endif // OKNOLOGOWANIA_H
