#ifndef DIALOGPODAJNICKGRACZA_H
#define DIALOGPODAJNICKGRACZA_H

#include <QDialog>

namespace Ui {
class DialogPodajNickGracza;
}

class DialogPodajNickGracza : public QDialog
{
    Q_OBJECT

public:
    explicit DialogPodajNickGracza(QWidget *parent = 0);
    ~DialogPodajNickGracza();

private:
    Ui::DialogPodajNickGracza *ui;
};

#endif // DIALOGPODAJNICKGRACZA_H
