/*
 * fs_mat.cpp
 *
 *  Created on: 2011-3-10
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */


#include "fs_gen_all.h"
#include "fs_mem_all.h"


const fsMat fsMat::ZERO(fsVec(0.0f, 0.0f, 0.0f), fsVec(0.0f, 0.0f, 0.0f), fsVec(0.0f, 0.0f, 0.0f), fsVec(0.0f, 0.0f, 0.0f));
const fsMat fsMat::UNIT(fsVec(1.0f, 0.0f, 0.0f), fsVec(0.0f, 1.0f, 0.0f), fsVec(0.0f, 0.0f, 1.0f), fsVec(0.0f, 0.0f, 0.0f));


fsMat::fsMat() {}


fsMat::fsMat(const fsVec& x_axis_, const fsVec& y_axis_, const fsVec& z_axis_, const fsVec& trans_)
{
    x_axis = x_axis_;
    y_axis = y_axis_;
    z_axis = z_axis_;
    trans = trans_;
}


void fsMat::set(const fsVec& x_axis_, const fsVec& y_axis_, const fsVec& z_axis_, const fsVec& trans_)
{
    x_axis = x_axis_;
    y_axis = y_axis_;
    z_axis = z_axis_;
    trans = trans_;
}


bool fsMat::isUnit() const
{
    return ( //
        x_axis.x == 1.0f && x_axis.y == 0.0f && x_axis.z == 0.0f && //
        y_axis.x == 0.0f && y_axis.y == 1.0f && y_axis.z == 0.0f && //
        z_axis.x == 0.0f && z_axis.y == 0.0f && z_axis.z == 1.0f && //
        trans.x == 0.0f && trans.y == 0.0f && trans.z == 0.0f);
}


fsMat fsMat::rotateX_r32(r32 deg) const
{
    r32 sin = fsMath::sin_r32(deg);
    r32 cos = fsMath::cos_r32(deg);

    return fsMat(fsVec::X_UNIT, fsVec(0.0f, cos, sin), fsVec(0.0f, -sin, cos), fsVec::ZERO).toGlobalFrom(*this);
}


fsMat fsMat::rotateY_r32(r32 deg) const
{
    r32 sin = fsMath::sin_r32(deg);
    r32 cos = fsMath::cos_r32(deg);

    return fsMat(fsVec(cos, 0.0f, -sin), fsVec::Y_UNIT, fsVec(sin, 0.0f, cos), fsVec::ZERO).toGlobalFrom(*this);
}


fsMat fsMat::rotateZ_r32(r32 deg) const
{
    r32 sin = fsMath::sin_r32(deg);
    r32 cos = fsMath::cos_r32(deg);

    return fsMat(fsVec(cos, sin, 0.0f), fsVec(-sin, cos, 0.0f), fsVec::Z_UNIT, fsVec::ZERO).toGlobalFrom(*this);
}


fsMat fsMat::rotateX_s32(s32 deg) const
{
    r32 sin = fsMath::sin_s32(deg);
    r32 cos = fsMath::cos_s32(deg);

    return fsMat(fsVec::X_UNIT, fsVec(0.0f, cos, sin), fsVec(0.0f, -sin, cos), fsVec::ZERO).toGlobalFrom(*this);
}


fsMat fsMat::rotateY_s32(s32 deg) const
{
    r32 sin = fsMath::sin_s32(deg);
    r32 cos = fsMath::cos_s32(deg);

    return fsMat(fsVec(cos, 0.0f, -sin), fsVec::Y_UNIT, fsVec(sin, 0.0f, cos), fsVec::ZERO).toGlobalFrom(*this);
}


fsMat fsMat::rotateZ_s32(s32 deg) const
{
    r32 sin = fsMath::sin_s32(deg);
    r32 cos = fsMath::cos_s32(deg);

    return fsMat(fsVec(cos, sin, 0.0f), fsVec(-sin, cos, 0.0f), fsVec::Z_UNIT, fsVec::ZERO).toGlobalFrom(*this);
}


fsMat fsMat::scale(r32 x_scale, r32 y_scale, r32 z_scale) const
{
    return fsMat(x_axis * x_scale, y_axis * y_scale, z_axis * z_scale, trans);
}


fsMat fsMat::translate(r32 x, r32 y, r32 z) const
{
    return fsMat(x_axis, y_axis, z_axis, x_axis * x + y_axis * y + z_axis * z + trans);
}


fsMat fsMat::slerp(const fsMat& to, r32 ratio) const
{
    return fsQuat::fromMat(*this).slerp(fsQuat::fromMat(to), ratio).toMat(trans.interp(to.trans, ratio));
}


