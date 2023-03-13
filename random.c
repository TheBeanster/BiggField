#include "utility.h"



unsigned short randomseed = 0;



unsigned short Random()
{
    if (randomseed == 0x560A) randomseed;
    unsigned short s0 = (unsigned short)randomseed << 8;
    s0 = s0 ^ randomseed;
    randomseed = ((s0 & 0xff) << 8) | ((s0 & 0xff00) >> 8);
    s0 = ((unsigned short)s0 << 1) ^ randomseed;
    unsigned short s1 = (s0 >> 1) ^ 0xff80;
    if ((s0 & 1) == 0)
    {
        if (s1 == 0xaa55)
            randomseed = 0;
        else
            randomseed = s1 ^ 0x1ff4;
    } else
        randomseed = s1 ^ 0x8180;

    return (unsigned short)randomseed;
}



float RandomRange(float min, float max)
{
    float n = (float)Random()/65535.0f;
    n *= (max - min);
    n += min;
    return n;
}



void SetRandomSeed(unsigned short seed)
{
    randomseed = seed;
}