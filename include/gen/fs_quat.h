/*
 * fs_quat.h
 *
 *  Created on: 2011-3-10
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */

#ifndef FS_QUAT_H_
#define FS_QUAT_H_



/*!
    @ingroup fsMath
    四元数组.
*/
struct FS_API fsQuat
{
    r32 x; //!< The x-coordinate.
    r32 y; //!< The y-coordinate.
    r32 z; //!< The z-coordinate.
    r32 w; //!< The w-coordinate.

    /*!
        Constructs a fsQuat.
    */
    fsQuat();

    /*!
        Constructs and initialize a fsQaut from the specified xyzw coordinates.
        @param[in] x_ The x-coordinate.
        @param[in] y_ The y-coordinate.
        @param[in] z_ The z-coordinate.
        @param[in] w_ The w-coordinate.
    */
    fsQuat(r32 x_, r32 y_, r32 z_, r32 w_);

    /*!
        Sets all components from the specified xyzw coordinates.
        @param[in] x_ The x-coordinate.
        @param[in] y_ The y-coordinate.
        @param[in] z_ The z-coordinate.
        @param[in] w_ The w-coordinate.
    */
    void set(r32 x_, r32 y_, r32 z_, r32 w_);

    /*!
        Returns a fsQuat which is interpolated between this fsQuat and an another fsQuat by the specified ratio, using spherical linear interpolation.
        @param[in] to An another fsQuat.
        @param[in] ratio The value which indicates how far to interpolate between two fsQuats. This value is clamped between 0.0f and 1.0f.
        @return A fsQuat which is interpolated between two fsQuats.
    */
    fsQuat slerp(const fsQuat& to, r32 ratio) const;

    /*!
        Returns a fsMat which is converted from the axes of this fsQuat and the translation of the specified fsVec.
        @param[in] trans The translation.
        @return A fsMat which is converted from the axes of this fsQuat and the translation of the specified fsVec.
    */
    fsMat toMat(const fsVec& trans) const;

    /*!
        Returns a fsQuat which is converted from the specified fsMat.
        @param[in] mat A fsMat, whose translation is ignored.
        @return A fsQuat which is converted from the specified fsMat.
    */
    static fsQuat fromMat(const fsMat& mat);
};


#endif /* FS_QUAT_H_ */
