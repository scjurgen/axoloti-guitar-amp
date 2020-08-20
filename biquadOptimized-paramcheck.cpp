#include "BiquadOptimized.h"
#include <cstdio>


int main(int ac,char *av[])
{
    BiquadOptimized bq;
    bq.calc(FilterType::HighShelf, 44100, 1000, 1.0, 6.0);
    printf("a1: %+g\na2: %+g\nb0: %+g\nb1: %+g\nb2: %+g\n\n", -bq.a1, -bq.a2, bq.b0, bq.b1, bq.b2);
    bq.calc(FilterType::LowShelf, 44100, 1000, 1.0, 6.0);
    printf("a1: %+g\na2: %+g\nb0: %+g\nb1: %+g\nb2: %+g\n\n", -bq.a1, -bq.a2, bq.b0, bq.b1, bq.b2);
    bq.calc(FilterType::Peak, 44100, 1000, 1.0, 6.0);
    printf("a1: %+g\na2: %+g\nb0: %+g\nb1: %+g\nb2: %+g\n\n", -bq.a1, -bq.a2, bq.b0, bq.b1, bq.b2);
    bq.calc(FilterType::LowPass, 44100, 1000, 1.0, 0);
    printf("a1: %+g\na2: %+g\nb0: %+g\nb1: %+g\nb2: %+g\n\n", -bq.a1, -bq.a2, bq.b0, bq.b1, bq.b2);
    bq.calc(FilterType::HighPass, 44100, 1000, 1.0, 0);
    printf("a1: %+g\na2: %+g\nb0: %+g\nb1: %+g\nb2: %+g\n\n", -bq.a1, -bq.a2, bq.b0, bq.b1, bq.b2);
    bq.calc(FilterType::Notch, 44100, 1000, 1.0, 0);
    printf("a1: %+g\na2: %+g\nb0: %+g\nb1: %+g\nb2: %+g\n\n", -bq.a1, -bq.a2, bq.b0, bq.b1, bq.b2);
    bq.calc(FilterType::BandPass, 44100, 1000, 1.0, 0);
    printf("a1: %+g\na2: %+g\nb0: %+g\nb1: %+g\nb2: %+g\n\n", -bq.a1, -bq.a2, bq.b0, bq.b1, bq.b2);
}
