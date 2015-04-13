#ifndef DIALOGCZEKAJNAODPOWIEDZ_H
#define DIALOGCZEKAJNAODPOWIEDZ_H

#include <QDialog>

namespace Ui {
class DialogCzekajNaOdpowiedz;
}

class DialogCzekajNaOdpowiedz : public QDialog
{
    Q_OBJECT

public:
    explicit DialogCzekajNaOdpowiedz(QWidget *parent = 0);
    ~DialogCzekajNaOdpowiedz();

private:
    Ui::DialogCzekajNaOdpowiedz *ui;
};

#endif // DIALOGCZEKAJNAODPOWIEDZ_H
