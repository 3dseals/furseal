/*
 * fs_flag.h
 *
 *  Created on: 2011-3-4
 *      Author: administrator
 *
 *  Copyright (c) 2011 netsurfers
 */

#ifndef FS_FLAG_H_
#define FS_FLAG_H_


/*!
    @ingroup fsGen
    A set of bits as a data structure.
    @tparam D The data type to store flag bits.
    @tparam T The enum type whose constant values mean bit number. The each value of the enum constants must be less than the number of bits of the data type.
*/
template<class D, class T> class fsFlag
{
public:
    /*!
        Constructs and initializes a fsFlag whose all bits are off.
    */
    fsFlag()
    {
        clear();
    }

    /*!
        Returns whether this fsFlag is equal to the right hand side fsFlag.
        @param[in] flag The right hand side fsFlag.
        @return Whether this fsFlag is equal to the right hand side fsFlag.
    */
    bool operator==(fsFlag<D, T> flag) const
    {
        return (m_flag == flag.m_flag);
    }

    /*!
        Returns whether this fsFlag is unequal to the right hand side fsFlag.
        @param[in] flag The right hand side fsFlag.
        @return Whether this fsFlag is unequal to the right hand side fsFlag.
    */
    bool operator!=(fsFlag<D, T> flag) const
    {
        return (m_flag != flag.m_flag);
    }

    /*!
        Returns the bits of this fsFlag as the data type.
        @return The bits of this fsFlag as the data type.
    */
    D getValue() const
    {
        return m_flag;
    }

    /*!
        Returns whether the specified flag is on.
        @param[in] flag A flag which is a value of the enum constants.
        @return Whether the specified flag is on.
    */
    bool isOn(T flag) const
    {
        return (m_flag & (1 << flag)) ? true : false;
    }

    /*!
        Returns whether the specified flag is off.
        @param[in] flag A flag which is a value of the enum constants.
        @return Whether the specified flag is off.
    */
    bool isOff(T flag) const
    {
        return (m_flag & (1 << flag)) ? false : true;
    }

    /*!
        Sets the specified flag on.
        @param[in] flag A flag which is a value of the enum constants.
    */
    void setOn(T flag)
    {
        m_flag |= (1 << flag);
    }

    /*!
        Sets the specified flag off.
        @param[in] flag A flag which is a value of the enum constants.
    */
    void setOff(T flag)
    {
        m_flag &= ~(1 << flag);
    }

    /*!
        Sets the specified flag to the specified value.
        @param[in] flag A flag which is a value of the enum constants.
        @param[in] is_on If true, the flag is set on. Otherwise, the flag is set off.
    */
    void set(T flag, bool is_on)
    {
        if (is_on)
        {
            setOn(flag);
        }
        else
        {
            setOff(flag);
        }
    }

    /*!
        Sets all bits of this fsFlag off.
    */
    void clear()
    {
        m_flag = 0;
    }

private:
    D m_flag;
};


#endif /* FS_FLAG_H_ */
