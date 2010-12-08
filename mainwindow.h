#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QTableWidget>
#include <QVector>
#include <QPair>
#include "symulacja.h"
#include "parametry.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    Symulacja symulacja;
    Trasa trasa;
    QTimer timer;

private slots:
<<<<<<< HEAD
    void on_pushButton_2_clicked();
=======
    void on_pushButton_clicked();
>>>>>>> bd93f75a626c9940829636c6df2ea9e075e3c39f
    void on_pushButton_6_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void tykniecie();

private:
    Ui::MainWindow *ui;
<<<<<<< HEAD
    OgraniczeniaF1 ograniczenia;
    QVector<QPair<QTableWidgetItem,QTableWidgetItem> > ogr_dane;
=======

>>>>>>> bd93f75a626c9940829636c6df2ea9e075e3c39f
};

#endif // MAINWINDOW_H
