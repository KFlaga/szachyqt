#include "logger.h"
#include "ui_logger.h"

Logger::Logger(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Logger)
{
    ui->setupUi(this);
}

Logger::~Logger()
{
    delete ui;
}

void Logger::dodajLog(const QString &log)
{
    ui->log->append(log);
}

void Logger::ustawTytul(const QString &tytul)
{
    ui->label->setText(tytul);
}

void Logger::wyslijKlient()
{
    emit nadajWiadomoscLokalnie(QByteArray::fromStdString(ui->lineEdit->text().toStdString()));
}

void Logger::wyslijSerwer()
{

}
