#include "stdafx.h"
#include "VSMath.h"
#include <cstdint>

int square(int x) {
    return x * x;
}

int fast_sqrt(int x)
{
    union { float f; int32_t x; } v;
    v.f = (float)x;
    // fast aprox sqrt
    //  assumes float is in IEEE 754 single precision format 
    //  assumes int is 32 bits
    //  b = exponent bias
    //  m = number of mantissa bits
    v.x -= 1 << 23; // subtract 2^m 
    v.x >>= 1;       // divide by 2
    v.x += 1 << 29; // add ((b + 1) / 2) * 2^m

    // convert to int
    return (int)((float)v.f);
}
bool is_overlapped_pure(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2)
{
    return (x1 < x2 + w2 && x1 + w1 > x2 && y1 < y2 + h2 && y1 + h1 > y2);
}