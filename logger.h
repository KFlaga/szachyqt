#ifndef LOGGER_H
#define LOGGER_H

#include <QDialog>

namespace Ui {
class Logger;
}

class Logger : public QDialog
{
    Q_OBJECT

public:
    explicit Logger(QWidget *parent = 0);
    ~Logger();

    void ustawTytul(const QString& tytul);

public slots:
    void dodajLog(const QString& log);

private:
    Ui::Logger *ui;
};

#endif // LOGGER_H
