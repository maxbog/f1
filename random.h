#ifndef RANDOM_H
#define RANDOM_H

#include <ctime>

class Random
{
public:
    typedef unsigned long uint32;
    enum { N = 624 };

    Random(uint32 ziarno);
    Random();

    uint32 nastInt(); // [0,2^32-1]
    uint32 nastInt(uint32 n); // [0,n]
    double nastDouble(); // [0,1]
    double nastDouble(double n); //[0,n]

    void inicjuj();
    void inicjuj(uint32 ziarno);

protected:
    enum { M = 397 };

    uint32 stan[N];
    int mti;
    void inicjujStan(uint32 ziarno);
    static uint32 hash(std::time_t t, std::clock_t c);

};

#endif // RANDOM_H
