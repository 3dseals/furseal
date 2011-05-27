/*
 * fs_fix.h
 *
 *  Created on: 2011-5-27
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */

#ifndef FS_FIX_H_
#define FS_FIX_H_

/*!
    @ingroup fsGen
    A 32-bit fixed point number.
*/
class FS_API fsFix
{
public:
    //! @cond
    fsDefineException(ExceptionOverflow);
    fsDefineException(ExceptionUnderflow);
    //! @endcond

    /*!
        Constructs a fsFix.
    */
    fsFix();

    /*!
        Constructs and initialize a fsFix from the specified s32 value.
        @param[in] n An s32 value.
    */
    fsFix(s32 n);

    /*!
        Constructs and initialize a fsFix from the specified r32 value.
        @param[in] r An r32 value.
    */
    fsFix(r32 r);

    /*!
        Returns whether this fsFix is equal to the right hand side fsFix.
        @param[in] fix The right hand side fsFix.
        @return Whether this fsFix is equal to the right hand side fsFix.
    */
    bool operator==(fsFix fix) const;

    /*!
        Returns whether this fsFix is unequal to the right hand side fsFix.
        @param[in] fix The right hand side fsFix.
        @return Whether this fsFix is unequal to the right hand side fsFix.
    */
    bool operator!=(fsFix fix) const;

    /*!
        Returns whether this fsFix is less than the right hand side fsFix.
        @param[in] fix The right hand side fsFix.
        @return Whether this fsFix is less than the fsFix.
    */
    bool operator<(fsFix fix) const;

    /*!
        Returns whether this fsFix is greater than the right hand side fsFix.
        @param[in] fix The right hand side fsFix.
        @return Whether this fsFix is greater than the fsFix.
    */
    bool operator>(fsFix fix) const;

    /*!
        Returns whether this fsFix is less than or equal to the right hand side fsFix.
        @param[in] fix The right hand side fsFix.
        @return Whether this fsFix is less than or equal to the fsFix.
    */
    bool operator<=(fsFix fix) const;

    /*!
        Returns whether this fsFix is greater than or equal to the right hand side fsFix.
        @param[in] fix The right hand side fsFix.
        @return Whether this fsFix is greater than or equal to the fsFix.
    */
    bool operator>=(fsFix fix) const;

    /*!
        Returns a fsFix which is the sum of this fsFix and the right hand side fsFix.
        @param[in] fix The right hand side fsFix.
        @return A fsFix which is the sum of two fsFixes.
    */
    fsFix operator+(fsFix fix) const;

    /*!
        Returns a fsFix which is the sum of the left hand side s32 value and the right hand side fsFix.
        @param[in] n The left hand side s32 value.
        @param[in] fix The right hand side fsFix.
        @return a fsFix which is the sum of the left hand side s32 value and the right hand side fsFix.
    */
    FS_API friend fsFix operator+(s32 n, fsFix fix);

    /*!
        Returns a fsFix which is the sum of the left hand side r32 value and the right hand side fsFix.
        @param[in] r The left hand side r32 value.
        @param[in] fix The right hand side fsFix.
        @return a fsFix which is the sum of the left hand side r32 value and the right hand side fsFix.
    */
    FS_API friend fsFix operator+(r32 r, fsFix fix);

    /*!
        Adds the right hand side fsFix to this fsFix.
        @param[in] fix The right hand side fsFix.
    */
    void operator+=(fsFix fix);

    /*!
        Returns a fsFix which is the negative of this fsFix.
        @return A fsFix which is the negative of this fsFix.
    */
    fsFix operator-() const;

    /*!
        Returns a fsFix which is the right hand side fsFix subtracted from this fsFix.
        @param[in] fix The right hand side fsFix.
        @return A fsFix which is the right hand side fsFix subtracted from this fsFix.
    */
    fsFix operator-(fsFix fix) const;

