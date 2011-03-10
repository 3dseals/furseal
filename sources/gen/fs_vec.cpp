/*
 * fs_vec.cpp
 *
 *  Created on: 2011-3-10
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */


#include "fs.h"


const fsVec fsVec::ZERO(0.0f, 0.0f, 0.0f);
const fsVec fsVec::X_UNIT(1.0f, 0.0f, 0.0f);
const fsVec fsVec::Y_UNIT(0.0f, 1.0f, 0.0f);
const fsVec fsVec::Z_UNIT(0.0f, 0.0f, 1.0f);


fsVec::fsVec() {}


fsVec::fsVec(r32 x_, r32 y_, r32 z_)
{
    x = x_;
    y = y_;
    z = z_;
}


void fsVec::set(r32 x_, r32 y_, r32 z_)
{
    x = x_;
    y = y_;
    z = z_;
}


fsVec fsVec::operator+(const fsVec& vec) const
{
    return fsVec(x + vec.x, y + vec.y, z + vec.z);
}


void fsVec::operator+=(const fsVec& vec)
{
    x += vec.x;
    y += vec.y;
    z += vec.z;
}


fsVec fsVec::operator-() const
{
    return fsVec(-x, -y, -z);
}


fsVec fsVec::operator-(const fsVec& vec) const
{
    return fsVec(x - vec.x, y - vec.y, z - vec.z);
}


void fsVec::operator-=(const fsVec& vec)
{
    x -= vec.x;
    y -= vec.y;
    z -= vec.z;
}


fsVec fsVec::operator*(r32 s) const
{
    return fsVec(x * s, y * s, z * s);
}


fsVec operator*(r32 s, const fsVec& vec)
{
    return fsVec(vec.x * s, vec.y * s, vec.z * s);
}


void fsVec::operator*=(r32 s)
{
    x *= s;
    y *= s;
    z *= s;
}


fsVec fsVec::operator/(r32 s) const
{
    r32 inv_s = 1.0f / s;

    return fsVec(x * inv_s, y * inv_s, z * inv_s);
}


void fsVec::operator/=(r32 s)
{
    r32 inv_s = 1.0f / s;

    x *= inv_s;
    y *= inv_s;
    z *= inv_s;
}


r32 fsVec::length() const
{
    return fsMath::sqrt(x * x + y * y + z * z);
}


r32 fsVec::sqLength() const
{
    return x * x + y * y + z * z;
}


r32 fsVec::dist(const fsVec& vec) const
{
    return (*this - vec).length();
}


r32 fsVec::sqDist(const fsVec& vec) const
{
    return (*this - vec).sqLength();
}


r32 fsVec::dot(const fsVec& vec) const
{
    return x * vec.x + y * vec.y + z * vec.z;
}


fsVec fsVec::cross(const fsVec& vec) const
{
    return fsVec(y * vec.z - z * vec.y, z * vec.x - x * vec.z, x * vec.y - y * vec.x);
}


fsVec fsVec::rotateX_r32(r32 deg) const
{
    r32 sin = fsMath::sin_r32(deg);
    r32 cos = fsMath::cos_r32(deg);

    return fsVec(x, y * cos - z * sin, z * cos + y * sin);
}


fsVec fsVec::rotateY_r32(r32 deg) const
{
    r32 sin = fsMath::sin_r32(deg);
    r32 cos = fsMath::cos_r32(deg);

    return fsVec(x * cos + z * sin, y, z * cos - x * sin);
}


fsVec fsVec::rotateZ_r32(r32 deg) const
{
    r32 sin = fsMath::sin_r32(deg);
    r32 cos = fsMath::cos_r32(deg);

    return fsVec(x * cos - y * sin, y * cos + x * sin, z);
}


fsVec fsVec::rotateX_s32(s32 deg) const
{
    r32 sin = fsMath::sin_s32(deg);
    r32 cos = fsMath::cos_s32(deg);

    return fsVec(x, y * cos - z * sin, z * cos + y * sin);
}


