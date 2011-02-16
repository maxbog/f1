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

void MainWindow::on_buttonKrok_clicked()
{
    tykniecie();
}

void MainWindow::on_buttonResetuj_clicked()
{

    symulacja.inicjuj(ui->spinWielkoscPopulacji->value(), ograniczenia, zaleznosci, trasa, ui->comboBox->currentIndex(),ui->spinPrawd->value(),ui->checkNajlepszy->isChecked(), ui->comboKrzyzowanie->currentIndex() == 1);
    this->ui->buttonStart->setEnabled(true);
    this->ui->buttonStop->setEnabled(true);
    this->ui->buttonKrok->setEnabled(true);
}

void MainWindow::on_buttonStart_clicked()
{
    timer.start();
}

void MainWindow::on_buttonStop_clicked()
{
    timer.stop();
}

void MainWindow::tykniecie()
{
    symulacja.krok();
    // najlepszy w symulacji
    Chromosom najlepszy(symulacja.najlepszyWSymulacji());
    ui->najlepszyOcena->setText(QString::number(-najlepszy.ocena()));
    ui->tabelkaNajlepszy->setColumnCount(2);
    ui->tabelkaNajlepszy->setRowCount(najlepszy.ileParametrow());
    ui->tabelkaNajlepszy->setHorizontalHeaderLabels(QStringList() << "Parametr" << "Wartoœæ");
    ui->tabelkaNajlepszy->setColumnWidth(0,70);
    ui->tabelkaNajlepszy->setColumnWidth(1,70);
    for(int i = 0; i < najlepszy.ileParametrow(); ++i) {
        QTableWidgetItem *numer = new QTableWidgetItem(QString::number(i));
        QTableWidgetItem *par = new QTableWidgetItem(QString::number(najlepszy.Parametr(i)));
        ui->tabelkaNajlepszy->setItem(i,0,numer);
        ui->tabelkaNajlepszy->setItem(i,1,par);
    }

    // najlepszy globalnie
    if(ui->checkNajlepszy->isChecked()) {
        Chromosom najlepszyGlobalnie(symulacja.najlepszyGlobalnie());
        ui->najlepszyOcenaGlobalnie->setText(QString::number(-najlepszyGlobalnie.ocena()));
        ui->tabelkaNajlepszyGlobalnie->setColumnCount(2);
        ui->tabelkaNajlepszyGlobalnie->setRowCount(najlepszyGlobalnie.ileParametrow());
        ui->tabelkaNajlepszyGlobalnie->setHorizontalHeaderLabels(QStringList() << "Parametr" << "Wartoœæ");
        ui->tabelkaNajlepszyGlobalnie->setColumnWidth(0,70);
        ui->tabelkaNajlepszyGlobalnie->setColumnWidth(1,70);
        for(int i = 0; i < najlepszyGlobalnie.ileParametrow(); ++i) {
            QTableWidgetItem *numer = new QTableWidgetItem(QString::number(i));
            QTableWidgetItem *par = new QTableWidgetItem(QString::number(najlepszyGlobalnie.Parametr(i)));
            ui->tabelkaNajlepszyGlobalnie->setItem(i,0,numer);
            ui->tabelkaNajlepszyGlobalnie->setItem(i,1,par);
        }
}
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

void MainWindow::on_comboBox_activated(int )
{
    switch(ui->comboBox->currentIndex()) {
    case 3: {
            ui->label_8->setEnabled(false);
            ui->spinPrawd->setEnabled(false);
            break;
        }
    case 2: {
            ui->label_8->setEnabled(true);
            ui->label_8->setText("Prawd. wyboru");
            ui->spinPrawd->setEnabled(true);
            ui->spinPrawd->setValue(symulacja.prawdTurniejowe());
            break;
        }
    default: {
            ui->label_8->setEnabled(false);
            ui->spinPrawd->setEnabled(false);
            break;
        }
    }

}

void MainWindow::on_pushButton_8_clicked()
{
    QString plik = QFileDialog::getOpenFileName(this, "Wybierz plik zale¿noœci", ".", "Pliki tekstowe (*.txt)");
    if(plik != "") {
        zaleznosci.Wczytaj(plik);
    }
}

void MainWindow::on_spinPrawd_valueChanged(double )
{
    switch(ui->comboBox->currentIndex()) {
    case 2: {
            symulacja.prawdTurniejowe(ui->spinPrawd->value());
            break;
        }
    case 1: {

            break;
        }
    }
}

void MainWindow::on_buttonLosuj_clicked()
{
    trasa.Losuj(ui->spinIleOdc->value(), ui->spinIleParamDr->value());
    ograniczenia.Losuj(ui->spinIleParPoj->value());
    zaleznosci.Losuj(ui->spinIleParamDr->value(),ograniczenia.wekt());

    ui->tabelkaTrasa->clear();
    ui->tabelkaOgraniczenia->clear();

    ui->tabelkaTrasa->setColumnCount(trasa.ileParametrow());
    ui->tabelkaTrasa->setRowCount(trasa.ileOdcinkow());
    for(int i=0; i<trasa.ileOdcinkow(); ++i) {
        for(int j=0; j<trasa.ileParametrow(); ++j) {
            ui->tabelkaTrasa->setColumnWidth(j,40);
            QTableWidgetItem *element = new QTableWidgetItem(QString::number(trasa.Parametr(i,j)),0);
            ui->tabelkaTrasa->setItem(i,j,element);
        }
    }

//    ui->tabelkaOgraniczenia->setColumnCount(2);
//    ui->tabelkaOgraniczenia->setRowCount(ograniczenia.ileParametrow());
//    ogr_dane.resize(ograniczenia.ileParametrow());
//    ui->tabelkaOgraniczenia->setHorizontalHeaderLabels(QStringList() << "Parametr" << "Dopuszczalne wartosci");
//    ui->tabelkaOgraniczenia->setColumnWidth(0,70);
//    ui->tabelkaOgraniczenia->setColumnWidth(1,150);
//    for(int i = 0; i < ograniczenia.ileParametrow(); ++i) {
//        ogr_dane[i].first = QTableWidgetItem(QString::number(i));
//        ui->tabelkaOgraniczenia->setItem(i, 0, &ogr_dane[i].first);
//        QString tmp = QString("%1").arg(ograniczenia.Parametr(i,0));
//        for(int j = 1; j < ograniczenia.ileOgraniczen(i); ++j) {
//            tmp.append(QString(", %1").arg(ograniczenia.Parametr(i,j)));
//        }
//        ogr_dane[i].second = QTableWidgetItem(tmp);
//        ui->tabelkaOgraniczenia->setItem(i, 1, &ogr_dane[i].second);
//    }

}

void MainWindow::on_comboKrzyzowanie_currentIndexChanged(int index)
{
    symulacja.krzyzWielopunktowe(index == 1);
}

void MainWindow::on_checkNajlepszy_toggled(bool checked)
{
    symulacja.wyborNajlepszego(checked);
}
