#ifndef CHROMOSOM_H
#define CHROMOSOM_H

#include <QString>
#include <QVector>

class Chromosom
{
public:
    Chromosom();
    Chromosom(const Chromosom &inny);

    static Chromosom wczytaj(QString nazwa_pliku);

private:
    QVector<int> _parametry;
};

#endif // CHROMOSOM_H
