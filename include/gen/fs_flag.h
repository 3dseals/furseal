/*
 * fs_flag.h
 *
 *  Created on: 2011-3-4
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */

#ifndef FS_FLAG_H_
#define FS_FLAG_H_


/*!
    @ingroup fsGen
    以一系列的bit作为标志位的数据结构.
    @tparam D 存储bit的数据类型.
    @tparam T 表示每个bit位的枚举类型,枚举的数目必须小于bit位数.
*/
template<class D, class T> class fsFlag
{
public:
    /*!
        构建初始化fsFlag,清除所有标志位.
    */
    fsFlag()
    {
        clear();
    }

    /*!
        重载==运算,判断右边的标志是否相等.
        @param[in] flag 右边的标志fsFlag.
        @return 右边的标志是否相等.
    */
    bool operator==(fsFlag<D, T> flag) const
    {
        return (m_flag == flag.m_flag);
    }

    /*!
        重载!=运算,判断右边的标志是否不等.
        @param[in] flag 右边的标志fsFlag.
        @return 右边的标志是否不等.
    */
    bool operator!=(fsFlag<D, T> flag) const
    {
        return (m_flag != flag.m_flag);
    }

    /*!
        返回标志的数据.
        @return 标志fsFlag的数据.
    */
    D getValue() const
    {
        return m_flag;
    }

    /*!
        判断某个标志位是否启用.
        @param[in] flag 一个枚举类型中定义的标志.
        @return 某个标志位是否启用.
    */
    bool isOn(T flag) const
    {
        return (m_flag & (1 << flag)) ? true : false;
    }

    /*!
        判断某个标志位是否禁用.
        @param[in] flag 一个枚举类型中定义的标志.
        @return 某个标志位是否禁用.
    */
    bool isOff(T flag) const
    {
        return (m_flag & (1 << flag)) ? false : true;
    }

    /*!
        设置某个标志位启用.
        @param[in] flag 一个枚举类型中定义的标志.
    */
    void setOn(T flag)
    {
        m_flag |= (1 << flag);
    }

    /*!
        设置某个标志位禁用.
        @param[in] flag 一个枚举类型中定义的标志.
    */
    void setOff(T flag)
    {
        m_flag &= ~(1 << flag);
    }

    /*!
        设置某个标志位是否启用.
        @param[in] flag 一个枚举类型中定义的标志.
        @param[in] is_on 如果值为true, 启用标志否则禁用标志.
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
        设置所有标志为0.
    */
    void clear()
    {
        m_flag = 0;
    }

private:
    D m_flag;
};


#endif /* FS_FLAG_H_ */
