#include "listauzytkownikow.h"
#include "ui_listauzytkownikow.h"
#include <QTableView>

ListaUzytkownikow::ListaUzytkownikow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ListaUzytkownikow)
{
    ui->setupUi(this);
}

ListaUzytkownikow::~ListaUzytkownikow()
{
    delete ui;
}

void ListaUzytkownikow::uzytkownikWybrany(QModelIndex idx)
{

}

void ListaUzytkownikow::podwojneKlikniecie(QModelIndex idx)
{

}

void ListaUzytkownikow::dodajUzytkownika(Uzytkownik* uzyt)
{
    QTableView
}

void ListaUzytkownikow::usunUzytkownika(QString nick)
{

}

void ListaUzytkownikow::usunUzytkownika(Uzytkownik* uzyt)
{

}