fsMat fsMat::slerp_noTrans(const fsMat& to, r32 ratio) const
{
    return fsQuat::fromMat(*this).slerp(fsQuat::fromMat(to), ratio).toMat(fsVec::ZERO);
}


fsMat fsMat::orthonormal() const
{
    fsVec new_z_axis = z_axis.normalize();
    fsVec new_x_axis = y_axis.cross(z_axis).normalize();
    fsVec new_y_axis = new_z_axis.cross(new_x_axis);

    return fsMat(new_x_axis, new_y_axis, new_z_axis, trans);
}


fsMat fsMat::toLocalOf(const fsMat& mat) const
{
    r32 rsq_xa = 1.0f / mat.x_axis.sqLength();
    r32 rsq_ya = 1.0f / mat.y_axis.sqLength();
    r32 rsq_za = 1.0f / mat.z_axis.sqLength();
    fsVec vec = trans - mat.trans;

    return fsMat( //
        fsVec(x_axis.dot(mat.x_axis) * rsq_xa, x_axis.dot(mat.y_axis) * rsq_ya, x_axis.dot(mat.z_axis) * rsq_za), //
        fsVec(y_axis.dot(mat.x_axis) * rsq_xa, y_axis.dot(mat.y_axis) * rsq_ya, y_axis.dot(mat.z_axis) * rsq_za), //
        fsVec(z_axis.dot(mat.x_axis) * rsq_xa, z_axis.dot(mat.y_axis) * rsq_ya, z_axis.dot(mat.z_axis) * rsq_za), //
        fsVec(vec.dot(mat.x_axis) * rsq_xa, vec.dot(mat.y_axis) * rsq_ya, vec.dot(mat.z_axis) * rsq_za));
}


fsMat fsMat::toGlobalFrom(const fsMat& mat) const
{
    return fsMat( //
        x_axis.toGlobalFrom_noTrans(mat), //
        y_axis.toGlobalFrom_noTrans(mat), //
        z_axis.toGlobalFrom_noTrans(mat), //
        trans.toGlobalFrom(mat));
}


fsMat fsMat::toLocalOf_noTrans(const fsMat& mat) const
{
    r32 rsq_xa = 1.0f / mat.x_axis.sqLength();
    r32 rsq_ya = 1.0f / mat.y_axis.sqLength();
    r32 rsq_za = 1.0f / mat.z_axis.sqLength();

    return fsMat( //
        fsVec(x_axis.dot(mat.x_axis) * rsq_xa, x_axis.dot(mat.y_axis) * rsq_ya, x_axis.dot(mat.z_axis) * rsq_za), //
        fsVec(y_axis.dot(mat.x_axis) * rsq_xa, y_axis.dot(mat.y_axis) * rsq_ya, y_axis.dot(mat.z_axis) * rsq_za), //
        fsVec(z_axis.dot(mat.x_axis) * rsq_xa, z_axis.dot(mat.y_axis) * rsq_ya, z_axis.dot(mat.z_axis) * rsq_za), //
        fsVec::ZERO);
}


fsMat fsMat::toGlobalFrom_noTrans(const fsMat& mat) const
{
    return fsMat( //
        x_axis.toGlobalFrom_noTrans(mat), //
        y_axis.toGlobalFrom_noTrans(mat), //
        z_axis.toGlobalFrom_noTrans(mat), //
        fsVec::ZERO);
}


fsMat fsMat::lookAt(const fsVec& from, const fsVec& to, const fsVec& up)
{
    fsVec new_z_axis = (from - to).normalize();
    fsVec new_x_axis = up.cross(new_z_axis).normalize();
    fsVec new_y_axis = new_z_axis.cross(new_x_axis);

    return fsMat(new_x_axis, new_y_axis, new_z_axis, from);
}


void fsMat::toR32x16(r32* r32x16) const
{
    r32x16[0] = x_axis.x;
    r32x16[1] = x_axis.y;
    r32x16[2] = x_axis.z;
    r32x16[3] = 0.0f;

    r32x16[4] = y_axis.x;
    r32x16[5] = y_axis.y;
    r32x16[6] = y_axis.z;
    r32x16[7] = 0.0f;

    r32x16[8] = z_axis.x;
    r32x16[9] = z_axis.y;
    r32x16[10] = z_axis.z;
    r32x16[11] = 0.0f;

    r32x16[12] = trans.x;
    r32x16[13] = trans.y;
    r32x16[14] = trans.z;
    r32x16[15] = 1.0f;
}


