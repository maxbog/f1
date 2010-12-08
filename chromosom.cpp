#include "chromosom.h"

#include <QFile>
#include <QTextStream>
#include <stdexcept>

#include "random.h"

Chromosom::Chromosom()
    : _parametry() {
}

Chromosom::Chromosom(const Chromosom &inny)
    : _parametry(inny._parametry) {

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
        nowy._parametry[i] = ogr.Parametr(i, rand.nastInt(ogr.ileOgraniczen(i) - 1));
    }
    return nowy;
}

Chromosom Chromosom::mutacja(const OgraniczeniaF1& ogr, unsigned ileGenow) const {
    Chromosom nowy(*this);
    Random rand;
    int nastepny = -1;
    // pêtelka gwarantuje, ¿e nie bêdzie dwukrotnej mutacji tego samego genu
    while(ileGenow) {
        // ca³y wzór: rand.nastInt(_parametry.size() - 1 - (ileGenow - 1) - (nastepny + 1)) + nastepny + 1;
        nastepny = rand.nastInt(nowy._parametry.size() - ileGenow - nastepny - 1) + nastepny + 1;
        nowy._parametry[nastepny] = ogr.Parametr(nastepny,rand.nastInt(ogr.ileOgraniczen(nastepny)));
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

int Chromosom::Parametr(int i) const {
    return _parametry.at(i);
}
