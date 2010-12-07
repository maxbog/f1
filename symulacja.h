#ifndef SYMULACJA_H
#define SYMULACJA_H

#include "chromosom.h"
#include <QVector>

class Symulacja
{
public:
    Symulacja();
    void krok();
    void inicjuj(unsigned wielkosc_populacji, const OgraniczeniaF1& ogr);
    void ocen_populacje();
    int ocen_jeden(const Chromosom& chr);
    void selekcja();
    void mutacja(double prawdopodobienstwo);
    void krzyzowanie(double prawdopodobienstwo);
private:
    QVector<Chromosom> _populacja;
    QVector<int> _oceny;
    OgraniczeniaF1 _ogr;
    unsigned _ilosc_krokow;
};

#endif // SYMULACJA_H
