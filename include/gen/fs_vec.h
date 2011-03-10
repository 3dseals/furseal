/*
 * fs_vec.h
 *
 *  Created on: 2011-3-10
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */

#ifndef FS_VEC_H_
#define FS_VEC_H_


struct fsMat;


/*!
    @ingroup fsGen
    A 3-element vector which is represented by xyz coordinates.
*/
struct FS_API fsVec
{
    static const fsVec ZERO; //!< The zero vector.
    static const fsVec X_UNIT; //!< The orthonormal x-axis.
    static const fsVec Y_UNIT; //!< The orthonormal y-axis.
    static const fsVec Z_UNIT; //!< The orthonormal z-axis.

    r32 x; //!< The x-coordinate.
    r32 y; //!< The y-coordinate.
    r32 z; //!< The z-coordinate.

    /*!
        Constructs a fsVec.
    */
    fsVec();

    /*!
        Constructs and initializes a fsVec from the specified xyz coordinates.
        @param[in] x_ The x-coordinate.
        @param[in] y_ The y-coordinate.
        @param[in] z_ The z-coordinate. If not specified, 0.0f is used.
    */
    fsVec(r32 x_, r32 y_, r32 z_ = 0.0f);

    /*!
        Sets all components from the specified xyz coordinates.
        @param[in] x_ The x-coordinate.
        @param[in] y_ The y-coordinate.
        @param[in] z_ The z-coordinate. If not specified, 0.0f is used.
    */
    void set(r32 x_, r32 y_, r32 z_ = 0.0f);

    /*!
        Returns a fsVec which is the sum of this fsVec and the right hand side fsVec.
        @param[in] vec The right hand side fsVec.
        @return A fsVec which is the sum of two fsVecs.
    */
    fsVec operator+(const fsVec& vec) const;

    /*!
        Adds the right hand side fsVec to this fsVec.
        @param[in] vec The right hand side fsVec.
    */
    void operator+=(const fsVec& vec);

    /*!
        Returns a fsVec which is the negative of this fsVec.
        @return A fsVec which is the negative of this fsVec.
    */
    fsVec operator-() const;

    /*!
        Returns a fsVec which is the right hand side fsVec subtracted from this fsVec.
        @param[in] vec The right hand side fsVec.
        @return A fsVec which is the right hand side fsVec subtracted from this fsVec.
    */
    fsVec operator-(const fsVec& vec) const;

    /*!
        Subtracts the right hand side fsVec from this fsVec.
        @param[in] vec The right hand side fsVec.
    */
    void operator-=(const fsVec& vec);

    /*!
        Returns a fsVec which is this fsVec multiplied with the right hand side scalar.
        @param[in] s The right hand side scalar.
        @return A fsVec which is this fsVec multiplied with the right hand side scalar.
    */
    fsVec operator*(r32 s) const;

    /*!
        Returns a fsVec which is the right hand side fsVec multiplied with the left hand side scalar.
        @param[in] s The left hand side scalar.
        @param[in] vec The right hand side fsVec.
        @return A fsVec which is the right hand side fsVec multiplied with the left hand side scalar.
    */
    FS_API friend fsVec operator*(r32 s, const fsVec& vec);

    /*!
        Multiplies this fsVec with the right hand side scalar.
        @param[in] s The right hand side scalar.
    */
    void operator*=(r32 s);

    /*!
        Returns a fsVec which is this fsVec divided by the right hand side scalar.
        @param[in] s The right hand side scalar.
        @return A fsVec which is this fsVec divided by the right hand side scalar.
    */
    fsVec operator/(r32 s) const;

    /*!
        Divides this fsVec by the right hand side scalar.
        @param[in] s The right hand side scalar.
    */
    void operator/=(r32 s);

    /*!
        Returns the length of this fsVec.
        @return The length of this fsVec.
    */
    r32 length() const;

    /*!
        Returns the squared length of this fsVec.
        @return The squared length of this fsVec.
    */
    r32 sqLength() const;

    /*!
        Returns the distance between this fsVec and an another fsVec.
        @param[in] vec An another fsVec.
        @return The distance between two fsVecs.
    */
    r32 dist(const fsVec& vec) const;

    /*!
        Returns the squared distance between this fsVec and an another fsVec.
        @param[in] vec An another fsVec.
        @return The squared distance between two fsVecs.
    */
    r32 sqDist(const fsVec& vec) const;

    /*!
        Returns the inner product of this fsVec and an another fsVec.
        @param[in] vec An another fsVec.
        @return The inner product of two fsVecs.
    */
    r32 dot(const fsVec& vec) const;

    /*!
        Returns the outer product of this fsVec and an another fsVec.
        @param[in] vec An another fsVec.
        @return The outer product of two fsVecs.
    */
    fsVec cross(const fsVec& vec) const;

    /*!
        Returns a fsVec which is this fsVec rotated around the orthonormal x-axis.
        @param[in] deg An r32 angle of rotation in degrees.
        @return A fsVec which is this fsVec rotated around the orthonormal x-axis.
    */
    fsVec rotateX_r32(r32 deg) const;

