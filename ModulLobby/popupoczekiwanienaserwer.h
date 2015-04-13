#ifndef POPUPOCZEKIWANIENASERWER_H
#define POPUPOCZEKIWANIENASERWER_H

#include <QDialog>
#include <QTimer>
#include <QLabel>
#include <QMessageBox>

namespace Ui {
class PopupOczekiwanieNaSerwer;
}

class PopupOczekiwanieNaSerwer : public QDialog
{
    Q_OBJECT

public:
    explicit PopupOczekiwanieNaSerwer(QWidget *parent = 0);
    ~PopupOczekiwanieNaSerwer();

    int exec();

private:
    Ui::PopupOczekiwanieNaSerwer *ui;
    QTimer* timer;
};


#endif // POPUPOCZEKIWANIENASERWER_H
