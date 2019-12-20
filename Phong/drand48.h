#ifndef DRAND48_H
#define DRAND48_H

#include <stdlib.h>

#define m_ 0x100000000LL
#define c_ 0xB16
#define a_ 0x5DEECE66DLL

static unsigned long long seed = 1;

float drand48(void)
{
    seed = (a_ * seed + c_) & 0xFFFFFFFFFFFFLL;
    unsigned int x = (unsigned int)(seed >> 16);
    return (float((double)x / (double)m_));
}

void srand48(unsigned int i)
{
    seed = (((long long int)i) << 16) | rand();
}

#endif