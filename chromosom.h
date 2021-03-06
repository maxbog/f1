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

    void mutacja(const OgraniczeniaF1& ogr, unsigned ileGenow = 1);
    QPair<Chromosom, Chromosom> krzyzuj(const Chromosom& inny, bool wielopunktowe = false, int ilosc = -1) const;

    int Parametr(int) const;

    void ocen(const OgraniczeniaF1& ogr, const Trasa& trasa, const MacierzZaleznosci& zaleznosci);

    int ocena() const { return _ocena; }

    static Chromosom wczytaj(QString nazwa_pliku);
    static Chromosom losuj(const OgraniczeniaF1&);

    int ileParametrow() const { return _parametry.size(); }

private:
    QVector<int> losoweLiczby(int ile, int max) const;
    QVector<int> _parametry;
    int _ocena;
};

#endif // CHROMOSOM_H
