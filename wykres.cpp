#include "wykres.h"
#include "dane.h"

#include <qwt_plot_curve.h>
#include <qwt_legend_item.h>
#include <qwt_legend.h>
#include <qwt_plot_marker.h>


wykres::wykres(): QwtPlot() {
    setTitle("Wyniki symulacji");
    resize(600,400);


}

void wykres::ustaw_czas(int t) {
   this->detachItems();
    time.clear();
    for (int i=0; i<t; ++i) time.append(i);
}

void wykres::dodaj_naj(double d) {
    QwtPlotMarker *mY = new QwtPlotMarker();
    mY->setLabel(QString::fromLatin1("Osobnik najlepszy = ")+QString::number(-d));
    mY->setLabelAlignment(Qt::AlignRight|Qt::AlignTop);
    mY->setLineStyle(QwtPlotMarker::HLine);
    mY->setYValue(-d);
    mY->attach(this);
}

void wykres::dodaj(QVector<double> min,QVector<double> max,QVector<double> sr,QVector<double> odch ) {

    cmin = new QwtPlotCurve("Minimum");
    cmax = new QwtPlotCurve("Maximum");
    csr = new QwtPlotCurve("Srednia");
    codch = new QwtPlotCurve("Odchylenie");
    cmin->setSamples(time,min);
    cmin->setPen(QPen(Qt::red));
    cmax->setSamples(time,max);
    cmax->setPen(QPen(Qt::blue));
    csr->setSamples(time,sr);
    csr->setPen(QPen(Qt::green));
    codch->setSamples(time,odch);
    codch->setPen(QPen(Qt::magenta));
    insertLegend(new QwtLegend(), QwtPlot::RightLegend);
    cmin->attach(this);
    cmax->attach(this);
    csr->attach(this);
    codch->attach(this);
    replot();
}

