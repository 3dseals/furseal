/*
 * fs_mat.h
 *
 *  Created on: 2011-3-10
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */

#ifndef FS_MAT_H_
#define FS_MAT_H_


/*!
    @ingroup fsGen
    一个 3x4 的矩阵由 4 个 fsVec 向量组成.
*/
struct FS_API fsMat
{
    static const fsMat ZERO; //!< The zero matrix.
    static const fsMat UNIT; //!< The unit matrix.

    fsVec x_axis; //!< The 1st row, which is usually used as the x-axis.
    fsVec y_axis; //!< The 2nd row, which is usually used as the y-axis.
    fsVec z_axis; //!< The 3rd row, which is usually used as the z-axis.
    fsVec trans; //!< The 4th row, which is usually used as the translation.

    /*!
        Constructs a fsMat.
    */
    fsMat();

    /*!
        Constructs and initialize a fsMat from the specified four fsVecs.
        @param[in] x_axis_ The x-axis.
        @param[in] y_axis_ The y-axis.
        @param[in] z_axis_ The z-axis.
        @param[in] trans_ The translation.
    */
    fsMat(const fsVec& x_axis_, const fsVec& y_axis_, const fsVec& z_axis_, const fsVec& trans_);

    /*!
        Sets all components from the specified four fsVecs.
        @param[in] x_axis_ The x-axis.
        @param[in] y_axis_ The y-axis.
        @param[in] z_axis_ The z-axis.
        @param[in] trans_ The translation.
    */
    void set(const fsVec& x_axis_, const fsVec& y_axis_, const fsVec& z_axis_, const fsVec& trans_);

    /*!
        Returns whether this fsMat is equal to the unit matrix.
        @return Whether this fsMat is equal to the unit matrix.
    */
    bool isUnit() const;

    /*!
        Returns a fsMat which is this fsMat rotated around its x-axis.
        @param[in] deg An r32 angle of rotation in degrees.
        @return A fsMat which is this fsMat rotated around its x-axis.
    */
    fsMat rotateX_r32(r32 deg) const;

    /*!
        Returns a fsMat which is this fsMat rotated around its y-axis.
        @param[in] deg An r32 angle of rotation in degrees.
        @return A fsMat which is this fsMat rotated around its y-axis.
    */
    fsMat rotateY_r32(r32 deg) const;

    /*!
        Returns a fsMat which is this fsMat rotated around its z-axis.
        @param[in] deg An r32 angle of rotation in degrees.
        @return A fsMat which is this fsMat rotated around its z-axis.
    */
    fsMat rotateZ_r32(r32 deg) const;

    /*!
        Returns a fsMat which is this fsMat rotated around its x-axis.
        This method allows only an s32 type as an angle, but is faster than its r32 version.
        @param[in] deg An s32 angle of rotation in degrees.
        @return A fsMat which is this fsMat rotated around its x-axis.
    */
    fsMat rotateX_s32(s32 deg) const;

    /*!
        Returns a fsMat which is this fsMat rotated around its y-axis.
        This method allows only an s32 type as an angle, but is faster than its r32 version.
        @param[in] deg An s32 angle of rotation in degrees.
        @return A fsMat which is this fsMat rotated around its y-axis.
    */
    fsMat rotateY_s32(s32 deg) const;

    /*!
        Returns a fsMat which is this fsMat rotated around its z-axis.
        This method allows only an s32 type as an angle, but is faster than its r32 version.
        @param[in] deg An s32 angle of rotation in degrees.
        @return A fsMat which is this fsMat rotated around its z-axis.
    */
    fsMat rotateZ_s32(s32 deg) const;

    /*!
        Returns a fsMat which is this fsMat scaled by the specified values.
        @param[in] x_scale The x-axis scale factor.
        @param[in] y_scale The y-axis scale factor.
        @param[in] z_scale The z-axis scale factor. If not specified, 1.0f is used.
        @return A fsMat which is this fsMat scaled by the specified values.
    */
    fsMat scale(r32 x_scale, r32 y_scale, r32 z_scale = 1.0f) const;

