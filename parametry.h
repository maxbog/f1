#ifndef PARAMETRY_H
#define PARAMETRY_H

#include <QVector>
#include <QString>

class Trasa {

public:
    Trasa();
    Trasa(const Trasa&);
    ~Trasa() { }
    void Wczytaj(QString);
    int ileOdcinkow() const { return odcinki; }
    int ileParametrow() const { return parametry; }
    int Parametr(int,int) const;

private:
    QVector<QVector<int> > wektOdcinki;
    int odcinki,parametry;

};


class OgraniczeniaF1 {
public:
    OgraniczeniaF1();
    OgraniczeniaF1(const OgraniczeniaF1&);
    ~OgraniczeniaF1() { }
    void Wczytaj(QString);
    int ileOgraniczen(int parametr) const { return wektOgraniczenia[parametr].size(); }
    int ileParametrow() const { return parametry_poj; }
    int Parametr(int,int) const;

private:
    QVector<QVector<int> > wektOgraniczenia;
    int ograniczenia_poj,parametry_poj;
};

class MacierzZaleznosci {
public:
    MacierzZaleznosci();
    MacierzZaleznosci(const MacierzZaleznosci &);
    ~MacierzZaleznosci() { }
    void Wczytaj(QString);
    int Parametr(int,int,int,int) const;

private:
    QVector<QVector<QVector <QVector <int> > > > wektZaleznosci;
    QVector<QVector <int> > wspDrogi, wspPoj;
    int ograniczenia_poj,parametry_poj,parametry_drogi;
};


#endif // PARAMETRY_H
