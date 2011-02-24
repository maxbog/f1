#include "random.h"

#include <climits>

const unsigned long MATRIX_A = 0x9908b0dfUL;
const unsigned long UPPER_MASK = 0x80000000UL;
const unsigned long LOWER_MASK = 0x7fffffffUL;

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
    : mti(N+1) {
    inicjuj();
}

Random::Random(uint32 ziarno)
    : mti(N+1) {
    inicjuj(ziarno);
}

void Random::inicjujStan(uint32 ziarno) {
    stan[0]= ziarno & 0xffffffffUL;
    for (mti=1; mti<N; mti++) {
        stan[mti] = (1812433253UL * (stan[mti-1] ^ (stan[mti-1] >> 30)) + mti);
        stan[mti] &= 0xffffffffUL;
    }
}

void Random::inicjuj(uint32 ziarno) {
    inicjujStan(ziarno);
}

void Random::inicjuj() {
    inicjuj(hash(std::time(NULL), std::clock()));
}

Random::uint32 Random::nastInt() {
    unsigned long y;
    static unsigned long mag01[2]={0x0UL, MATRIX_A};

    if (mti >= N) {
        int kk;

        if (mti == N+1)
            inicjujStan(5489UL);

        for (kk=0;kk<N-M;kk++) {
            y = (stan[kk]&UPPER_MASK)|(stan[kk+1]&LOWER_MASK);
            stan[kk] = stan[kk+M] ^ (y >> 1) ^ mag01[y & 0x1UL];
        }
        for (;kk<N-1;kk++) {
            y = (stan[kk]&UPPER_MASK)|(stan[kk+1]&LOWER_MASK);
            stan[kk] = stan[kk+(M-N)] ^ (y >> 1) ^ mag01[y & 0x1UL];
        }
        y = (stan[N-1]&UPPER_MASK)|(stan[0]&LOWER_MASK);
        stan[N-1] = stan[M-1] ^ (y >> 1) ^ mag01[y & 0x1UL];

        mti = 0;
    }

    y = stan[mti++];

    y ^= (y >> 11);
    y ^= (y << 7) & 0x9d2c5680UL;
    y ^= (y << 15) & 0xefc60000UL;
    y ^= (y >> 18);

    return y;
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
