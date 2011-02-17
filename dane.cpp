#include "dane.h"

Dane::Dane()
{
}

void Dane::dodaj(int min, int max, double srednia, double odchylenie) {
    _minima.append((double)min);
    _maksima.append((double)max);
    _srednie.append(srednia);
    _odchylenia.append(odchylenie);
}
void Dane::wyczysc() {
    _minima.clear();
    _maksima.clear();
    _srednie.clear();
    _odchylenia.clear();
}

QVector<double> Dane::argumenty() const {
    QVector<double> arg(_minima.size());
    for(int i = 0; i < _minima.size(); ++i) {
        arg.append((double)i);
    }
    return arg;
}