fsVec fsVec::rotateY_s32(s32 deg) const
{
    r32 sin = fsMath::sin_s32(deg);
    r32 cos = fsMath::cos_s32(deg);

    return fsVec(x * cos + z * sin, y, z * cos - x * sin);
}


fsVec fsVec::rotateZ_s32(s32 deg) const
{
    r32 sin = fsMath::sin_s32(deg);
    r32 cos = fsMath::cos_s32(deg);

    return fsVec(x * cos - y * sin, y * cos + x * sin, z);
}


fsVec fsVec::normalize() const
{
    return *this / length();
}


fsVec fsVec::interp(const fsVec& to, r32 ratio) const
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


fsVec fsVec::slerp(const fsVec& to, r32 ratio) const
{
    /*
        the length of each vector must be 1.0
    */

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
        r32 a = fsMath::acos(dot(to));
        r32 b = a * ratio;

        if (fsMath::abs(a) > 180.0f - fsMath::EPSILON)
        {
            return *this;
        }
        else
        {
            return fsMath::cos_r32(b) * *this + (to - fsMath::cos_r32(a) * *this) * (fsMath::sin_r32(b) / fsMath::sin_r32(a));
        }
    }
}


fsVec fsVec::toLocalOf(const fsMat& mat) const
{
    fsVec vec = *this - mat.trans;

    return fsVec( //
        vec.dot(mat.x_axis) / mat.x_axis.sqLength(), //
        vec.dot(mat.y_axis) / mat.y_axis.sqLength(), //
        vec.dot(mat.z_axis) / mat.z_axis.sqLength());
}


fsVec fsVec::toGlobalFrom(const fsMat& mat) const
{
    return mat.x_axis * x + mat.y_axis * y + mat.z_axis * z + mat.trans;
}


fsVec fsVec::toLocalOf_noTrans(const fsMat& mat) const
{
    return fsVec( //
        dot(mat.x_axis) / mat.x_axis.sqLength(), //
        dot(mat.y_axis) / mat.y_axis.sqLength(), //
        dot(mat.z_axis) / mat.z_axis.sqLength());
}


fsVec fsVec::toGlobalFrom_noTrans(const fsMat& mat) const
{
    return mat.x_axis * x + mat.y_axis * y + mat.z_axis * z;
}


void fsVec::toR32x4(r32* r32x4, r32 w) const
{
    r32x4[0] = x;
    r32x4[1] = y;
    r32x4[2] = z;
    r32x4[3] = w;
}


fsVec fsVec::fromR32x4(const r32* r32x4)
{
    return fsVec(r32x4[0], r32x4[1], r32x4[2]);
}


void fsVec::mulR32x4(r32* res_r32x4, const r32* lhs_r32x16, const r32* rhs_r32x4)
{
    r32 v[4];

    v[0] = lhs_r32x16[0] * rhs_r32x4[0] + lhs_r32x16[4] * rhs_r32x4[1] + lhs_r32x16[8] * rhs_r32x4[2] + lhs_r32x16[12] * rhs_r32x4[3];
    v[1] = lhs_r32x16[1] * rhs_r32x4[0] + lhs_r32x16[5] * rhs_r32x4[1] + lhs_r32x16[9] * rhs_r32x4[2] + lhs_r32x16[13] * rhs_r32x4[3];
    v[2] = lhs_r32x16[2] * rhs_r32x4[0] + lhs_r32x16[6] * rhs_r32x4[1] + lhs_r32x16[10] * rhs_r32x4[2] + lhs_r32x16[14] * rhs_r32x4[3];
    v[3] = lhs_r32x16[3] * rhs_r32x4[0] + lhs_r32x16[7] * rhs_r32x4[1] + lhs_r32x16[11] * rhs_r32x4[2] + lhs_r32x16[15] * rhs_r32x4[3];

    fsMemHelper::memcpy(res_r32x4, v, sizeof(v));
}
