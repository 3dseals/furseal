/*
 * fs_quat.cpp
 *
 *  Created on: 2011-3-10
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */


#include "fs.h"


fsQuat::fsQuat() {}


fsQuat::fsQuat(r32 x_, r32 y_, r32 z_, r32 w_)
{
    x = x_;
    y = y_;
    z = z_;
    w = w_;
}


void fsQuat::set(r32 x_, r32 y_, r32 z_, r32 w_)
{
    x = x_;
    y = y_;
    z = z_;
    w = w_;
}


fsQuat fsQuat::slerp(const fsQuat& to, r32 ratio) const
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
        r32 cos_om = x * to.x + y * to.y + z * to.z + w * to.w;
        fsQuat quat;

        if (cos_om < 0.0f)
        {
            cos_om = -cos_om;

            quat.x = -to.x;
            quat.y = -to.y;
            quat.z = -to.z;
            quat.w = -to.w;
        }
        else
        {
            quat = to;
        }

        if (cos_om >= 1.0f)
        {
            return to;
        }
        else
        {
            r32 omega = fsMath::acos(cos_om > 1.0f ? 1.0f : cos_om);
            r32 sin_om = fsMath::sin_r32(omega);
            r32 scale0 = fsMath::sin_r32(omega * (1.0f - ratio)) / sin_om;
            r32 scale1 = fsMath::sin_r32(omega * ratio) / sin_om;

            return fsQuat( //
                x * scale0 + quat.x * scale1, //
                y * scale0 + quat.y * scale1, //
                z * scale0 + quat.z * scale1, //
                w * scale0 + quat.w * scale1);
        }
    }
}


fsMat fsQuat::toMat(const fsVec& trans) const
{
    r32 x2 = x + x;
    r32 y2 = y + y;
    r32 z2 = z + z;
    r32 wx2 = w * x2;
    r32 wy2 = w * y2;
    r32 wz2 = w * z2;
    r32 xx2 = x * x2;
    r32 xy2 = x * y2;
    r32 xz2 = x * z2;
    r32 yy2 = y * y2;
    r32 yz2 = y * z2;
    r32 zz2 = z * z2;

    return fsMat( //
        fsVec(1.0f - (yy2 + zz2), xy2 + wz2, xz2 - wy2), //
        fsVec(xy2 - wz2, 1.0f - (xx2 + zz2), yz2 + wx2), //
        fsVec(xz2 + wy2, yz2 - wx2, 1.0f - (xx2 + yy2)), //
        trans);
}


fsQuat fsQuat::fromMat(const fsMat& mat)
{
    r32 trace = mat.x_axis.x + mat.y_axis.y + mat.z_axis.z;

    if (trace > 0.0f)
    {
        r32 root = fsMath::sqrt(trace + 1.0f);
        r32 scale = 0.5f / root;

        return fsQuat( //
            (mat.y_axis.z - mat.z_axis.y) * scale, //
            (mat.z_axis.x - mat.x_axis.z) * scale, //
            (mat.x_axis.y - mat.y_axis.x) * scale, //
            root * 0.5f);
    }
    else
    {
#define MAT(a, b) *(reinterpret_cast<const r32*>(&mat) + a * 3 + b)
#define QUAT(a) *(reinterpret_cast<r32*>(&quat) + a)

        s32 i = 0;

        if (MAT(1, 1) > MAT(i, i))
        {
            i = 1;
        }

        if (MAT(2, 2) > MAT(i, i))
        {
            i = 2;
        }

        s32 j = (i + 1) % 3;
        s32 k = (i + 2) % 3;

        r32 root = fsMath::sqrt(MAT(i, i) - (MAT(j, j) + MAT(k, k)) + 1.0f);
        r32 scale = (root != 0.0f) ? 0.5f / root : root;

        fsQuat quat;

        QUAT(i) = root * 0.5f;
        QUAT(j) = (MAT(i, j) + MAT(j, i)) * scale;
        QUAT(k) = (MAT(k, i) + MAT(i, k)) * scale;
        QUAT(3) = (MAT(j, k) - MAT(k, j)) * scale;

        return quat;

#undef MAT
#undef QUAT
    }
}