    /*!
        Returns a fsMat which is this fsMat translated along its axes for the specified lengths.
        @param[in] x A length of translation along the x-axis.
        @param[in] y A length of translation along the y-axis.
        @param[in] z A length of translation along the z-axis. If not specified, 0.0f is used.
        @return A fsMat which is this fsMat translated along its axes for the specified lengths.
    */
    fsMat translate(r32 x, r32 y, r32 z = 0.0f) const;

    /*!
        Returns a fsMat which is interpolated between this fsMat and an another fsMat by the specified ratio, using spherical linear interpolation.
        @param[in] to An another fsMat.
        @param[in] ratio The value which indicates how far to interpolate between two fsMats. This value is clamped between 0.0f and 1.0f.
        @return A fsMat which is interpolated between two fsMats.
    */
    fsMat slerp(const fsMat& to, r32 ratio) const;

    /*!
        Returns a fsMat whose components except the translation is interpolated between this fsMat and an another fsMat by the specified ratio,
        using spherical linear interpolation, and whose translation is equal to the translation of this fsMat.
        @param[in] to An another fsMat.
        @param[in] ratio The value which indicates how far to interpolate between two fsMats. This value is clamped between 0.0f and 1.0f.
        @return A fsMat whose components except the translation is interpolated between two fsMats.
    */
    fsMat slerp_noTrans(const fsMat& to, r32 ratio) const;

    /*!
        Returns a fsMat which is orthonormalized version of this fsMat.
        @return A fsMat which is orthonormalized version of this fsMat.
    */
    fsMat orthonormal() const;

    /*!
        Returns a fsMat which is this fsMat converted from in the world coordinate system to in the local coordinate system of the specified fsMat.
        @param[in] mat The origin fsMat of the local coordinate system.
        @return A fsMat which is this fsMat in the local coordinate system.
    */
    fsMat toLocalOf(const fsMat& mat) const;

    /*!
        Returns a fsMat which is this fsMat converted from in the local coordinate system of the specified fsMat to in the world coordinate system.
        @param[in] mat The origin fsMat of the local coordinate system.
        @return A fsMat which is this fsMat in the world coordinate system.
    */
    fsMat toGlobalFrom(const fsMat& mat) const;

    /*!
        Returns a fsMat which is this fsMat converted from in the world coordinate system to
        in the local coordinate system of the specified fsMat whose translation is ignored.
        @param[in] mat The origin fsMat of the local coordinate system, whose translation is ignored.
        @return A fsMat which is this fsMat in the local coordinate system.
    */
    fsMat toLocalOf_noTrans(const fsMat& mat) const;

    /*!
        Returns a fsMat which is this fsMat converted from in the local coordinate system of
        the specified fsMat whose translation is ignored to in the world coordinate system.
        @param[in] mat The origin fsMat of the local coordinate system, whose translation is ignored.
        @return A fsMat which is this fsMat in the world coordinate system.
    */
    fsMat toGlobalFrom_noTrans(const fsMat& mat) const;

    /*!
        Returns a fsMat which is the look-at matrix built by the specified values.
        @param[in] from The position of the camera.
        @param[in] to The position of the camera look-at target.
        @param[in] up The up direction of the camera.
        @return A fsMat which is the look-at matrix built by the specified values.
    */
    static fsMat lookAt(const fsVec& from, const fsVec& to, const fsVec& up);

    /*!
        Converts this fsMat to a 16-element array.
        @param[out] r32x16 A destination of the converted 16-element array.
    */
    void toR32x16(r32* r32x16) const;

    /*!
        Returns a fsMat which is converted from a 16-element array.
        @param[in] r32x16 A 16-element array to be converted.
        @return A fsMat which is converted from the 16-element array.
    */
    static fsMat fromR32x16(const r32* r32x16);

    /*!
        Multiplies two 16-element arrays as two matrices.
        @param[out] res_r32x16 The destination of the multiplied 16-element array.
        @param[in] lhs_r32x16 The left hand side 16-element array.
        @param[in] rhs_r32x16 The right hand side 16-element array.
    */
    static void mulR32x16(r32* res_r32x16, const r32* lhs_r32x16, const r32* rhs_r32x16);
};


#endif /* FS_MAT_H_ */
