#ifndef SYMULACJA_H
#define SYMULACJA_H

#include "chromosom.h"
#include <QVector>

class Symulacja
{
public:
    Symulacja();
    void krok();
    void inicjuj(unsigned wielkosc_populacji, const OgraniczeniaF1& ogr, const MacierzZaleznosci&, const Trasa&,
                 int rodzaj_selekcji, double p_turniejowe=1,double p_rankingowe=0.5);
    void ocen_populacje();
    int ocen_jeden(const Chromosom& chr);
    void selekcja();
    void mutacja(double prawdopodobienstwo);
    void krzyzowanie(double prawdopodobienstwo);
    Chromosom najlepszyOsobnik();
private:
    QVector<Chromosom> _populacja;
    QVector<int> _oceny;
    OgraniczeniaF1 _ogr;
    Trasa _trasa;
    MacierzZaleznosci _zaleznosci;
    unsigned _ilosc_krokow;
    int _rodzaj_selekcji;
    double _p_turniejowe;
    double _p_rankingowe;
    void quicksort(QVector<int>&,int,int);
};

#endif // SYMULACJA_H
