/*
 * fs_low_level_math_gen.cpp
 *
 *  Created on: 2011-3-10
 *      Author: Lional King
 *
 *   Copyright (c) 2011 netsurfers
 */


#include <math.h>
#include <stdlib.h>


#include "base/fs_low_level_api.h"


r32 fsLowLevelAPI::sqrt(r32 x)
{
    return ::sqrtf(x);
}


r32 fsLowLevelAPI::sin(r32 rad)
{
    return ::sinf(rad);
}


r32 fsLowLevelAPI::cos(r32 rad)
{
    return ::cosf(rad);
}


r32 fsLowLevelAPI::asin(r32 x)
{
    return ::asinf(x);
}


r32 fsLowLevelAPI::acos(r32 x)
{
    return ::acosf(x);
}


r32 fsLowLevelAPI::atan2(r32 y, r32 x)
{
    return ::atan2f(y, x);
}


void fsLowLevelAPI::srand(s32 seed)
{
    ::srand(seed);
}


s32 fsLowLevelAPI::rand()
{
    return ::rand();
}
