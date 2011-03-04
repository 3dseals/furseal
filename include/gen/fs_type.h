/*
 * fs_type.h
 *
 *  Created on: 2011-3-4
 *      Author: administrator
 *
 *  Copyright (c) 2011 netsurfers
 */

#ifndef FS_TYPE_H_
#define FS_TYPE_H_


/*!
    @ingroup fsGen
    An enum type with specified size.
    @tparam D The data type to store a value of the enum type.
    @tparam T The enum type stored in the data type.
*/
template<class D, class T> class fsType
{
public:
    /*!
        Constructs and initializes a fsType to zero.
    */
    fsType()
    {
        m_type = 0;
    }

    /*!
        Constructs and initializes a fsType from the specified type.
        @param[in] type A type.
    */
    fsType(T type)
    {
        m_type = static_cast<D>(type);
    }

    /*!
        Assigns the right hand side type to this fsType.
        @param[in] type The right hand side type.
        @return A reference to this fsType.
    */
    fsType<D, T>& operator=(T type)
    {
        m_type = static_cast<D>(type);

        return *this;
    }

    /*!
        Returns whether this fsType is equal to the right hand side fsType.
        @param[in] type The right hand side fsType.
        @return Whether this fsType is equal to the right hand side fsType.
    */
    bool operator==(fsType<D, T> type) const
    {
        return (getType() == type.getType());
    }

    /*!
        Returns whether this fsType is equal to the right hand side type.
        @param[in] type The right hand side type.
        @return Whether this fsType is equal to the right hand side type.
    */
    bool operator==(T type) const
    {
        return (getType() == type);
    }

    /*!
        Returns whether the left hand side type is equal to the right hand side fsType.
        @param[in] type1 The left hand side type.
        @param[in] type2 The right hand side fsType.
        @return Whether the left hand side type is equal to the right hand side fsType.
    */
    friend bool operator==(T type1, fsType<D, T> type2)
    {
        return (type1 == type2.getType());
    }

    /*!
        Returns whether this fsType is unequal to the right hand side fsType.
        @param[in] type The right hand side fsType.
        @return Whether this fsType is unequal to the right hand side fsType.
    */
    bool operator!=(fsType<D, T> type) const
    {
        return (getType() != type.getType());
    }

    /*!
        Returns whether this fsType is unequal to the right hand side type.
        @param[in] type The right hand side type.
        @return Whether this fsType is unequal to the right hand side type.
    */
    bool operator!=(T type) const
    {
        return (getType() != type);
    }

    /*!
        Returns whether the left hand side type is unequal to the right hand side fsType.
        @param[in] type1 The left hand side type.
        @param[in] type2 The right hand side fsType.
        @return Whether the left hand side type is unequal to the right hand side fsType.
    */
    friend bool operator!=(T type1, fsType<D, T> type2)
    {
        return (type1 != type2.getType());
    }

    /*!
        Returns the value of this fsType as the enum type.
        @return The value of this fsType as the enum type.
    */
    T getType() const
    {
        return static_cast<T>(m_type);
    }

    /*!
        Returns the value of this fsType as the data type.
        @return The value of this fsType as the data type.
    */
    D getValue() const
    {
        return m_type;
    }

private:
    D m_type;
};


//! @cond
template<class D> class fsType<D, bool>
{
public:
    fsType()
    {
        m_type = 0;
    }

    fsType(bool type)
    {
        m_type = static_cast<D>(type);
    }

    fsType<D, bool>& operator=(bool type)
    {
        m_type = static_cast<D>(type);

        return *this;
    }

    bool operator==(fsType<D, bool> type) const
    {
        return (getType() == type.getType());
    }

    bool operator==(bool type) const
    {
        return (getType() == type);
    }

    friend bool operator==(bool type1, fsType<D, bool> type2)
    {
        return (type1 == type2.getType());
    }

    bool operator!=(fsType<D, bool> type) const
    {
        return (getType() != type.getType());
    }

    bool operator!=(bool type) const
    {
        return (getType() != type);
    }

    friend bool operator!=(bool type1, fsType<D, bool> type2)
    {
        return (type1 != type2.getType());
    }

    bool getType() const
    {
        return m_type ? true : false;
    }

    D getValue() const
    {
        return m_type;
    }

private:
    D m_type;
};


#endif /* FS_TYPE_H_ */
