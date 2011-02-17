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
    timer.stop();
    symulacja.inicjuj(ui->spinWielkoscPopulacji->value(), ui->spinIloscKrokow->value(), ograniczenia, zaleznosci, trasa);

    this->ui->progressKroki->setMaximum(this->ui->spinIloscKrokow->value());
    this->ui->progressKroki->setValue(0);
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
    ui->progressKroki->setValue(symulacja.iloscKrokow());
    if(symulacja.zakonczona()) {
        this->ui->buttonStart->setEnabled(false);
        this->ui->buttonStop->setEnabled(false);
        this->ui->buttonKrok->setEnabled(false);
        timer.stop();
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    QString plik = QFileDialog::getOpenFileName(this, "Wybierz plik ograniczen", ".", "Pliki tekstowe (*.txt)");
    if(plik != "") {
        ograniczenia.Wczytaj(plik);
        odswiez_ograniczenia();
    }
}

void MainWindow::on_pushButton_clicked() // wczytywanie trasy
{
    QString plik = QFileDialog::getOpenFileName(this, "Wybierz plik trasy", ".", "Pliki tekstowe (*.txt)");
    if(plik != "") {
        trasa.Wczytaj(plik);
        odswiez_trase();
    }
}

void MainWindow::on_comboSelekcja_activated(int )
{
    switch(ui->comboSelekcja->currentIndex()) {
    case 3: {
            ui->label_8->setEnabled(false);
            ui->spinPrawd->setEnabled(false);
            break;
        }
    case 2: {
            ui->label_8->setEnabled(true);
            ui->label_8->setText("Prawd. wyboru");
            ui->spinPrawd->setEnabled(true);
            ui->spinPrawd->setValue(symulacja.prawdSelekcji());
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
    symulacja.prawdSelekcji(ui->spinPrawd->value());
}

void MainWindow::on_buttonLosuj_clicked()
{
    trasa.Losuj(ui->spinIleOdc->value(), ui->spinIleParamDr->value());
    ograniczenia.Losuj(ui->spinIleParPoj->value());
    zaleznosci.Losuj(ui->spinIleParamDr->value(),ograniczenia.wekt());

    odswiez_trase();
    odswiez_ograniczenia();
}

void MainWindow::on_comboKrzyzowanie_currentIndexChanged(int index)
{
    symulacja.krzyzWielopunktowe(index == 1);
}

void MainWindow::on_checkNajlepszy_toggled(bool checked)
{
    symulacja.wyborNajlepszego(checked);
}

void MainWindow::on_checkElita_toggled(bool checked)
{
    symulacja.wyborElity(checked);
}

void MainWindow::on_comboSelekcja_currentIndexChanged(int index)
{
    symulacja.rodzajSelekcji(index);
}

void MainWindow::odswiez_ograniczenia() {
    ui->tabelkaOgraniczenia->clear();
    ui->tabelkaOgraniczenia->setColumnCount(2);
    ui->tabelkaOgraniczenia->setRowCount(ograniczenia.ileParametrow());
    ui->tabelkaOgraniczenia->setHorizontalHeaderLabels(QStringList() << "Parametr" << "Dopuszczalne wartosci");
    ui->tabelkaOgraniczenia->setColumnWidth(0,70);
    ui->tabelkaOgraniczenia->setColumnWidth(1,150);
    for(int i = 0; i < ograniczenia.ileParametrow(); ++i) {
        ui->tabelkaOgraniczenia->setItem(i, 0, new QTableWidgetItem(QString::number(i)));
        QString tmp = QString("%1").arg(ograniczenia.Parametr(i,0));
        for(int j = 1; j < ograniczenia.ileOgraniczen(i); ++j) {
            tmp.append(QString(", %1").arg(ograniczenia.Parametr(i,j)));
        }
        ui->tabelkaOgraniczenia->setItem(i, 1, new QTableWidgetItem(tmp));
    }
}

void MainWindow::odswiez_trase() {
    ui->tabelkaTrasa->clear();
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
