#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QTableWidget>
#include <QVector>
#include <QPair>
#include "symulacja.h"
#include "parametry.h"
#include "wykres.h"


namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QTimer timer;

private slots:
    void on_pushEksport_clicked();
    void on_pushWszystko_clicked();
    void on_pushNazwy_clicked();
    void on_buttonWyniki_clicked();
    void on_comboSelekcja_currentIndexChanged(int index);
    void on_checkElita_toggled(bool checked);
    void on_checkNajlepszy_toggled(bool checked);
    void on_comboKrzyzowanie_currentIndexChanged(int index);
    void on_buttonLosuj_clicked();
    void on_spinPrawd_valueChanged(double );
    void on_pushZaleznosci_clicked();
    void on_comboSelekcja_activated(int );
    void on_pushOgraniczenia_clicked();
    void on_pushTrasa_clicked();
    void on_buttonStart_clicked();

    void on_buttonStop_clicked();

    void on_buttonResetuj_clicked();

    void on_buttonKrok_clicked();

    void tykniecie();

private:
    Ui::MainWindow *ui;
    OgraniczeniaF1 ograniczenia;
    Symulacja symulacja;
    Trasa trasa;
    MacierzZaleznosci zaleznosci;
    Nazwy nazwy;
    QVector<QPair<QTableWidgetItem,QTableWidgetItem> > ogr_dane;

    void odswiez_ograniczenia();
    void odswiez_trase();

    wykres w;

};

#endif // MAINWINDOW_H
