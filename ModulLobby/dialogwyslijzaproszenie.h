#ifndef DIALOGWYSLIJZAPROSZENIE_H
#define DIALOGWYSLIJZAPROSZENIE_H

#include <QDialog>

namespace Ui {
class DialogWyslijZaproszenie;
}

class DialogWyslijZaproszenie : public QDialog
{
    Q_OBJECT

public:
    explicit DialogWyslijZaproszenie(QWidget *parent = 0);
    ~DialogWyslijZaproszenie();

    void ustawWlasnyNick(const QString& nick)
    {
        wlasnyNick = nick;
    }

    void podstawNick(const QString& nick);

    QString wezNick();
    int wezCzas();

private slots:
    void sprawdzPoprawnosc();

private:
    Ui::DialogWyslijZaproszenie *ui;
    QString wlasnyNick;
};

#endif // DIALOGWYSLIJZAPROSZENIE_H