fsMat fsMat::fromR32x16(const r32* r32x16)
{
    return fsMat( //
        fsVec(r32x16[0], r32x16[1], r32x16[2]), //
        fsVec(r32x16[4], r32x16[5], r32x16[6]), //
        fsVec(r32x16[8], r32x16[9], r32x16[10]), //
        fsVec(r32x16[12], r32x16[13], r32x16[14]));
}


void fsMat::mulR32x16(r32* res_r32x16, const r32* lhs_r32x16, const r32* rhs_r32x16)
{
    r32 m[16];

    m[0] = lhs_r32x16[0] * rhs_r32x16[0] + lhs_r32x16[4] * rhs_r32x16[1] + lhs_r32x16[8] * rhs_r32x16[2] + lhs_r32x16[12] * rhs_r32x16[3];
    m[1] = lhs_r32x16[1] * rhs_r32x16[0] + lhs_r32x16[5] * rhs_r32x16[1] + lhs_r32x16[9] * rhs_r32x16[2] + lhs_r32x16[13] * rhs_r32x16[3];
    m[2] = lhs_r32x16[2] * rhs_r32x16[0] + lhs_r32x16[6] * rhs_r32x16[1] + lhs_r32x16[10] * rhs_r32x16[2] + lhs_r32x16[14] * rhs_r32x16[3];
    m[3] = lhs_r32x16[3] * rhs_r32x16[0] + lhs_r32x16[7] * rhs_r32x16[1] + lhs_r32x16[11] * rhs_r32x16[2] + lhs_r32x16[15] * rhs_r32x16[3];

    m[4] = lhs_r32x16[0] * rhs_r32x16[4] + lhs_r32x16[4] * rhs_r32x16[5] + lhs_r32x16[8] * rhs_r32x16[6] + lhs_r32x16[12] * rhs_r32x16[7];
    m[5] = lhs_r32x16[1] * rhs_r32x16[4] + lhs_r32x16[5] * rhs_r32x16[5] + lhs_r32x16[9] * rhs_r32x16[6] + lhs_r32x16[13] * rhs_r32x16[7];
    m[6] = lhs_r32x16[2] * rhs_r32x16[4] + lhs_r32x16[6] * rhs_r32x16[5] + lhs_r32x16[10] * rhs_r32x16[6] + lhs_r32x16[14] * rhs_r32x16[7];
    m[7] = lhs_r32x16[3] * rhs_r32x16[4] + lhs_r32x16[7] * rhs_r32x16[5] + lhs_r32x16[11] * rhs_r32x16[6] + lhs_r32x16[15] * rhs_r32x16[7];

    m[8] = lhs_r32x16[0] * rhs_r32x16[8] + lhs_r32x16[4] * rhs_r32x16[9] + lhs_r32x16[8] * rhs_r32x16[10] + lhs_r32x16[12] * rhs_r32x16[11];
    m[9] = lhs_r32x16[1] * rhs_r32x16[8] + lhs_r32x16[5] * rhs_r32x16[9] + lhs_r32x16[9] * rhs_r32x16[10] + lhs_r32x16[13] * rhs_r32x16[11];
    m[10] = lhs_r32x16[2] * rhs_r32x16[8] + lhs_r32x16[6] * rhs_r32x16[9] + lhs_r32x16[10] * rhs_r32x16[10] + lhs_r32x16[14] * rhs_r32x16[11];
    m[11] = lhs_r32x16[3] * rhs_r32x16[8] + lhs_r32x16[7] * rhs_r32x16[9] + lhs_r32x16[11] * rhs_r32x16[10] + lhs_r32x16[15] * rhs_r32x16[11];

    m[12] = lhs_r32x16[0] * rhs_r32x16[12] + lhs_r32x16[4] * rhs_r32x16[13] + lhs_r32x16[8] * rhs_r32x16[14] + lhs_r32x16[12] * rhs_r32x16[15];
    m[13] = lhs_r32x16[1] * rhs_r32x16[12] + lhs_r32x16[5] * rhs_r32x16[13] + lhs_r32x16[9] * rhs_r32x16[14] + lhs_r32x16[13] * rhs_r32x16[15];
    m[14] = lhs_r32x16[2] * rhs_r32x16[12] + lhs_r32x16[6] * rhs_r32x16[13] + lhs_r32x16[10] * rhs_r32x16[14] + lhs_r32x16[14] * rhs_r32x16[15];
    m[15] = lhs_r32x16[3] * rhs_r32x16[12] + lhs_r32x16[7] * rhs_r32x16[13] + lhs_r32x16[11] * rhs_r32x16[14] + lhs_r32x16[15] * rhs_r32x16[15];

    fsMemHelper::memcpy(res_r32x16, m, sizeof(m));
}
