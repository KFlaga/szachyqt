#ifndef OKNOLOGOWANIA_H
#define OKNOLOGOWANIA_H

#include <QDialog>

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

signals:
    void logowanieZakonczone();

private:
    Ui::OknoLogowania *ui;
};

#endif // OKNOLOGOWANIA_H
