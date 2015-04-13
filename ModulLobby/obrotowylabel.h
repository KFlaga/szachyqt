#ifndef ANIMOWANYLABEL_H
#define ANIMOWANYLABEL_H

#include <QLabel>
#include <QPixmap>
#include <QTransform>


class ObrotowyLabel : public QLabel
{
    Q_OBJECT
public:
    ObrotowyLabel(QWidget *parent = 0) : QLabel(parent) { }
    ~ObrotowyLabel() { }

    void zaladujObraz(const QString& sciezka)
    {
        this->sciezka = sciezka;
        pixmap.load(sciezka);
        setPixmap(pixmap);
    }

public slots:
    void reset()
    {
        pixmap.load(sciezka);
        setPixmap(pixmap);
    }

    void obroc() // 20 ruchow
    {
        QTransform rot;
        rot.rotate(20);
        QPixmap rotated = pixmap.transformed(rot);
        int xoffset = (rotated.width() - pixmap.width()) / 2;
        int yoffset = (rotated.height() - pixmap.height()) / 2;
        pixmap = rotated.copy(xoffset, yoffset, pixmap.width(), pixmap.height());
        setPixmap(pixmap);
    }

private:
    QString sciezka;
    QPixmap pixmap;
};


#endif // ANIMOWANYLABEL_H
