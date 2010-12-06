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

    static Chromosom wczytaj(QString nazwa_pliku);
    static Chromosom losuj(const OgraniczeniaF1&);

private:
    QVector<int> _parametry;
};

#endif // CHROMOSOM_H
