#include <QtGui/QApplication>
#include "mainwindow.h"

#include <iostream>
#include <QFile>
#include <QTextStream>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
