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
    }
    return nowy;
}

Chromosom Chromosom::losuj(const OgraniczeniaF1& ogr) {
    Chromosom nowy;
    Random rand;
    nowy._parametry.reserve(ogr.ileParametrow());
    for(int i = 0; i < ogr.ileParametrow(); ++i) {
        nowy._parametry[i] = ogr.Parametr(i, rand.nastInt(ogr.ileOgraniczen()) - 1);
    }
    return nowy;
}
