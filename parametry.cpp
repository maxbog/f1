#include <QFile>
#include <QTextStream>
#include <stdexcept>
#include "parametry.h"

Trasa::Trasa(): wektOdcinki() { }

Trasa::Trasa(const Trasa & nowy):
            wektOdcinki(nowy.wektOdcinki),
            odcinki(nowy.odcinki),
            parametry(nowy.parametry)
            { }

void Trasa::Wczytaj(QString plik) {

    QVector<int> tmp;

    QFile file(plik);
    if(!file.open(QFile::ReadOnly)) {
        // TODO: jakiœ log/raportowanie b³êdów?
        throw std::runtime_error("plik nie istnieje");
    }

    QTextStream in(&file);
    in >> odcinki >> parametry;
    int a;
    for(int i=0;i<odcinki;++i) {
        for(int j=0;j<parametry;++j) {
            in >> a;
            tmp.append(a);
        }
        wektOdcinki.append(tmp);
    }

}

int Trasa::Parametr(int o, int p) const {
    return wektOdcinki.at(o).at(p);
}


OgraniczeniaF1::OgraniczeniaF1(): wektOgraniczenia() { }

OgraniczeniaF1::OgraniczeniaF1(const OgraniczeniaF1 & nowy):
               wektOgraniczenia(nowy.wektOgraniczenia),
               parametry_poj(nowy.parametry_poj),
               ograniczenia_poj(nowy.ograniczenia_poj)
               { }

void OgraniczeniaF1::Wczytaj(QString plik) {

    QVector<int> tmp;

    QFile file(plik);
    if(!file.open(QFile::ReadOnly)) {
        // TODO: jakiœ log/raportowanie b³êdów?
        throw std::runtime_error("plik nie istnieje");
    }

    QTextStream in(&file);                        // ograniczenia w postaci:
    in >> parametry_poj;                          // liczba parametrow auta
    int a;                                        // dop wartosci1: wart1 wart2 ...
    for(int i=0;i<parametry_poj;++i) {            // dop wartosci2: wart 1 wart 2...
        in >> ograniczenia_poj;
        for(int j=0;j<ograniczenia_poj;++j) {
            in >> a;
            tmp.append(a);
        }
        wektOgraniczenia.append(tmp);
    }

}

int OgraniczeniaF1::Parametr(int i, int j) const {
    return wektOgraniczenia.at(i).at(j);
}
