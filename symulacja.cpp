#include "symulacja.h"

#include "random.h"

bool wiekszy_ocena(const Chromosom& ch1,const Chromosom& ch2) {
    return ch1.ocena() > ch2.ocena();
}

bool mniejszy_ocena(const Chromosom& ch1,const Chromosom& ch2) {
    return ch1.ocena() < ch2.ocena();
}

Symulacja::Symulacja()
    : _p_turniejowe(1), _p_rankingowe(0.5)
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
                        int rodzaj_selekcji, double prawd_selekcji,bool najlepszy) {
    _ilosc_krokow = 0;
    _rodzaj_selekcji = rodzaj_selekcji;
    switch (rodzaj_selekcji) {
    case 1: {_p_rankingowe = prawd_selekcji; break; }
    case 2: {_p_turniejowe = prawd_selekcji; break; }
    default: break;
    }

    _najlepszy = najlepszy;

    _ogr = ogr;
    _trasa = tr;
    _zaleznosci = mzal;
    _populacja.clear();
    for(unsigned i = 0; i < wielkosc_populacji; ++i) {
        _populacja.append(Chromosom::losuj(_ogr));
    }
    ocen_populacje();
}

void Symulacja::ocen_populacje() {
    for(int i = 0; i < _populacja.size(); ++i) {
        _populacja[i].ocen(_ogr, _trasa, _zaleznosci);
    }
}

void Symulacja::selekcja() {

    switch(_rodzaj_selekcji) {

    case 0: { // metoda ruletki ?
            Random rand;
            int suma_ocen = 0;
            for(int i = 0; i < _populacja.size(); ++i) {
                suma_ocen += _populacja[i].ocena();
            }
            QVector<double> dystrybuanty(_populacja.size());
            dystrybuanty[0] = double(_populacja[0].ocena())/double(suma_ocen);
            for(int i = 1; i < _populacja.size(); ++i) {
                dystrybuanty[i] = double(_populacja[i].ocena())/double(suma_ocen) + dystrybuanty[i-1];
            }
            dystrybuanty[dystrybuanty.size() - 1] = 1.01; // kompensacja b³êdów obliczeniowych
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
            qSort(_populacja.begin(), _populacja.end(), wiekszy_ocena);
            //quicksort(_oceny,0,_oceny.size());
            QVector<Chromosom> nowa_populacja(ile_rodzicow);
            for(int i=0;i<ile_rodzicow;++i) nowa_populacja.append(_populacja[i]);
            _populacja = nowa_populacja;
            break;
        }

    case 2: {   // turniejowa
            Random rand;
            int a, b;
            double p1, p2;
            QVector<Chromosom> nowa_populacja;
            for(int i=0;i<_populacja.size();++i) {

                a = rand.nastInt(_populacja.size()-1);
                b = rand.nastInt(_populacja.size()-1);
                p1 = rand.nastDouble();
                p2 = rand.nastDouble();
                if(_populacja[a].ocena() < _populacja[b].ocena()) {
                    int t = a;
                    a = b;
                    b = t;
                }

                if( p1 <= _p_turniejowe) nowa_populacja.append(_populacja[a]);      // zwyciezca
                if( p2 <= _p_turniejowe*(1-_p_turniejowe) ) nowa_populacja.append(_populacja[b]);
            }
            if(nowa_populacja.size() > 0)
            _populacja = nowa_populacja;
            break;

        }

    case 3: { // liniowa wg indeksów
            Random rand;
            qSort(_populacja.begin(), _populacja.end(), mniejszy_ocena);
            QVector<double> dystrybuanty(_populacja.size());
            double wszystkie = _populacja.size() * (_populacja.size() + 1) / 2;
            QVector<Chromosom> stara_populacja(_populacja);
            for(int i = 0; i < _populacja.size(); ++i) {
                double los = rand.nastDouble();
                for(int j = 0; j < _populacja.size(); ++j) {
                    if(los < (j+1)*(j+2)/(2*wszystkie) + 0.001) {
                        _populacja[i] = stara_populacja[j];
                        break;
                    }
                }
            }
            break;
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
    if(krzyzowane.size() == 0)
        return; // nie krzy¿ujemy, jeœli nie ma czego krzy¿owaæ
    int pierwszy, drugi;
    QPair<Chromosom, Chromosom> krzyz;
    pierwszy = krzyzowane[rand.nastInt(krzyzowane.size() - 1)];
    drugi = krzyzowane[rand.nastInt(krzyzowane.size() - 1)];
    krzyz = _populacja[pierwszy].krzyzuj(_populacja[drugi]);
    _populacja[pierwszy] = krzyz.first;
    _populacja[drugi] = krzyz.second;
}

//void Symulacja::quicksort(QVector<int> & oceny, int left, int right) {
//    int i=left;
//    int j=right;
//    int x=oceny[(left+right)/2];
//    do{
//        while(oceny[i]<x) i++;
//        while(oceny[j]>x) j--;
//        if(i<=j){
//            int temp=oceny[i];
//            oceny[i]=oceny[j];
//            oceny[j]=temp;
//            Chromosom nowy(_populacja[i]);
//            _populacja[i] = _populacja[j];
//            _populacja[j] = nowy;
//            i++;
//            j--;
//        }
//    }while(i<=j);
//    if(left<j) quicksort(oceny,left,j);
//    if(right>i) quicksort(oceny,i,right);
//}

Chromosom Symulacja::najlepszyOsobnik() {
    int naj_index = 0;
    int naj_ocena = 0;
    for(int i=0;i<_populacja.size();++i) {
        if(_populacja[i].ocena() > naj_ocena) {
            naj_ocena = _populacja[i].ocena();
            naj_index = i;
        }
    }
    return _populacja[naj_index];
}

