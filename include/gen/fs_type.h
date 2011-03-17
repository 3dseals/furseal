/*
 * fs_type.h
 *
 *  Created on: 2011-3-4
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */

#ifndef FS_TYPE_H_
#define FS_TYPE_H_


/*!
    @ingroup fsGen
    枚举类型的数据结构.
    @tparam D 存储枚举类型的数据类型.
    @tparam T 枚举的类型.
*/
template<class D, class T> class fsType
{
public:
    /*!
        构建fsType并初始化置为0.
    */
    fsType()
    {
        m_type = 0;
    }

    /*!
        从枚举的类型构建fsType并初始化.
        @param[in] type 一个枚举的值.
    */
    fsType(T type)
    {
        m_type = static_cast<D>(type);
    }

    /*!
        重载=运算符,使得fsType值等于枚举的类型.
        @param[in] type 一个枚举的值.
        @return 这个fsType值.
    */
    fsType<D, T>& operator=(T type)
    {
        m_type = static_cast<D>(type);

        return *this;
    }

    /*!
        重载==运算符,判断是否是相等的fsType类型.
        @param[in] type 一个枚举的类型.
        @return 是否是相等的fsType类型.
    */
    bool operator==(fsType<D, T> type) const
    {
        return (getType() == type.getType());
    }

    /*!
        重载==运算符,判断是否是相等的fsType值.
        @param[in] type 一个枚举的值.
        @return 是否是相等的fsType值.
    */
    bool operator==(T type) const
    {
        return (getType() == type);
    }

    /*!
        重载==运算符,判断左值是否是等于右边的fsType类型.
        @param[in] type1 左边的fsType值.
        @param[in] type2 右边的fsType值类型.
        @return 左值是否是等于右边的fsType类型.
    */
    friend bool operator==(T type1, fsType<D, T> type2)
    {
        return (type1 == type2.getType());
    }

    /*!
        重载!=运算符,判断是否是不相等的fsType类型.
        @param[in] type 一个枚举的类型.
        @return 是否是不相等的fsType类型.
    */
    bool operator!=(fsType<D, T> type) const
    {
        return (getType() != type.getType());
    }

    /*!
        重载!=运算符,判断是否是不相等的fsType值.
        @param[in] type 一个枚举的值.
        @return 是否是不相等的fsType值.
    */
    bool operator!=(T type) const
    {
        return (getType() != type);
    }

    /*!
        重载!=运算符,判断左值是否不等于右边的fsType类型.
        @param[in] type1 左边的fsType值.
        @param[in] type2 右边的fsType值类型.
        @return 左值是否不等于右边的fsType类型.
    */
    friend bool operator!=(T type1, fsType<D, T> type2)
    {
        return (type1 != type2.getType());
    }

    /*!
        返回fsType的枚举类型.
        @return fsType的枚举类型.
    */
    T getType() const
    {
        return static_cast<T>(m_type);
    }

    /*!
        返回fsType的枚举值.
        @return fsType的枚举值.
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
