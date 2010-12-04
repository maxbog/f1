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
    uint32 *nastepny;
    int jeszcze;
    void inicjujStan(uint32 ziarno);
    void przeladuj();

    uint32 hiBit(uint32 u) const {
        return u & 0x80000000UL;
    }
    uint32 loBit(uint32 u) const {
        return u & 0x00000001UL;
    }
    uint32 loBits(uint32 u) const {
        return u & 0x7fffffffUL;
    }
    uint32 miksuj(uint32 u, uint32 v) const {
        return hiBit(u) | loBits(v);
    }
    uint32 magia(uint32 u) const {
        return loBit(u) ? 0x9908b0dfUL : 0x0UL;
    }
    uint32 twist(uint32 m, uint32 s0, uint32 s1) const {
        return m ^ (miksuj(s0,s1) >> 1) ^ magia(s1);
    }

    static uint32 hash(std::time_t t, std::clock_t c);

};

#endif // RANDOM_H
