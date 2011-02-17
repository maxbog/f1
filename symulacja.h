#ifndef SYMULACJA_H
#define SYMULACJA_H

#include "chromosom.h"
#include <QVector>

class Symulacja
{
public:
    Symulacja();
    void krok();
    void inicjuj(unsigned wielkosc_populacji, const OgraniczeniaF1& ogr, const MacierzZaleznosci&, const Trasa&);
    void ocen_populacje();
    void selekcja();
    void mutacja(double prawdopodobienstwo);
    void krzyzowanie(double prawdopodobienstwo);
    Chromosom najlepszyWSymulacji() const { return _najlepszyWSymulacji; }
    Chromosom najlepszyGlobalnie() const { return _najlepszyGlobalnie; }
    void prawdSelekcji(double p_turniejowe) { _p_selekcji = p_turniejowe; }
    double prawdSelekcji() const { return _p_selekcji; }
    void krzyzWielopunktowe(bool wielopunktowe) { _wielopunktowe = wielopunktowe; }
    double krzyzWielopunktowe() const { return _wielopunktowe; }
    void wyborNajlepszego(bool najlepszy) { _najlepszy = najlepszy; }
    bool wyborNajlepszego() const { return _najlepszy; }
    void wyborElity(bool elita) { _elita = elita; }
    bool wyborElity() const { return _elita; }
    void rodzajSelekcji(int rodzaj) { _rodzaj_selekcji = rodzaj; }
    bool rodzajSelekcji() const { return _rodzaj_selekcji; }

private:
    QVector<Chromosom> _populacja;
    OgraniczeniaF1 _ogr;
    Trasa _trasa;
    MacierzZaleznosci _zaleznosci;
    unsigned _ilosc_krokow;
    int _rodzaj_selekcji;
    double _p_selekcji;
    bool _elita;
    bool _najlepszy;
    bool _wielopunktowe;
    Chromosom _najlepszyWSymulacji;
    Chromosom _najlepszyGlobalnie;

    void quicksort(QVector<int>&,int,int);
    void wybierz_najlepszego();
};

#endif // SYMULACJA_H
