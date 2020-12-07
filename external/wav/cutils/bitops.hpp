#pragma once

#include <memory>

template<typename Integral>
constexpr Integral shlr(Integral x, int s) noexcept
{
    return Integral(typename std::make_unsigned<Integral>::type(x) >> s);
}

template <typename Integral>
constexpr int popcount(Integral x) noexcept
{
    x = (x & Integral(0x5555555555555555UL)) + (shlr(x, 1) & Integral(0x5555555555555555UL));
    x = (x & Integral(0x3333333333333333UL)) + (shlr(x, 2) & Integral(0x3333333333333333UL));
    x = (x & Integral(0x0F0F0F0F0F0F0F0FUL)) + (shlr(x, 4) & Integral(0x0F0F0F0F0F0F0F0FUL));
    if (sizeof(x) > 1)
    {
        x = (x & Integral(0x00FF00FF00FF00FFUL)) + (shlr(x, 8) & Integral(0x00FF00FF00FF00FFUL));
        if (sizeof(x) > 2)
        {
            x = (x & Integral(0x0000FFFF0000FFFFUL)) + (shlr(x, 16) & Integral(0x0000FFFF0000FFFFUL));
            if (sizeof(x) > 4)
            {
                x = (x & Integral(0x00000000FFFFFFFFUL)) + (shlr(x, 32) & Integral(0x00000000FFFFFFFFUL));
            }
        }
    }
    return x;
}