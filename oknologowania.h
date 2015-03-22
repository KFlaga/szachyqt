#ifndef OKNOLOGOWANIA_H
#define OKNOLOGOWANIA_H

#include <QDialog>
#include "oknogry.h"

namespace Ui {
class OknoLogowania;
}

class OknoLogowania : public QDialog
{
    Q_OBJECT

public:
    explicit OknoLogowania(QWidget *parent = 0);
    ~OknoLogowania();

private slots:
    void on_buttonLogin_clicked();

private:
    Ui::OknoLogowania *ui;
    OknoGry *okno;
};

#endif // OKNOLOGOWANIA_H
