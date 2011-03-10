/*
 * fs_col.cpp
 *
 *  Created on: 2011-3-10
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */


#include "fs.h"


const fsCol fsCol::ZERO = fsCol(0, 0, 0, 0);
const fsCol fsCol::FULL = fsCol(255, 255, 255, 255);


fsCol::fsCol() {}


fsCol::fsCol(const fsCol& col)
{
    r = col.r;
    g = col.g;
    b = col.b;
    a = col.a;
}


fsCol::fsCol(u8 r_, u8 g_, u8 b_, u8 a_)
{
    r = r_;
    g = g_;
    b = b_;
    a = a_;
}


void fsCol::set(u8 r_, u8 g_, u8 b_, u8 a_)
{
    r = r_;
    g = g_;
    b = b_;
    a = a_;
}


bool fsCol::operator==(fsCol col) const
{
    return (r == col.r && g == col.g && b == col.b && a == col.a);
}


bool fsCol::operator!=(fsCol col) const
{
    return (r != col.r || g != col.g || b != col.b || a != col.a);
}


fsCol fsCol::operator+(fsCol col) const
{
    return fsCol( //
        fsMath::min(r + col.r, 255), //
        fsMath::min(g + col.g, 255), //
        fsMath::min(b + col.b, 255), //
        fsMath::min(a + col.a, 255));
}


void fsCol::operator+=(fsCol col)
{
    *this = *this + col;
}


fsCol fsCol::operator-(fsCol col) const
{
    return fsCol( //
        fsMath::max(r - col.r, 0), //
        fsMath::max(g - col.g, 0), //
        fsMath::max(b - col.b, 0), //
        fsMath::max(a - col.a, 0));
}


void fsCol::operator-=(fsCol col)
{
    *this = *this - col;
}


fsCol fsCol::operator*(fsCol col) const
{
    if (*this == FULL)
    {
        return col;
    }
    else if (col == FULL)
    {
        return *this;
    }
    else
    {
        return fsCol( //
            ((r + 1) * (col.r + 1) - 1) >> 8, //
            ((g + 1) * (col.g + 1) - 1) >> 8, //
            ((b + 1) * (col.b + 1) - 1) >> 8, //
            ((a + 1) * (col.a + 1) - 1) >> 8);
    }
}


void fsCol::operator*=(fsCol col)
{
    if (*this == FULL)
    {
        *this = col;
    }
    else if (col == FULL)
    {
        // do nothing
    }
    else
    {
        r = ((r + 1) * (col.r + 1) - 1) >> 8;
        g = ((g + 1) * (col.g + 1) - 1) >> 8;
        b = ((b + 1) * (col.b + 1) - 1) >> 8;
        a = ((a + 1) * (col.a + 1) - 1) >> 8;
    }
}


fsCol fsCol::operator*(r32 s) const
{
    s32 new_r = static_cast<s32>(r * s);
    s32 new_g = static_cast<s32>(g * s);
    s32 new_b = static_cast<s32>(b * s);
    s32 new_a = static_cast<s32>(a * s);

    if (new_r > 255)
    {
        new_r = 255;
    }
    else if (new_r < 0)
    {
        new_r = 0;
    }

    if (new_g > 255)
    {
        new_g = 255;
    }
    else if (new_g < 0)
    {
        new_g = 0;
    }

    if (new_b > 255)
    {
        new_b = 255;
    }
    else if (new_b < 0)
    {
        new_b = 0;
    }

    if (new_a > 255)
    {
        new_a = 255;
    }
    else if (new_a < 0)
    {
        new_a = 0;
    }

    return fsCol(new_r, new_g, new_b, new_a);
}


fsCol operator*(r32 s, fsCol col)
{
    return col * s;
}


void fsCol::operator*=(r32 s)
{
    *this = *this * s;
}


fsCol fsCol::operator/(r32 s) const
{
    return *this * (1.0f / s);
}


void fsCol::operator/=(r32 s)
{
    *this = *this / s;
}


fsCol fsCol::interp(fsCol to, r32 ratio) const
{
    if (ratio < fsMath::EPSILON)
    {
        return *this;
    }
    else if (ratio > 1.0f - fsMath::EPSILON)
    {
        return to;
    }
    else
    {
        return *this * (1.0f - ratio) + to * ratio;
    }
}
