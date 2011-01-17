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

void Symulacja::inicjuj(unsigned wielkosc_populacji, const OgraniczeniaF1& ogr, const MacierzZaleznosci& mzal, const Trasa& tr,
                        int rodzaj_selekcji, double p_turniejowe,double p_rankingowe) {
    _ilosc_krokow = 0;
    _rodzaj_selekcji = rodzaj_selekcji;
    _p_turniejowe = p_turniejowe;
    _p_rankingowe = p_rankingowe;
    _ogr = ogr;
    _trasa = tr;
    _zaleznosci = mzal;
    _populacja.clear();
    _oceny.clear();
    for(unsigned i = 0; i < wielkosc_populacji; ++i) {
        _populacja.append(Chromosom::losuj(_ogr));
    }
    ocen_populacje();
}
int Symulacja::ocen_jeden(const Chromosom& chr) {
    int czas=0;

    for(int i=0; i<_ogr.ileParametrow(); ++i) {
        for(int j=0; j< _trasa.ileOdcinkow(); ++j) {
            for(int k=0; k< _trasa.ileParametrow(); ++k)
                czas += _zaleznosci.Parametr(i,k,chr.Parametr(i),_trasa.Parametr(j,k));
        }
    }
    return czas;
}

void Symulacja::ocen_populacje() {
    _oceny.clear();
    for(int i = 0; i < _populacja.size(); ++i) {
        _oceny.append(ocen_jeden(_populacja[i]));
    }
}

void Symulacja::selekcja() {

    switch(_rodzaj_selekcji) {

    case 0: { // metoda ruletki ?
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
                        break;
                    }
                }
            }
            break;
        }

    case 1: {   // rankingowa - prymitywna :P
            int ile_rodzicow = _populacja.size()*_p_rankingowe;
            qSort(_oceny.begin(), _oceny.end(), qGreater<int>());
            //quicksort(_oceny,0,_oceny.size());
            QVector<Chromosom> nowa_populacja;
            for(int i=0;i<ile_rodzicow;++i) nowa_populacja.append(_populacja[i]);
            _populacja = nowa_populacja;
            break;
        }

    default: {   // turniejowa
            Random rand;
            int a, b;
            double p1, p2;
            QVector<Chromosom> nowa_populacja;
            for(int i=0;i<_populacja.size();++i) {

                a = rand.nastInt(_populacja.size()-1);
                b = rand.nastInt(_populacja.size()-1);
                p1 = rand.nastDouble();
                p2 = rand.nastDouble();
                if(ocen_jeden(_populacja[a]) < ocen_jeden(_populacja[b])) {
                    int t = a;
                    a = b;
                    b = t;
                }

                if( p1 <= _p_turniejowe) nowa_populacja.append(_populacja[a]);      // zwyciezca
                if( p2 <= _p_turniejowe*(1-_p_turniejowe) ) nowa_populacja.append(_populacja[b]);
            }
            _populacja = nowa_populacja;

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

void Symulacja::quicksort(QVector<int> & oceny, int left, int right) {
    int i=left;
    int j=right;
    int x=oceny[(left+right)/2];
    do{
        while(oceny[i]<x) i++;
        while(oceny[j]>x) j--;
        if(i<=j){
            int temp=oceny[i];
            oceny[i]=oceny[j];
            oceny[j]=temp;
            Chromosom nowy(_populacja[i]);
            _populacja[i] = _populacja[j];
            _populacja[j] = nowy;
            i++;
            j--;
        }
    }while(i<=j);
    if(left<j) quicksort(oceny,left,j);
    if(right>i) quicksort(oceny,i,right);
}

Chromosom Symulacja::najlepszyOsobnik() {
    int naj_index = 0;
    int naj_ocena = 0;
    for(int i=0;i<_oceny.size();++i) {
        if(_oceny[i] > naj_ocena) {
            naj_ocena = _oceny[i];
            naj_index = i;
        }
    }
    return _populacja[naj_index];
}

