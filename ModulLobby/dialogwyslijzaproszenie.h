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

    QString wezNick()
    {
        return nick;
    }

    int wezCzas()
    {
        return czas;
    }

private slots:
    void zmienCzas(int nczas)
    {
        czas = nczas;
    }
    void zmienNick(QString nnick)
    {
        nick = nnick;
    }
    void sprawdzPoprawnosc();

private:
    Ui::DialogWyslijZaproszenie *ui;
    int czas;
    QString nick;
    QString wlasnyNick;
};

#endif // DIALOGWYSLIJZAPROSZENIE_H
