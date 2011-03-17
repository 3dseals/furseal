/*
 * fs_str.h
 *
 *  Created on: 2011-3-9
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */


/*!
    @ingroup fsGen
    一个修复了长度的字符串类型.
    @tparam T 组成该字符串的元素类型,一般为char或者wchar_t.
    @tparam N 字符串的长度,必须大于0.
*/
template<class T, u8 N> class fsStr
{
public:
    //! @cond
    fsDefineException(ExceptionOutOfRange);
    //! @endcond

    /*!
        构建一个长度为零的fsStr.
    */
    fsStr()
    {
        m_str[0] = 0;
        m_length = 0;
    }

    /*!
        构建一个指定的fsStr.
        @param[in] ch 一个字符或宽字符.
    */
    fsStr(T ch)
    {
        *this = ch;
    }

    /*!
        构建一个指定的fsStr.
        @param[in] str 一个字符串允许为NULL.
    */
    fsStr(const T* str)
    {
        *this = str;
    }

    /*!
        拷贝构造函数fsStr.
        @param[in] str 一个fsStr.
    */
    fsStr(const fsStr<T, N>& str)
    {
        *this = str;
    }

    /*!
         构建一个指定的不同长度的fsStr.
        @tparam N2 fsStr的长度.
        @param[in] str 指定的不同长度的fsStr.
    */
    template<u8 N2> fsStr(const fsStr<T, N2>& str)
    {
        *this = str;
    }

    /*!
        重载=运算符,使得fsStr值等于字符或宽字符.
        @param[in] ch 右边的字符或宽字符.
        @return 这个fsStr.
    */
    fsStr<T, N>& operator=(T ch)
    {
        m_str[0] = ch;
        m_str[1] = 0;

        m_length = 1;

        return *this;
    }

    /*!
        重载=运算符,使得fsStr值等于字符串.
        @param[in] ch 右边的字符串.
        @return 这个fsStr.
    */
    fsStr<T, N>& operator=(const T* str)
    {
        for (m_length = 0; m_length < N; m_length++)
        {
            T t = str[m_length];

            if (t == 0)
            {
                break;
            }

            m_str[m_length] = t;
        }

        m_str[m_length] = 0;

        return *this;
    }

    /*!
        重载=运算符,使得左边的fsStr值等于右边的fsStr值.
        @param[in] ch 右边的fsStr值.
        @return 这个fsStr.
    */
    fsStr<T, N>& operator=(const fsStr<T, N>& str)
    {
        m_length = str.m_length;

        fsMemHelper::memcpy(m_str, str.m_str, sizeof(T) * (m_length + 1));

        return *this;
    }

    /*!
        重载=运算符,使得左边的fsStr值等于右边不同长度的fsStr.
        @tparam N2 右边fsStr的长度.
        @param[in] str 不同长度的fsStr.
        @return 这个fsStr.
    */
    template<u8 N2> fsStr<T, N>& operator=(const fsStr<T, N2>& str)
    {
        m_length = (str.getLength() < N) ? str.getLength() : N;

        fsMemHelper::memcpy(m_str, str.getString(), sizeof(T) * (m_length + 1));

        m_str[m_length] = 0;

        return *this;
    }

    /*!
        重载==运算符,判断fsStr值是否等于右边字符或宽字符.
        @param[in] ch 右边的字符或宽字符.
        @return fsStr值是否等于右边字符或宽字符.
    */
    bool operator==(T ch) const
    {
        return (*this == fsStr<T, N>(ch));
    }

    /*!
        重载==运算符,判断fsStr值是否等于右边字符串.
        @param[in] str 右边的字符串.
        @return fsStr值是否等于右边字符串.
    */
    bool operator==(const T* str) const
    {
        for (s32 i = 0; ; i++)
        {
            T t = m_str[i];

            if (str[i] != t)
            {
                return false;
            }

            if (t == 0)
            {
                return true;
            }
        }

        return false;
    }

    /*!
        重载==运算符,判断左边的fsStr值是否等于右边不同长度的fsStr.
        @tparam N2 右边fsStr的长度.
        @param[in] str 不同长度的fsStr.
        @return 左边的fsStr值是否等于右边不同长度的fsStr.
    */
    template<u8 N2> bool operator==(const fsStr<T, N2>& str) const
    {
        if (m_length == str.getLength())
        {
            const T* str2 = str.getString();

            for (s32 i = 0; ; i++)
            {
                T t = m_str[i];

                if (str2[i] != t)
                {
                    return false;
                }

                if (t == 0)
                {
                    return true;
                }
            }
        }

        return false;
    }

    /*!
        重载==运算符,判断fsStr值是否不等于右边字符或宽字符.
        @param[in] ch 右边的字符或宽字符.
        @return fsStr值是否不等于右边字符或宽字符.
    */
    bool operator!=(T ch) const
    {
        return !(*this == ch);
    }

    /*!
        重载==运算符,判断fsStr值是否不等于右边字符串.
        @param[in] str 右边的字符串.
        @return fsStr值是否不等于右边字符串.
    */
    bool operator!=(const T* str) const
    {
        return !(*this == str);
    }

    /*!
        重载==运算符,判断左边的fsStr值是否不等于右边不同长度的fsStr.
        @tparam N2 右边fsStr的长度.
        @param[in] str 不同长度的fsStr.
        @return 左边的fsStr值是否不等于右边不同长度的fsStr.
    */
    template<u8 N2> bool operator!=(const fsStr<T, N2>& str) const
    {
        return !(*this == str);
    }

    /*!
       重载[]运算符,返回指定位置的字符或宽字符.
        @param[in] index 字符或宽字符的位置.
        @return 指定位置的字符或宽字符.
    */
    T& operator[](u8 index)
    {
        if (index >= m_length)
        {
            fsThrow(ExceptionOutOfRange);
        }

        return m_str[index];
    }

    /*!
        重载+运算符,使得fsStr值加上字符或宽字符.
        @param[in] ch 右边的字符或宽字符.
        @return 这个fsStr.
    */
    fsStr<T, N> operator+(T ch) const
    {
        fsStr<T, N> res = *this;

        res += ch;

        return res;
    }

    /*!
        重载+运算符,使得fsStr值加上字符串.
        @param[in] ch 右边的字符串.
        @return 这个fsStr.
    */
    fsStr<T, N> operator+(const T* str) const
    {
        fsStr<T, N> res = *this;

        res += str;

        return res;
    }

    /*!
        重载+运算符,使得左边的fsStr值加上右边不同长度的fsStr.
        @tparam N2 右边fsStr的长度.
        @param[in] str 不同长度的fsStr.
        @return 这个fsStr.
    */
    template<u8 N2> fsStr<T, N> operator+(const fsStr<T, N2>& str) const
    {
        fsStr<T, N> res = *this;

        res += str;

        return res;
    }

    /*!
        重载+=运算符,使得fsStr值加上右边字符或宽字符.
        @param[in] ch 右边的字符或宽字符.
    */
    void operator+=(T ch)
    {
        *this += fsStr<T, N>(ch);
    }

    /*!
        重载+运算符,使得fsStr值加上右边字符串.
        @param[in] ch 右边的字符串.
    */
    void operator+=(const T* str)
    {
        for ( ; m_length < N; m_length++)
        {
            if (*str == 0)
            {
                break;
            }

            m_str[m_length] = *str;

            str++;
        }

        m_str[m_length] = 0;
    }

    /*!
        重载+=运算符,使得左边的fsStr值加上右边不同长度的fsStr.
        @tparam N2 右边fsStr的长度.
        @param[in] str 不同长度的fsStr.
    */
    template<u8 N2> void operator+=(const fsStr<T, N2>& str)
    {
        const T* str2 = str.getString();

        for ( ; m_length < N; m_length++)
        {
            if (*str2 == 0)
            {
                break;
            }

            m_str[m_length] = *str2;

            str2++;
        }

        m_str[m_length] = 0;
    }

    /*!
        返回一个少于指定的值散列值.
        这个方法用于fsMap.
        @param[in] n 指定值.
        @return 散列值.
    */
    u32 operator%(u32 n) const
    {
        u32 acc = 0;

        for (s32 i = 0; i < m_length; i++)
        {
            acc += m_str[i];
        }

        return acc % n;
    }

    /*!
        返回字符串首地址.
        @return 字符串首地址.
    */
    const T* getString() const
    {
        return m_str;
    }

    /*!
        返回字符串长度.
        @return 字符串长度.
    */
    u8 getLength() const
    {
        return m_length;
    }

    /*!
        从指定的区域提取子字符串.
        @param[in] index 提取字符串的起始位置.
        @param[in] length 提取字符串的长度,如果没有指定默认为这个fsStr长度.
        @return 提取的子字符串.
    */
    fsStr<T, N> getSubStr(u8 index, u8 length = 0) const
    {
        fsStr<T, N> res;

        if (length == 0)
        {
            length = N;
        }

        for ( ; res.m_length < length; res.m_length++)
        {
            if (index >= m_length || index >= N)
            {
                break;
            }

            res.m_str[res.m_length] = m_str[index];

            index++;
        }

        res.m_str[res.m_length] = 0;

        return res;
    }

private:
    T m_str[N + 1];
    u8 m_length;
};


//! @cond
template<class T> class fsStr<T, 0>
{
private:
    fsStr() {}
};
//! @endcond
