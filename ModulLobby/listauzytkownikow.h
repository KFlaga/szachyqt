#ifndef LISTAUZYTKOWNIKOW_H
#define LISTAUZYTKOWNIKOW_H

#include <QWidget>
#include "uzytkownik.h"
#include <QListWidgetItem>
#include <QVector>

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
    void dodajUzytkownika(Uzytkownik&);
    void usunUzytkownika(Uzytkownik&);
    void usunUzytkownika(QString nick);
    void czysc();

private slots:
    void zaprosGracza(QListWidgetItem *gracz);

signals:
    void zaproszono(QString nick);

private:
    Ui::ListaUzytkownikow *ui;
};

#endif // LISTAUZYTKOWNIKOW_H
