#include "random.h"

#include <climits>

Random::uint32 Random::hash(std::time_t t, std::clock_t c) {
    static uint32 roznica = 0;
    uint32 h1 = 0;
    unsigned char *p = (unsigned char *) &t;
    for(std::size_t i = 0; i < sizeof(t); ++i) {
        h1 *= UCHAR_MAX + 2U;
        h1 += p[i];
    }
    uint32 h2 = 0;
    p = (unsigned char *) &c;
    for(std::size_t j = 0; j < sizeof(c); ++j) {
        h2 *= UCHAR_MAX + 2U;
        h2 += p[j];
    }
    return (h1 + roznica++) ^ h2;
}

Random::Random()
{
    inicjuj();
}

Random::Random(uint32 ziarno)
{
    inicjuj(ziarno);
}

void Random::inicjujStan(uint32 ziarno) {
    register uint32 *s = stan;
    register uint32 *r = stan;
    register int i = 1;
    *s++ = ziarno & 0xffffffffUL;
    for( ; i < N; ++i) {
        *s++ = (1812433253UL * (*r ^ (*r >> 30)) + 1) & 0xffffffffUL;
        r++;
    }
}

void Random::przeladuj() {
    static const int MmN = int(M) - int(N);
    register uint32 *p = stan;
    register int i;
    for(i = N - M; i--; ++p) {
        *p = twist(p[M], p[0], p[1]);
    }
    for(i = M; i--; ++p) {
        *p = twist(p[MmN], p[0], p[1]);
    }
    *p = twist(p[MmN], p[0], stan[0]);
    jeszcze = N;
    nastepny = stan;
}

void Random::inicjuj(uint32 ziarno) {
    inicjujStan(ziarno);
    przeladuj();
}

void Random::inicjuj() {
    inicjuj(hash(std::time(NULL), std::clock()));
}

Random::uint32 Random::nastInt() {
    if(jeszcze == 0) {
        przeladuj();
    }
    --jeszcze;

    register uint32 s1;
    s1 = *nastepny++;
    s1 ^= (s1 >> 11);
    s1 ^= (s1 <<  7) & 0x9d2c5680UL;
    s1 ^= (s1 << 15) & 0xefc60000UL;
    return ( s1 ^ (s1 >> 18) );
}

Random::uint32 Random::nastInt(uint32 n)
{
        uint32 used = n;
        used |= used >> 1;
        used |= used >> 2;
        used |= used >> 4;
        used |= used >> 8;
        used |= used >> 16;

        uint32 i;
        do
                i = nastInt() & used;
        while( i > n );
        return i;
}

double Random::nastDouble() {
    return double(nastInt()) * (1.0/4294967295.0);
}

double Random::nastDouble(double n) {
    return nastDouble() * n;
}
