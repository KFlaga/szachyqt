#ifndef OKNOREJESTRACJI_H
#define OKNOREJESTRACJI_H

#include <QDialog>

namespace Ui {
class OknoRejestracji;
}

class OknoRejestracji : public QDialog
{
    Q_OBJECT

public:
    explicit OknoRejestracji(QWidget *parent = 0);
    ~OknoRejestracji();

private:
    bool sprawdzPola();
    bool sprawdzUnikalnoscDanych();
    bool utworzUzytkownika();

private slots:
    void on_buttonRegister_clicked();

private:
    Ui::OknoRejestracji *ui;
};

#endif // OKNOREJESTRACJI_H
