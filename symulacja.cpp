#include "symulacja.h"

#include "random.h"

Symulacja::Symulacja()
{
}

void Symulacja::krok() {
    ++_ilosc_krokow;
    selekcja();
    krzyzowanie(0.25);
    mutacja(0.25);
    ocen_populacje();
}

void Symulacja::inicjuj(unsigned wielkosc_populacji, const OgraniczeniaF1& ogr) {
    _ilosc_krokow = 0;
    _ogr = ogr;
    _populacja.clear();
    _oceny.clear();
    for(unsigned i = 0; i < wielkosc_populacji; ++i) {
        _populacja.append(Chromosom::losuj(_ogr));
    }
    ocen_populacje();
}
int Symulacja::ocen_jeden(const Chromosom& chr) {
    return 1;
}

void Symulacja::ocen_populacje() {
    for(int i = 0; i < _populacja.size(); ++i) {
        _oceny.append(ocen_jeden(_populacja[i]));
    }
}

void Symulacja::selekcja() {
    Random rand;
    int suma_ocen = 0;
    for(int i = 0; i < _oceny.size(); ++i) {
        suma_ocen += _oceny[i];
    }
    QVector<double> dystrybuanty(_oceny.size());
    dystrybuanty[0] = double(_oceny[0])/double(suma_ocen);
    for(int i = 1; i < _oceny.size(); ++i) {
        dystrybuanty[i] = double(_oceny[i])/double(suma_ocen) + dystrybuanty[i-1];
    }
    dystrybuanty[dystrybuanty.size() - 1] = 1.01;
    QVector<Chromosom> stara_populacja(_populacja);
    for(int i = 0; i < _populacja.size(); ++i) {
        double los = rand.nastDouble();
        for(int j = 0; j < dystrybuanty.size(); ++j) {
            if(los < dystrybuanty[j]) {
                _populacja[i] = stara_populacja[j];
            }
        }
    }
}

void Symulacja::mutacja(double prawdopodobienstwo) {
    Random rand;
    for(int i = 0; i < _populacja.size(); ++i) {
        if(rand.nastDouble() < prawdopodobienstwo) {
            _populacja[i] = _populacja[i].mutacja(_ogr, rand.nastInt(4) + 1);
        }
    }
}

void Symulacja::krzyzowanie(double prawdopodobienstwo) {
    Random rand;
    QVector<int> krzyzowane;
    for(int i = 0; i < _populacja.size(); ++i) {
        if(rand.nastDouble() < prawdopodobienstwo) {
            krzyzowane.append(i);
        }
    }
    if(krzyzowane.size() % 2 == 1) {
        krzyzowane.pop_back();
    }
    int pierwszy, drugi;
    QPair<Chromosom, Chromosom> krzyz;
    pierwszy = krzyzowane[rand.nastInt(krzyzowane.size() - 1)];
    drugi = krzyzowane[rand.nastInt(krzyzowane.size() - 1)];
    krzyz = _populacja[pierwszy].krzyzuj(_populacja[drugi]);
    _populacja[pierwszy] = krzyz.first;
    _populacja[drugi] = krzyz.second;
}