    /*!
        Returns a fsVec which is this fsVec rotated around the orthonormal y-axis.
        @param[in] deg An r32 angle of rotation in degrees.
        @return A fsVec which is this fsVec rotated around the orthonormal y-axis.
    */
    fsVec rotateY_r32(r32 deg) const;

    /*!
        Returns a fsVec which is this fsVec rotated around the orthonormal z-axis.
        @param[in] deg An r32 angle of rotation in degrees.
        @return A fsVec which is this fsVec rotated around the orthonormal z-axis.
    */
    fsVec rotateZ_r32(r32 deg) const;

    /*!
        Returns a fsVec which is this fsVec rotated around the orthonormal x-axis.
        This method allows only an s32 type as an angle, but is faster than its r32 version.
        @param[in] deg An s32 angle of rotation in degrees.
        @return A fsVec which is this fsVec rotated around the orthonormal x-axis.
    */
    fsVec rotateX_s32(s32 deg) const;

    /*!
        Returns a fsVec which is this fsVec rotated around the orthonormal y-axis.
        This method allows only an s32 type as an angle, but is faster than its r32 version.
        @param[in] deg An s32 angle of rotation in degrees.
        @return A fsVec which is this fsVec rotated around the orthonormal y-axis.
    */
    fsVec rotateY_s32(s32 deg) const;

    /*!
        Returns a fsVec which is this fsVec rotated around the orthonormal z-axis.
        This method allows only an s32 type as an angle, but is faster than its r32 version.
        @param[in] deg An s32 angle of rotation in degrees.
        @return A fsVec which is this fsVec rotated around the orthonormal z-axis.
    */
    fsVec rotateZ_s32(s32 deg) const;

    /*!
        Returns a fsVec which is the normalized this fsVec.
        @return A fsVec which is the normalized this fsVec.
    */
    fsVec normalize() const;

    /*!
        Returns a fsVec which is interpolated between this fsVec and an another fsVec by the specified ratio, using linear interpolation.
        @param[in] to An another fsVec.
        @param[in] ratio The value which indicates how far to interpolate between two fsVecs. This value is clamped between 0.0f and 1.0f.
        @return A fsVec which is interpolated between two fsVecs.
    */
    fsVec interp(const fsVec& to, r32 ratio) const;

    /*!
        Returns a fsVec which is interpolated between this fsVec and an another fsVec by the specified ratio, using spherical linear interpolation.
        @param[in] to An another fsVec.
        @param[in] ratio The value which indicates how far to interpolate between two fsVecs. This value is clamped between 0.0f and 1.0f.
        @return A fsVec which is interpolated between two fsVecs.
    */
    fsVec slerp(const fsVec& to, r32 ratio) const;

    /*!
        Returns a fsVec which is this fsVec converted from in the world coordinate system to in the local coordinate system of the specified fsMat.
        @param[in] mat The origin fsMat of the local coordinate system.
        @return A fsVec which is this fsVec in the local coordinate system.
    */
    fsVec toLocalOf(const fsMat& mat) const;

    /*!
        Returns a fsVec which is this fsVec converted from in the local coordinate system of the specified fsMat to in the world coordinate system.
        @param[in] mat The origin fsMat of the local coordinate system.
        @return A fsVec which is this fsVec in the world coordinate system.
    */
    fsVec toGlobalFrom(const fsMat& mat) const;

    /*!
        Returns a fsVec which is this fsVec converted from in the world coordinate system to
        in the local coordinate system of the specified fsMat whose translation is ignored.
        @param[in] mat The origin fsMat of the local coordinate system, whose translation is ignored.
        @return A fsVec which is this fsVec in the local coordinate system.
    */
    fsVec toLocalOf_noTrans(const fsMat& mat) const;

    /*!
        Returns a fsVec which is this fsVec converted from in the local coordinate system of
        the specified fsMat whose translation is ignored to in the world coordinate system.
        @param[in] mat The origin fsMat of the local coordinate system, whose translation is ignored.
        @return A fsVec which is this fsVec in the world coordinate system.
    */
    fsVec toGlobalFrom_noTrans(const fsMat& mat) const;

    /*!
        Converts this fsVec to a 4-element array.
        @param[out] r32x4 A destination of the converted 4-element array.
        @param[in] w The 4th coordinate of this fsVec.
    */
    void toR32x4(r32* r32x4, r32 w) const;

    /*!
        Returns a fsVec which is converted from a 4-element array.
        @param[in] r32x4 A 4-element array to be converted.
        @return A fsVec which is converted from the 4-element array.
    */
    static fsVec fromR32x4(const r32* r32x4);

    /*!
        Multiplies a 16-element array by a 4-element array as a matrix and a vector respectively.
        @param[out] res_r32x4 The destination of the multiplied 4-element array.
        @param[in] lhs_r32x16 The left hand side 16-element array.
        @param[in] rhs_r32x4 The right hand side 4-element array.
    */
    static void mulR32x4(r32* res_r32x4, const r32* lhs_r32x16, const r32* rhs_r32x4);
};


#endif /* FS_VEC_H_ */
