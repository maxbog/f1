#ifndef DANE_H
#define DANE_H

#include <QVector>

class Dane
{
public:
    Dane();

    void dodaj(int min, int max, double srednia, double odchylenie);
    void wyczysc();
    QVector<double> minima() const { return _minima; }
    QVector<double> maksima() const { return _maksima; }
    QVector<double> srednie() const { return _srednie; }
    QVector<double> odchylenia() const { return _odchylenia; }
    int iloscPunktow() const { return _minima.size(); }
    QVector<double> argumenty() const;

private:
    QVector<double> _minima;
    QVector<double> _maksima;
    QVector<double> _srednie;
    QVector<double> _odchylenia;
};

#endif // DANE_H
