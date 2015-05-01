#ifndef LISTAUZYTKOWNIKOW_H
#define LISTAUZYTKOWNIKOW_H

#include <QWidget>
#include "uzytkownik.h"

namespace Ui {
class ListaUzytkownikow;
}

class ListaUzytkownikow : public QWidget
{
    Q_OBJECT

public:
    explicit ListaUzytkownikow(QWidget *parent = 0);
    ~ListaUzytkownikow();

public slots:
    void dodajUzytkownika(Uzytkownik*);
    void usunUzytkownika(Uzytkownik*);
    void usunUzytkownika(QString nick);

private slots:
    void uzytkownikWybrany(QModelIndex);
    void podwojneKlikniecie(QModelIndex);

signals:
    void zaprosUzytkownika(QString nick);

private:
    Ui::ListaUzytkownikow *ui;
};

#endif // LISTAUZYTKOWNIKOW_H
