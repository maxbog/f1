#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTableWidgetItem>

#include <stdexcept>

#include <QFileDialog>
#include <QMessageBox>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_legend_item.h>
#include <qwt_legend.h>
#include <qwt_text.h>

#include "dane.h"

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

    this->ui->najlepszyOcena->clear();
    this->ui->tabelkaNajlepszy->setRowCount(0);
    this->ui->tabelkaNajlepszy->setColumnCount(0);
    this->ui->najlepszyOcenaGlobalnie->clear();
    this->ui->tabelkaNajlepszyGlobalnie->setRowCount(0);
    this->ui->tabelkaNajlepszyGlobalnie->setColumnCount(0);
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
    ui->tabelkaNajlepszy->setHorizontalHeaderLabels(QStringList() << "Parametr" << "Wartość");
    ui->tabelkaNajlepszy->setColumnWidth(0,70);
    ui->tabelkaNajlepszy->setColumnWidth(1,70);
    for(int i = 0; i < najlepszy.ileParametrow(); ++i) {
        QTableWidgetItem *numer = new QTableWidgetItem(nazwy.parametrSamochodu(i));
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
        ui->tabelkaNajlepszyGlobalnie->setHorizontalHeaderLabels(QStringList() << "Parametr" << "Wartość");
        ui->tabelkaNajlepszyGlobalnie->setColumnWidth(0,70);
        ui->tabelkaNajlepszyGlobalnie->setColumnWidth(1,70);
        for(int i = 0; i < najlepszyGlobalnie.ileParametrow(); ++i) {
            QTableWidgetItem *numer = new QTableWidgetItem(nazwy.parametrSamochodu(i));
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

void MainWindow::on_pushOgraniczenia_clicked()
{
    QString plik = QFileDialog::getOpenFileName(this, "Wybierz plik ograniczen", ".", "Pliki tekstowe (*.txt)");
    if(plik != "") {
        try {
            ograniczenia.Wczytaj(plik);
            odswiez_ograniczenia();
        } catch(std::runtime_error error) {
            QMessageBox::warning(this, "Błąd", QString("Błąd podszas wczytywania ograniczeń: ") + error.what());
        }
    }
}

void MainWindow::on_pushTrasa_clicked() // wczytywanie trasy
{
    QString plik = QFileDialog::getOpenFileName(this, "Wybierz plik trasy", ".", "Pliki tekstowe (*.txt)");
    if(plik != "") {
        try {
            trasa.Wczytaj(plik);
            odswiez_trase();
        } catch(std::runtime_error error) {
            QMessageBox::warning(this, "Błąd", QString("Błąd podszas wczytywania trasy: ") + error.what());
        }
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

void MainWindow::on_pushZaleznosci_clicked()
{
    QString plik = QFileDialog::getOpenFileName(this, "Wybierz plik zależności", ".", "Pliki tekstowe (*.txt)");
    if(plik != "") {
        try {
            zaleznosci.Wczytaj(plik);
        } catch(std::runtime_error error) {
            QMessageBox::warning(this, "Błąd", QString("Błąd podszas wczytywania zależności: ") + error.what());
        }
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

    nazwy.wyczysc();
    for(int i = 0; i < ui->spinIleParPoj->value(); ++i) {
        nazwy.dodajParSamochodu(QString::number(i));
    }
    for(int i = 0; i < ui->spinIleParamDr->value(); ++i) {
        nazwy.dodajParTrasy(QString::number(i));
    }

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
        ui->tabelkaOgraniczenia->setItem(i, 0, new QTableWidgetItem(nazwy.parametrSamochodu(i)));
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
            ui->tabelkaTrasa->setHorizontalHeaderItem(j, new QTableWidgetItem(nazwy.parametrTrasy(j)));
            ui->tabelkaTrasa->setColumnWidth(j,40);
            QTableWidgetItem *element = new QTableWidgetItem(QString::number(trasa.Parametr(i,j)),0);
            ui->tabelkaTrasa->setItem(i,j,element);
        }
    }
}

void MainWindow::on_buttonWyniki_clicked()
{


    QVector<double> min = symulacja.zapisaneDane().minima();
    QVector<double> max = symulacja.zapisaneDane().maksima();
    QVector<double> sr = symulacja.zapisaneDane().srednie();
    QVector<double> odch = symulacja.zapisaneDane().odchylenia();

    w.ustaw_czas(symulacja.iloscKrokow());
    w.dodaj_naj(double(symulacja.najlepszyGlobalnie().ocena()));
    w.dodaj(min,max,sr,odch);

    w.show();

}

void MainWindow::on_pushNazwy_clicked()
{
    QString plik = QFileDialog::getOpenFileName(this, "Wybierz plik z nazwami", ".", "Pliki tekstowe (*.txt)");
    if(plik != "") {
        try {
            nazwy.Wczytaj(plik);
            ui->pushTrasa->setEnabled(true);
            ui->pushOgraniczenia->setEnabled(true);
            ui->pushZaleznosci->setEnabled(true);
        } catch(std::runtime_error error) {
            QMessageBox::warning(this, "Błąd", QString("Błąd podszas wczytywania nazw: ") + error.what());
        }
    }
}

void MainWindow::on_pushWszystko_clicked()
{
    QString plik = QFileDialog::getOpenFileName(this, "Wybierz plik z danymi", ".", "Pliki tekstowe (*.txt)");
    if(plik != "") {
        QFile file(plik);
        if(!file.open(QFile::ReadOnly)) {
            QMessageBox::warning(this, "Błąd", QString("Błąd podczas wczytywania: Plik '") + plik + "' nie istnieje.");
            return;
        }

        QTextStream in(&file);

        try {
            nazwy.Wczytaj(in.readLine());
        } catch(std::runtime_error error) {
            QMessageBox::warning(this, "Błąd", QString("Błąd podczas wczytywania nazw: ") + error.what());
            return;
        }

        try {
            trasa.Wczytaj(in.readLine());
        } catch(std::runtime_error error) {
            QMessageBox::warning(this, "Błąd", QString("Błąd podczas wczytywania trasy: ") + error.what());
            return;
        }

        try {
            ograniczenia.Wczytaj(in.readLine());
        } catch(std::runtime_error error) {
            QMessageBox::warning(this, "Błąd", QString("Błąd podczas wczytywania ograniczeń: ") + error.what());
            return;
        }

        try {
            zaleznosci.Wczytaj(in.readLine());
        } catch(std::runtime_error error) {
            QMessageBox::warning(this, "Błąd", QString("Błąd podczas wczytywania zależności: ") + error.what());
            return;
        }

        odswiez_ograniczenia();
        odswiez_trase();
        ui->pushTrasa->setEnabled(true);
        ui->pushOgraniczenia->setEnabled(true);
        ui->pushZaleznosci->setEnabled(true);
    }
}

void MainWindow::on_pushEksport_clicked()
{
    QString nazwa = QFileDialog::getSaveFileName(this, "Podaj nazwę pliku do zapisu.");
    QFile plik(nazwa);
    if(!plik.open(QFile::WriteOnly)) {
        QMessageBox::warning(this, "Błąd", QString("Błąd podczas zapisu: Nie można zapisać do pliku '") + nazwa + "'.");
        return;
    }
    QTextStream out(&plik);
    for(int i = 0; i < symulacja.zapisaneDane().iloscPunktow(); ++i) {
        out << symulacja.zapisaneDane().minima()[i] << "\t";
        out << symulacja.zapisaneDane().maksima()[i]<< "\t";
        out << symulacja.zapisaneDane().srednie()[i]<< "\t";
        out << symulacja.zapisaneDane().odchylenia()[i]<< "\n";
    }
}

void MainWindow::on_spinMutacja_valueChanged(double value)
{
    symulacja.prawdMutacji(value);
}

void MainWindow::on_spinKrzyzowanie_valueChanged(double value)
{
    symulacja.prawdKrzyzowania(value);
}
