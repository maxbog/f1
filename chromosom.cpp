#include "chromosom.h"

#include <QFile>
#include <QTextStream>
#include <stdexcept>
#include <algorithm>
#include "random.h"

Chromosom::Chromosom()
    : _parametry(), _ocena(0) {
}

Chromosom::Chromosom(const Chromosom &inny)
    : _parametry(inny._parametry), _ocena(inny._ocena) {

}

Chromosom Chromosom::wczytaj(QString nazwa_pliku) {
    Chromosom nowy;
    QFile plik(nazwa_pliku);
    if(!plik.open(QFile::ReadOnly)) {
        // TODO: jakiœ log/raportowanie b³êdów?
        throw std::runtime_error("plik nie istnieje");
    }
    QTextStream wejscie(&plik);
    int ilosc, aktualny;
    wejscie >> ilosc;
    while(ilosc) {
        wejscie >> aktualny;
        nowy._parametry.append(aktualny);
        --ilosc;
    }
    return nowy;
}

Chromosom Chromosom::losuj(const OgraniczeniaF1& ogr) {
    Chromosom nowy;
    Random rand;
    nowy._parametry.reserve(ogr.ileParametrow());
    for(int i = 0; i < ogr.ileParametrow(); ++i) {
        nowy._parametry.append(ogr.Parametr(i, rand.nastInt(ogr.ileOgraniczen(i) - 1)));
    }
    return nowy;
}

Chromosom Chromosom::mutacja(const OgraniczeniaF1& ogr, unsigned ileGenow) const {
    Chromosom nowy(*this);
    ileGenow = std::min((int)ileGenow, _parametry.size());
    Random rand;
    QVector<int> liczby = losoweLiczby(ileGenow, _parametry.size()-1);
    for(int i=0;i< liczby.count();++i) {
        nowy._parametry[liczby[i]] = ogr.Parametr(liczby[i],rand.nastInt(ogr.ileOgraniczen(liczby[i])-1));
    }
    return nowy;
}

QPair<Chromosom, Chromosom> Chromosom::krzyzuj(const Chromosom& inny, int miejsce) const {
    if(miejsce < 0) {
        Random rand;
        miejsce = rand.nastInt(_parametry.size() - 3) + 1; // miejce przeciêcia gdzieœ w œrodku chromosomu
    }
    QPair<Chromosom, Chromosom> nowy;
    for (int i = 0; i < miejsce + 1; ++i) {
        nowy.first._parametry.append(_parametry[i]);
        nowy.second._parametry.append(inny._parametry[i]);
    }
    for (int i = miejsce + 1; i < _parametry.size(); ++i) {
        nowy.first._parametry.append(inny._parametry[i]);
        nowy.second._parametry.append(_parametry[i]);
    }
    return nowy;
}

void Chromosom::ocen(const OgraniczeniaF1& ogr, const Trasa& trasa, const MacierzZaleznosci& zaleznosci) {
    _ocena = 0;

    for(int i=0; i<ogr.ileParametrow(); ++i) {
        for(int j=0; j< trasa.ileOdcinkow(); ++j) {
            for(int k=0; k< trasa.ileParametrow(); ++k)
                _ocena += zaleznosci.Parametr(i,k,Parametr(i),trasa.Parametr(j,k));
        }
    }
    _ocena = -_ocena;
}

int Chromosom::Parametr(int i) const {
    return _parametry.at(i);
}

struct RandomFunctor {
    RandomFunctor()
        : _rand()
    {}

    int operator()(int max) {
        return _rand.nastInt(max-1);
    }

    Random _rand;
};

QVector<int> Chromosom::losoweLiczby(int ile, int max) const {
    QVector<int> tmp = QVector<int>(max+1);
    for(int i=0;i < max+1; ++i) {
        tmp[i] = i;
    }
    RandomFunctor functor;
    std::random_shuffle(tmp.begin(), tmp.end(), functor);
    tmp.resize(ile);
    return tmp;
}
