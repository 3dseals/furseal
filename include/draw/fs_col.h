/*
 * fs_col.h
 *
 *  Created on: 2011-3-10
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */

#ifndef FS_COL_H_
#define FS_COL_H_


/*!
    @ingroup fsDraw
    A color, which is represented by red, green, blue and alpha components.
*/
struct FS_API fsCol
{
    static const fsCol ZERO; //!< The color whose all components are 0s.
    static const fsCol FULL; //!< The color whose all components are 255s.

    u8 r; //!< Red component.
    u8 g; //!< Green component.
    u8 b; //!< Blue component.
    u8 a; //!< Alpha component.

    /*!
        Constructs a fsCol.
    */
    fsCol();

    /*!
        Constructs and initializes a fsCol from the specified fsCol.
        @param[in] col A fsCol.
    */
    fsCol(const fsCol& col);

    /*!
        Constructs and initializes a fsCol from the specified components.
        @param[in] r_ A red component.
        @param[in] g_ A green component.
        @param[in] b_ A blue component.
        @param[in] a_ An alpha component. If not specified, 255 is used.
    */
    fsCol(u8 r_, u8 g_, u8 b_, u8 a_ = 255);

    /*!
        Sets all components from the specified components.
        @param[in] r_ A red component.
        @param[in] g_ A green component.
        @param[in] b_ A blue component.
        @param[in] a_ An alpha component. If not specified, 255 is used.
    */
    void set(u8 r_, u8 g_, u8 b_, u8 a_ = 255);

    /*!
        Returns whether this fsCol is equal to the right hand side fsCol.
        @param[in] col The right hand side fsCol.
        @return Whether this fsCol is equal to the right hand side fsCol.
    */
    bool operator==(fsCol col) const;

    /*!
        Returns whether this fsCol is unequal to the right hand side fsCol.
        @param[in] col The right hand side fsCol.
        @return Whether this fsCol is unequal to the right hand side fsCol.
    */
    bool operator!=(fsCol col) const;

    /*!
        Returns a fsCol which is the sum of this fsCol and the right hand side fsCol.
        The values of the components are clamped between 0 and 255.
        @param[in] col The right hand side fsCol.
        @return A fsCol which is the sum of two fsCols.
    */
    fsCol operator+(fsCol col) const;

    /*!
        Adds the right hand side fsCol to this fsCol.
        The values of the components are clamped between 0 and 255.
        @param[in] col The right hand side fsCol.
    */
    void operator+=(fsCol col);

    /*!
        Returns a fsCol which is the right hand side fsCol subtracted from this fsCol.
        The values of the components are clamped between 0 and 255.
        @param[in] col The right hand side fsCol.
        @return A fsCol which is the right hand side fsCol subtracted from this fsCol.
    */
    fsCol operator-(fsCol col) const;

    /*!
        Subtracts the right hand side fsCol from this fsCol.
        The values of the components are clamped between 0 and 255.
        @param[in] col The right hand side fsCol.
    */
    void operator-=(fsCol col);

    /*!
        Returns a fsCol which is this fsCol multiplied with right hand side fsCol and divided by 255.
        The values of the components are clamped between 0 and 255.
        @param[in] col The right hand side fsCol.
        @return A fsCol which is this fsCol multiplied with right hand side fsCol and divided by 255.
    */
    fsCol operator*(fsCol col) const;

    /*!
        Multiplies this fsVec with the right hand side fsCol and divides by 255.
        The values of the components are clamped between 0 and 255.
        @param[in] col The right hand side fsCol.
    */
    void operator*=(fsCol col);

    /*!
        Returns a fsCol which is this fsCol multiplied with right hand side scalar and divided by 255.
        The values of the components are clamped between 0 and 255.
        @param[in] s The right hand side scalar.
        @return A fsCol which is this fsCol multiplied with right hand side scalar and divided by 255.
    */
    fsCol operator*(r32 s) const;

    /*!
        Returns a fsCol which is the right hand side fsCol multiplied with the left hand side scalar.
        The values of the components are clamped between 0 and 255.
        @param[in] s The left hand side scalar.
        @param[in] col The right hand side fsCol.
        @return A fsCol which is the right hand side fsCol multiplied with the left hand side scalar.
    */
    FS_API friend fsCol operator*(r32 s, fsCol col);

    /*!
        Multiplies this fsCol with the right hand side scalar.
        The values of the components are clamped between 0 and 255.
        @param[in] s The right hand side scalar.
    */
    void operator*=(r32 s);

    /*!
        Returns a fsCol which is this fsCol divided by the right hand side scalar.
        The values of the components are clamped between 0 and 255.
        @param[in] s The right hand side scalar.
        @return A fsCol which is this fsCol divided by the right hand side scalar.
    */
    fsCol operator/(r32 s) const;

    /*!
        Divides this fsCol by the right hand side scalar.
        The values of the components are clamped between 0 and 255.
        @param[in] s The right hand side scalar.
    */
    void operator/=(r32 s);

    /*!
        Returns a fsCol which is interpolated between this fsCol and an another fsCol by the specified ratio, using linear interpolation.
        @param[in] to An another fsCol.
        @param[in] ratio The value which indicates how far to interpolate between two fsCols. This value is clamped between 0.0f and 1.0f.
        @return A fsCol which is interpolated between two fsCols.
    */
    fsCol interp(fsCol to, r32 ratio) const;
};


#endif /* FS_COL_H_ */
