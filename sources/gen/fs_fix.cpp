/*
 * fs_fix.cpp
 *
 *  Created on: 2011-5-27
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */


#include "fs_gen_all.h"


fsFix::fsFix() {}


fsFix::fsFix(s32 n)
{
    s64 value = static_cast<s64>(n) << FRACTION_BIT_NUM;
    checkOverflow(value);

    m_value = static_cast<s32>(value);
}


fsFix::fsFix(r32 r)
{
    s64 value = static_cast<s64>(r * (1 << FRACTION_BIT_NUM));
    checkOverflow(value);

    if (r != 0.0f && value == 0)
    {
        fsThrow(ExceptionUnderflow);
    }

    m_value = static_cast<s32>(value);
}


bool fsFix::operator==(fsFix fix) const
{
    return (m_value == fix.m_value);
}


bool fsFix::operator!=(fsFix fix) const
{
    return (m_value != fix.m_value);
}


bool fsFix::operator<(fsFix fix) const
{
    return (m_value < fix.m_value);
}


bool fsFix::operator>(fsFix fix) const
{
    return (m_value > fix.m_value);
}


bool fsFix::operator<=(fsFix fix) const
{
    return (m_value <= fix.m_value);
}


bool fsFix::operator>=(fsFix fix) const
{
    return (m_value >= fix.m_value);
}


fsFix fsFix::operator+(fsFix fix) const
{
    s64 value = static_cast<s64>(m_value) + static_cast<s64>(fix.m_value);
    checkOverflow(value);

    return fromValue(static_cast<s32>(value));
}


fsFix operator+(s32 n, fsFix fix)
{
    return fsFix(n) + fix;
}


fsFix operator+(r32 r, fsFix fix)
{
    return fsFix(r) + fix;
}


void fsFix::operator+=(fsFix fix)
{
    *this = *this + fix;
}


fsFix fsFix::operator-() const
{
    s64 value = -static_cast<s64>(m_value);
    checkOverflow(value);

    return fromValue(static_cast<s32>(value));
}


fsFix fsFix::operator-(fsFix fix) const
{
    s64 value = static_cast<s64>(m_value) - static_cast<s64>(fix.m_value);
    checkOverflow(value);

    return fromValue(static_cast<s32>(value));
}


fsFix operator-(s32 n, fsFix fix)
{
    return fsFix(n) - fix;
}


fsFix operator-(r32 r, fsFix fix)
{
    return fsFix(r) - fix;
}


void fsFix::operator-=(fsFix fix)
{
    *this = *this - fix;
}


fsFix fsFix::operator*(fsFix fix) const
{
    s64 value = (static_cast<s64>(m_value) * static_cast<s64>(fix.m_value)) >> FRACTION_BIT_NUM;
    checkOverflow(value);

    if (m_value != 0 && fix.m_value != 0 && value == 0)
    {
        fsThrow(ExceptionUnderflow);
    }

    return fromValue(static_cast<s32>(value));
}


fsFix operator*(s32 n, fsFix fix)
{
    return fsFix(n) * fix;
}


fsFix operator*(r32 r, fsFix fix)
{
    return fsFix(r) * fix;
}


void fsFix::operator*=(fsFix fix)
{
    *this = *this * fix;
}


fsFix fsFix::operator/(fsFix fix) const
{
    s64 value = (static_cast<s64>(m_value) << FRACTION_BIT_NUM) / static_cast<s64>(fix.m_value);
    checkOverflow(value);

    if (m_value != 0 && fix.m_value != 0 && value == 0)
    {
        fsThrow(ExceptionUnderflow);
    }

    return fromValue(static_cast<s32>(value));
}


fsFix operator/(s32 n, fsFix fix)
{
    return fsFix(n) / fix;
}


fsFix operator/(r32 r, fsFix fix)
{
    return fsFix(r) / fix;
}


void fsFix::operator/=(fsFix fix)
{
    *this = *this / fix;
}


s32 fsFix::toS32() const
{
    return m_value >> FRACTION_BIT_NUM;
}


r32 fsFix::toR32() const
{
    return static_cast<r32>(m_value) / (1 << FRACTION_BIT_NUM);
}


s32 fsFix::getValue() const
{
    return m_value;
}


fsFix fsFix::fromValue(s32 value)
{
    fsFix fix;

    fix.m_value = value;

    return fix;
}


void fsFix::checkOverflow(s64 value)
{
    const s64 max_value = (static_cast<s64>(1) << (BIT_NUM - 1)) - 1;

    if (value > max_value || value < -max_value)
    {
        fsThrow(ExceptionOverflow);
    }
}
