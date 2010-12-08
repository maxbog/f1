#include "mainwindow.h"
#include "ui_mainwindow.h"

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
