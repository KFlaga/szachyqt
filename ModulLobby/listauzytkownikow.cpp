#include "listauzytkownikow.h"
#include "ui_listauzytkownikow.h"
#include <QListWidget>

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

void ListaUzytkownikow::dodajUzytkownika(Uzytkownik& uzyt)
{
    QListWidgetItem* item = new QListWidgetItem(ui->lista);
    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    QString text;
    text.fill(' ', 20 - uzyt.nick.size());
    text.prepend(uzyt.nick);
    QString status="";
    if(uzyt.status == 0)
    {
        status = "wolny";
    }
    else if(uzyt.status == 1)
    {
        status = "w grze";
    }
    text.append(QString::number(uzyt.ranking));text.append("     " + status);
    item->setText(text);
}

void ListaUzytkownikow::usunUzytkownika(QString nick)
{
    for(int i = 0; i < ui->lista->count(); i++)
    {
        if( ui->lista->item(i)->text().startsWith(nick) )
        {
            ui->lista->removeItemWidget(ui->lista->item(i));
            break;
        }
    }
}

void ListaUzytkownikow::usunUzytkownika(Uzytkownik& uzyt)
{
    for(int i = 0; i < ui->lista->count(); i++)
    {
        if( ui->lista->item(i)->text().startsWith(uzyt.nick) )
        {
            ui->lista->removeItemWidget(ui->lista->item(i));
            break;
        }
    }
}

void ListaUzytkownikow::czysc()
{
    ui->lista->clear();
}

void ListaUzytkownikow::zaprosGracza(QListWidgetItem* gracz)
{
    QString nick;
    int i = 0;
    while( gracz->text()[i] != ' ' && i < gracz->text().size())
    {
        nick.append(gracz->text()[i]);
        i++;
    }
    emit zaproszono(nick);
}
