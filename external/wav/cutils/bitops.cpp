#include "bitops.hpp"

extern "C" {
    int popcount(unsigned int x)
    {
        return popcount<unsigned int>(x);
    }

    int popcountl(unsigned long x)
    {
        return popcount<unsigned long>(x);
    }

    int popcountll(unsigned long long x)
    {
        return popcount<unsigned long long>(x);
    }
}