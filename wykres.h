#ifndef WYKRES_H
#define WYKRES_H

#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <QVector>
#include <QString>

class wykres : public QwtPlot
{
public:
    wykres();
    ~wykres() { }
    void ustaw_czas(int);
    void dodaj_naj(double);
    void dodaj(QVector<double>,QVector<double>,QVector<double>,QVector<double>);

private:
    QwtPlotCurve* cmin;
    QwtPlotCurve* cmax;
    QwtPlotCurve* csr;
    QwtPlotCurve* codch;
    QVector<double> time;
};
#endif // WYKRES_H
