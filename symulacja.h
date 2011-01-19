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
                 int rodzaj_selekcji, double prawd_selekcji,bool najlepszy);
    void ocen_populacje();
    void selekcja();
    void mutacja(double prawdopodobienstwo);
    void krzyzowanie(double prawdopodobienstwo);
    Chromosom najlepszyOsobnik();
    void prawdTurniejowe(double p_turniejowe) { _p_turniejowe = p_turniejowe; }
    void prawdRankingowe(double p_rankingowe) { _p_rankingowe = p_rankingowe; }
    double prawdTurniejowe() { return _p_turniejowe; }
    double prawdRankingowe() { return _p_rankingowe; }
private:
    QVector<Chromosom> _populacja;
    OgraniczeniaF1 _ogr;
    Trasa _trasa;
    MacierzZaleznosci _zaleznosci;
    unsigned _ilosc_krokow;
    int _rodzaj_selekcji;
    double _p_turniejowe;
    double _p_rankingowe;
    bool _najlepszy;

    void quicksort(QVector<int>&,int,int);
};

#endif // SYMULACJA_H
