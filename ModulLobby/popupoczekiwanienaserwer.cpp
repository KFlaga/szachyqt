#include "popupoczekiwanienaserwer.h"
#include "ui_popupoczekiwanienaserwer.h"
#include "obrotowylabel.h"

PopupOczekiwanieNaSerwer::PopupOczekiwanieNaSerwer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PopupOczekiwanieNaSerwer)
{
    this->setWindowFlags(Qt::Popup | Qt::Sheet);
    ui->setupUi(this);

    ui->labelCzekaj->zaladujObraz("czekaj.png");

    timer = new QTimer(this);
    timer->setInterval(100);
    timer->setTimerType(Qt::CoarseTimer);

    connect(timer, SIGNAL(timeout()), ui->labelCzekaj, SLOT(obroc()));
}

PopupOczekiwanieNaSerwer::~PopupOczekiwanieNaSerwer()
{
    delete ui;
}

int PopupOczekiwanieNaSerwer::exec()
{
    ui->labelCzekaj->reset();
    timer->start();
    int res = QDialog::exec();
    timer->stop();
    return res;
}
