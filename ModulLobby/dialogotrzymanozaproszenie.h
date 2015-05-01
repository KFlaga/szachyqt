#ifndef DIALOGOTRZYMANOZAPROSZENIE_H
#define DIALOGOTRZYMANOZAPROSZENIE_H

#include <QDialog>

namespace Ui {
class DialogOtrzymanoZaproszenie;
}

class DialogOtrzymanoZaproszenie : public QDialog
{
    Q_OBJECT

public:
    explicit DialogOtrzymanoZaproszenie(QWidget *parent = 0);
    ~DialogOtrzymanoZaproszenie();

    void ustawNick(const QString& nick);
    void ustawCzas(const QString& czas);
    QString wezNick();
    QString wezCzas();

private:
    Ui::DialogOtrzymanoZaproszenie *ui;
    QString nick;
    QString czas;
};

#endif // DIALOGOTRZYMANOZAPROSZENIE_H
