#ifndef CHROMOSOM_H
#define CHROMOSOM_H

#include <QString>
#include <QVector>

#include "parametry.h"

class Chromosom
{
public:
    Chromosom();
    Chromosom(const Chromosom &inny);

    Chromosom mutacja(const OgraniczeniaF1& ogr, unsigned ileGenow = 1) const;
    Chromosom krzyzuj(const Chromosom& inny, int miejsce = -1) const;

    static Chromosom wczytaj(QString nazwa_pliku);
    static Chromosom losuj(const OgraniczeniaF1&);

private:
    QVector<int> _parametry;
};

#endif // CHROMOSOM_H
