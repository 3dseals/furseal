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
        Constructs and initializes a fsStr whose length is zero.
    */
    fsStr()
    {
        m_str[0] = 0;
        m_length = 0;
    }

    /*!
        Constructs and initializes a fsStr from the specified character.
        @param[in] ch A character.
    */
    fsStr(T ch)
    {
        *this = ch;
    }

    /*!
        Constructs and initializes a fsStr from the specified string.
        @param[in] str A string. NULL is allowed.
    */
    fsStr(const T* str)
    {
        *this = str;
    }

    /*!
        Constructs and initializes a fsStr from the specified fsStr.
        @param[in] str A fsStr.
    */
    fsStr(const fsStr<T, N>& str)
    {
        *this = str;
    }

    /*!
        Constructs and initializes a fsStr from the specified fsStr with different length.
        @tparam N2 The length of a fsStr.
        @param[in] str A fsStr with different length.
    */
    template<u8 N2> fsStr(const fsStr<T, N2>& str)
    {
        *this = str;
    }

    /*!
        Assigns the right hand side character to this fsStr.
        @param[in] ch The right hand side character.
        @return A reference to this fsStr.
    */
    fsStr<T, N>& operator=(T ch)
    {
        m_str[0] = ch;
        m_str[1] = 0;

        m_length = 1;

        return *this;
    }

    /*!
        Assigns the right hand side string to this fsStr.
        @param[in] str The right hand side string.
        @return A reference to this fsStr.
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
        Assigns the right hand side fsStr to this fsStr.
        @param[in] str The right hand side fsStr.
        @return A reference to this fsStr.
    */
    fsStr<T, N>& operator=(const fsStr<T, N>& str)
    {
        m_length = str.m_length;

        fsMemHelper::memcpy(m_str, str.m_str, sizeof(T) * (m_length + 1));

        return *this;
    }

    /*!
        Assigns the right hand side fsStr with different length to this fsStr.
        @tparam N2 The length of the right hand side fsStr.
        @param[in] str The right hand side fsStr with different length.
        @return A referenct to this fsStr.
    */
    template<u8 N2> fsStr<T, N>& operator=(const fsStr<T, N2>& str)
    {
        m_length = (str.getLength() < N) ? str.getLength() : N;

        fsMemHelper::memcpy(m_str, str.getString(), sizeof(T) * (m_length + 1));

        m_str[m_length] = 0;

        return *this;
    }

    /*!
        Returns whether this fsStr is equal to the right hand side character.
        @param[in] ch The right hand side character.
        @return Whether this fsStr is equal to the right hand side character.
    */
    bool operator==(T ch) const
    {
        return (*this == fsStr<T, N>(ch));
    }

    /*!
        Returns whether this fsStr is equal to the right hand side string.
        @param[in] str The right hand side string.
        @return Whether this fsStr is equal to the right hand side string.
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
        Returns whether this fsStr is equal to the right hand side fsStr with different length.
        @tparam N2 The length of the right hand side fsStr.
        @param[in] str The right hand side fsStr with defferent length.
        @return Whether this fsStr is equal to the right hand side fsStr.
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
        Returns whether this fsStr is unequal to the right hand side character.
        @param[in] ch The right hand side character.
        @return Whether this fsStr is unequal to the right hand side character.
    */
    bool operator!=(T ch) const
    {
        return !(*this == ch);
    }

    /*!
        Returns whether this fsStr is unequal to the right hand side string.
        @param[in] str The right hand side string.
        @return Whether this fsStr is unequal to the right hand side string.
    */
    bool operator!=(const T* str) const
    {
        return !(*this == str);
    }

    /*!
        Returns whether this fsStr is unequal to the right hand side fsStr with different length.
        @tparam N2 The length of the right hand side fsStr.
        @param[in] str The right hand side fsStr with defferent length.
        @return Whether this fsStr is unequal to the right hand side fsStr.
    */
    template<u8 N2> bool operator!=(const fsStr<T, N2>& str) const
    {
        return !(*this == str);
    }

    /*!
        Returns a reference to the character of this fsStr at the position specified by the parameter index.
        @param[in] index The index of the position of the element to be referenced.
        @return A reference to the character of this fsStr at the position specified by the parameter index.
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
        Returns a fsStr which is the concatenation of this fsStr and the right hand side character.
        @param[in] ch The right hand side character.
        @return A fsStr which is the concatenation of this fsStr and the right hand side character.
    */
    fsStr<T, N> operator+(T ch) const
    {
        fsStr<T, N> res = *this;

        res += ch;

        return res;
    }

    /*!
        Returns a fsStr which is the concatenation of this fsStr and the right hand side string.
        @param[in] str The right hand side string.
        @return A fsStr which is the concatenation of this fsStr and the right hand side string.
    */
    fsStr<T, N> operator+(const T* str) const
    {
        fsStr<T, N> res = *this;

        res += str;

        return res;
    }

    /*!
        Returns a fsStr which is the concatenation of this fsStr and the right hand side fsStr width differenct length.
        @tparam N2 The length of the right hand side fsStr.
        @param[in] str The right hand side fsStr with different length.
        @return A fsStr which is the concatenation of this fsStr and the right hand side fsStr width differenct length.
    */
    template<u8 N2> fsStr<T, N> operator+(const fsStr<T, N2>& str) const
    {
        fsStr<T, N> res = *this;

        res += str;

        return res;
    }

    /*!
        Concatenates the right hand side character to the end of this fsStr.
        @param[in] ch The right hand side character.
    */
    void operator+=(T ch)
    {
        *this += fsStr<T, N>(ch);
    }

    /*!
        Concatenates the right hand side string to the end of this fsStr.
        @param[in] str The right hand side string.
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
        Concatenates the right hand side fsStr with different length to the end of this fsStr.
        @tparam N2 The length of the right hand side fsStr.
        @param[in] str The right hand side fsStr with different length.
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
        Returns a hash value which is less than the specified value.
        This method is used when fsStr is used as the key type of fsMap.
        @param[in] n A value.
        @return A hash value.
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
        Returns the pointer to the c-string of this fsStr.
        @return The pointer to the c-string of this fsStr.
    */
    const T* getString() const
    {
        return m_str;
    }

    /*!
        Returns the length of this fsStr.
        @return The length of this fsStr.
    */
    u8 getLength() const
    {
        return m_length;
    }

    /*!
        Returns the extracted fsStr from this fsStr in the specified range.
        @param[in] index The index of the position of the element to be extracted.
        @param[in] length The length of the extracted fsStr. If not specified, the length of this fsStr is used.
        @return The extracted fsStr from this fsStr in the specified range.
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
