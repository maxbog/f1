#ifndef CHROMOSOM_H
#define CHROMOSOM_H

#include <QString>
#include <QVector>
#include <QPair>

#include "parametry.h"

class Chromosom
{
public:
    Chromosom();
    Chromosom(const Chromosom &inny);

    Chromosom mutacja(const OgraniczeniaF1& ogr, unsigned ileGenow = 1) const;
    QPair<Chromosom, Chromosom> krzyzuj(const Chromosom& inny, int miejsce = -1) const;

    int Parametr(int) const;

    static Chromosom wczytaj(QString nazwa_pliku);
    static Chromosom losuj(const OgraniczeniaF1&);

    int ileParametrow() const { return _parametry.size(); }

private:
    QVector<int> _parametry;
};

#endif // CHROMOSOM_H
