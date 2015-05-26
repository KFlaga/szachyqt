#ifndef DIALOGSZUKANIE_H
#define DIALOGSZUKANIE_H

#include <QDialog>

namespace Ui {
class DialogSzukanie;
}

class DialogSzukanie : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSzukanie(QWidget *parent = 0);
    ~DialogSzukanie();

private slots:
    void on_buttonAnuluj_clicked();

    void on_buttonSzukaj_clicked();

private:
    Ui::DialogSzukanie *ui;

signals:
    void szukajG(QString);
};

#endif // DIALOGSZUKANIE_H