    /*!
        Returns a fsFix which is the right hand side fsFix subtracted from the left hand side s32 value.
        @param[in] n The left hand side s32 value.
        @param[in] fix The right hand side fsFix.
        @return A fsFix which is the right hand side fsFix subtracted from the left hand side s32 value.
    */
    FS_API friend fsFix operator-(s32 n, fsFix fix);

    /*!
        Returns a fsFix which is the right hand side fsFix subtracted from the left hand side r32 value.
        @param[in] r The left hand side r32 value.
        @param[in] fix The right hand side fsFix.
        @return A fsFix which is the right hand side fsFix subtracted from the left hand side r32 value.
    */
    FS_API friend fsFix operator-(r32 r, fsFix fix);

    /*!
        Subtracts the right hand side fsFix from this fsFix.
        @param[in] fix The right hand side fsFix.
    */
    void operator-=(fsFix fix);

    /*!
        Returns a fsFix which is this fsFix multiplied with the right hand side fsFix.
        @param[in] fix The right hand side fsFix.
        @return A fsFix which is this fsFix multiplied with the right hand side fsFix.
    */
    fsFix operator*(fsFix fix) const;

    /*!
        Returns a fsFix which is the left hand side s32 value multiplied with the right hand side fsFix.
        @param[in] n The left hand side s32 value.
        @param[in] fix The right hand side fsFix.
        @return A fsFix which is the left hand side s32 value multiplied with the right hand side fsFix.
    */
    FS_API friend fsFix operator*(s32 n, fsFix fix);

    /*!
        Returns a fsFix which is the left hand side r32 value multiplied with the right hand side fsFix.
        @param[in] r The left hand side r32 value.
        @param[in] fix The right hand side fsFix.
        @return A fsFix which is the left hand side r32 value multiplied with the right hand side fsFix.
    */
    FS_API friend fsFix operator*(r32 r, fsFix fix);

    /*!
        Multiplies this fsFix with the right hand side fsFix.
        @param[in] fix The right hand side fsFix.
    */
    void operator*=(fsFix fix);

    /*!
        Returns a fsFix which is this fsFix divided by the right hand side fsFix.
        @param[in] fix The right hand side fsFix.
        @return A fsVec which is this fsFix divided by the right hand side fsFix.
    */
    fsFix operator/(fsFix fix) const;

    /*!
        Returns a fsFix which is the left hand side s32 value divided by the right hand side fsFix.
        @param[in] n The left hand side s32 value.
        @param[in] fix The right hand side fsFix.
        @return A fsFix which is the left hand side s32 value devided by the right hand side fsFix.
    */
    FS_API friend fsFix operator/(s32 n, fsFix fix);

    /*!
        Returns a fsFix which is the left hand side r32 value divided by the right hand side fsFix.
        @param[in] r The left hand side r32 value.
        @param[in] fix The right hand side fsFix.
        @return A fsFix which is the left hand side r32 value divided by the right hand side fsFix.
    */
    FS_API friend fsFix operator/(r32 r, fsFix fix);

    /*!
        Divides this fsFix by the right hand side fsFix.
        @param[in] fix The right hand side fsFix.
    */
    void operator/=(fsFix fix);

    /*!
        Returns an s32 value which is converted from this fsFix.
        @return An s32 value which is converted from this fsFix.
    */
    s32 toS32() const;

    /*!
        Returns an r32 value which is converted from this fsFix.
        @return An r32 value which is converted from this fsFix.
    */
    r32 toR32() const;

    /*!
        Returns the value of this fsFix in the internal format.
        @return The value of thie fsFix in the internal format.
    */
    s32 getValue() const;

    /*!
        Returns a fsFix which is constructed from the specified value in the internal format.
        @param[in] value A value in the internal format.
        @return A fsFix which is constructed from the specified value in the internal format.
    */
    static fsFix fromValue(s32 value);

private:
    static const u32 BIT_NUM = 32;
    static const u32 FRACTION_BIT_NUM = 16;

    static void checkOverflow(s64 value);

    s32 m_value;
};


#endif /* FS_FIX_H_ */
