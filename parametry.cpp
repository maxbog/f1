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
        tmp.clear();
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
    ograniczenia_poj(nowy.ograniczenia_poj),
    parametry_poj(nowy.parametry_poj)
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
        tmp.clear();
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

MacierzZaleznosci::MacierzZaleznosci(): wektZaleznosci() { }

MacierzZaleznosci::MacierzZaleznosci(const MacierzZaleznosci & nowy):
    wektZaleznosci(nowy.wektZaleznosci),
    ograniczenia_poj(nowy.ograniczenia_poj),
    parametry_poj(nowy.parametry_poj),
    parametry_drogi(nowy.parametry_drogi)
{ }

void MacierzZaleznosci::Wczytaj(QString plik) {

    /* plik w postaci:
       ilosc_paramterow_pojazdu ilosc_paramterow_drogi
       ile_wsp_paramteru_pojazdu1 dopuszczalne wartosci wspolczynnikow paramteru pojazdu
       ile_wsp_paramteru_pojazdu2 dopuszczalne wartosci wspolczynnikow paramteru pojazdu
       ile_wsp_paramteru_pojazdu3 dopuszczalne wartosci wspolczynnikow paramteru pojazdu
       ...

       ile_wsp_parametru_drogi1 dopuszczalne wartosci wspolczynnikow paramteru drogi
       ile_wsp_parametru_drogi2 dopuszczalne wartosci wspolczynnikow paramteru drogi
       ile_wsp_parametru_drogi3 dopuszczalne wartosci wspolczynnikow paramteru drogi
       ...
       nr_aktualnego_param_poj nr_aktualnego_param_drogi
       czas11 czas12 ...
       czas21 czas22 ...
       ...
    */


    QFile file(plik);
    if(!file.open(QFile::ReadOnly)) {
        // TODO: jakiœ log/raportowanie b³êdów?
        throw std::runtime_error("plik nie istnieje");
    }

    QTextStream in(&file);
    in >> parametry_poj >> parametry_drogi;             // ile jest parametrow pojazdu i parametrow drogi

    int a;
    int nr_param_drogi, nr_param_poj;
    int ile_wsp_drogi, ile_wsp_poj;

    wektZaleznosci.resize(parametry_poj);
    wspPoj.resize(parametry_poj);
    for(int i=0;i<parametry_poj;++i) {
        in >> ile_wsp_poj;
        for(int j=0;j<ile_wsp_poj;j++) {
            in >> a;
            wspPoj[i].append(a);
        }
    }

    wspDrogi.resize(parametry_drogi);
    for(int i=0;i<parametry_drogi;++i) {
        in >> ile_wsp_drogi;
        for(int j=0;j<ile_wsp_drogi;j++) {
            in >> a;
            wspDrogi[i].append(a);
        }
    }

    for(int k=0;k<parametry_poj*parametry_drogi;++k) {

        in >> nr_param_poj >> nr_param_drogi;           // macierz dla jakich elementow jest aktualnie wczytywana

        QVector<int> wekt_czas;
        QVector<QVector <int> > m_zaleznosci;             // pojedyncza macierz zaleznosci
        wektZaleznosci[nr_param_poj].resize(parametry_drogi);

        for(int i=0;i<wspPoj[nr_param_poj].size();++i) {
            wekt_czas.clear();
            for(int j=0;j<wspDrogi[nr_param_drogi].size();++j) {
                in >> a;
                wekt_czas.append(a);
            }
            m_zaleznosci.append(wekt_czas);
        }

        wektZaleznosci[nr_param_poj][nr_param_drogi] = m_zaleznosci;
    }
}

int MacierzZaleznosci::Parametr(int param_poj,int param_droga,int wsp_poj,int wsp_drogi) const {
    int id_wsp_drogi = wspDrogi[param_droga].indexOf(wsp_drogi);
    int id_wsp_poj = wspPoj[param_poj].indexOf(wsp_poj);
    return wektZaleznosci.at(param_poj).at(param_droga).at(id_wsp_poj).at(id_wsp_drogi);
}
