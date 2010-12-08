#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTableWidgetItem>

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

void MainWindow::on_pushButton_clicked() // wczytanie trasy
{
    trasa.Wczytaj("trasa.txt");
    ui->tabelkaTrasa->setColumnCount(trasa.ileOdcinkow());
    ui->tabelkaTrasa->setRowCount(trasa.ileParametrow());

    for(int i=0; i<trasa.ileOdcinkow(); ++i) {
        for(int j=0; j<trasa.ileParametrow(); ++j) {
            QTableWidgetItem *element = new QTableWidgetItem(QString::number(trasa.Parametr(i,j)),0);
            ui->tabelkaTrasa->setItem(i,j,element);
        }

    }
}
