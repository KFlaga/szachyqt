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

private:
    Ui::DialogOtrzymanoZaproszenie *ui;
};

#endif // DIALOGOTRZYMANOZAPROSZENIE_H
