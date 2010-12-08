#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTableWidgetItem>

#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    timer.setInterval(0);
    connect(&timer, SIGNAL(timeout()), this, SLOT(tykniecie()));
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_6_clicked()
{
    symulacja.krok();
}

void MainWindow::on_pushButton_5_clicked()
{
    //symulacja.inicjuj(20, ograniczenia, zaleznosci, trasa);
}

void MainWindow::on_pushButton_3_clicked()
{
    timer.start();
}

void MainWindow::on_pushButton_4_clicked()
{
    timer.stop();
}

void MainWindow::tykniecie()
{
    symulacja.krok();
}

void MainWindow::on_pushButton_2_clicked()
{
    QString plik = QFileDialog::getOpenFileName(this, "Wybierz plik ograniczen", ".", "Pliki tekstowe (*.txt)");
    if(plik != "") {
        ograniczenia.Wczytaj(plik);
        ui->tabelkaOgraniczenia->setColumnCount(2);
        ui->tabelkaOgraniczenia->setRowCount(ograniczenia.ileParametrow());
        ogr_dane.resize(ograniczenia.ileParametrow());
        ui->tabelkaOgraniczenia->setHorizontalHeaderLabels(QStringList() << "Parametr" << "Dopuszczalne wartosci");
        ui->tabelkaOgraniczenia->setColumnWidth(0,70);
        ui->tabelkaOgraniczenia->setColumnWidth(1,150);
        for(int i = 0; i < ograniczenia.ileParametrow(); ++i) {
            ogr_dane[i].first = QTableWidgetItem(QString::number(i));
            ui->tabelkaOgraniczenia->setItem(i, 0, &ogr_dane[i].first);
            QString tmp = QString("%1").arg(ograniczenia.Parametr(i,0));
            for(int j = 1; j < ograniczenia.ileOgraniczen(i); ++j) {
                tmp.append(QString(", %1").arg(ograniczenia.Parametr(i,j)));
            }
            ogr_dane[i].second = QTableWidgetItem(tmp);
            ui->tabelkaOgraniczenia->setItem(i, 1, &ogr_dane[i].second);
        }
    }
}

void MainWindow::on_pushButton_clicked() // wczytywanie trasy
{
    QString plik = QFileDialog::getOpenFileName(this, "Wybierz plik trasy", ".", "Pliki tekstowe (*.txt)");
    if(plik != "") {
        trasa.Wczytaj(plik);
        ui->tabelkaTrasa->setColumnCount(trasa.ileParametrow());
        ui->tabelkaTrasa->setRowCount(trasa.ileOdcinkow());
        for(int i=0; i<trasa.ileOdcinkow(); ++i) {
            for(int j=0; j<trasa.ileParametrow(); ++j) {
                ui->tabelkaTrasa->setColumnWidth(j,40);
                QTableWidgetItem *element = new QTableWidgetItem(QString::number(trasa.Parametr(i,j)),0);
                ui->tabelkaTrasa->setItem(i,j,element);
            }
        }
    }
}
