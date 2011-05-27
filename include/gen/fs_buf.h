/*
 * fs_buf.h
 *
 *  Created on: 2011-5-27
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */

#ifndef FS_BUF_H_
#define FS_BUF_H_


/*!
    @ingroup fsGen
    A memory pool for the specified type.
    @tparam T The type which is managed by the memory pool.
*/
template<class T> class fsBuf
{
public:
    //! @cond
    fsDefineException(ExceptionInvalidArgument);
    fsDefineException(ExceptionNotInitialized);
    fsDefineException(ExceptionOutOfBuffer);
    //! @endcond

    /*!
        Constructs and initialize a fsBuf whose size is zero.
    */
    fsBuf()
    {
        m_elem = NULL;
        m_state = NULL;
        m_index_tbl = NULL;
        m_size = m_used_size = 0;
    }

    /*!
        Destructs this fsBuf.
    */
    ~fsBuf()
    {
        if (m_elem)
        {
            for (s32 i = 0; i < m_size; i++)
            {
                if (m_state[i] == STATE_USED)
                {
                    m_elem[i].~T();
                }
            }

            fsFree(m_elem);
            fsDeleteArray(m_state, u8);
            fsDeleteArray(m_index_tbl, u16);
        }
    }

    /*!
        Initializes this fsBuf with the specified size.
        @param[in] size The size of this fsBuf.
    */
    void init(u16 size)
    {
        if (size == 0)
        {
            fsThrow(ExceptionInvalidArgument);
        }

        this->~fsBuf();

        m_size = size;
        m_used_size = 0;

        m_elem = static_cast<T*>(fsMalloc(sizeof(T) * m_size));
        fsNewArray(m_state, u8, m_size);
        fsNewArray(m_index_tbl, u16, m_size);

        for (s32 i = 0; i < m_size; i++)
        {
            m_state[i] = STATE_FREE;
            m_index_tbl[i] = i;
        }
    }

    /*!
        Returns the size of this fsBuf.
        @return The size of this fsBuf.
    */
    u16 getSize() const
    {
        return m_size;
    }

    /*!
        Returns the used size of this fsBuf.
        @return The used size of this fsBuf.
    */
    u16 getUsedSize() const
    {
        return m_used_size;
    }

    /*!
        Returns the available size of this fsBuf.
        @return The available size of this fsBuf.
    */
    u16 getFreeSize() const
    {
        return m_size - m_used_size;
    }

    /*!
        Returns the first value in this fsBuf.
        If this fsBuf has no value, returns NULL.
        @return The first value in this fsBuf.
    */
    T* getFirstN() const
    {
        if (!m_elem)
        {
            fsThrow(ExceptionNotInitialized);
        }

        for (s32 i = 0; i < m_size; i++)
        {
            if (m_state[i] == STATE_USED)
            {
                return &m_elem[i];
            }
        }

        return NULL;
    }

    /*!
        Returns the last value in this fsBuf.
        If this fsBuf has no value, returns NULL.
        @return The last value in this fsBuf.
    */
    T* getLastN() const
    {
        if (!m_elem)
        {
            fsThrow(ExceptionNotInitialized);
        }

        for (s32 i = m_size - 1; i >= 0; i--)
        {
            if (m_state[i] == STATE_USED)
            {
                return &m_elem[i];
            }
        }

        return NULL;
    }

    /*!
        Returns the previous value of the specified value.
        If the specified value has no previous value, returns NULL.
        @param[in] elem A value in this fsBuf.
        @return The previous value of the specified value.
    */
    T* getPrevN(const T* elem) const
    {
        if (!m_elem)
        {
            fsThrow(ExceptionNotInitialized);
        }

        if (elem < m_elem || elem >= m_elem + m_size)
        {
            fsThrow(ExceptionInvalidArgument);
        }

        u16 index = static_cast<u16>(elem - m_elem);

        if (m_state[index] == STATE_FREE)
        {
            fsThrow(ExceptionInvalidArgument);
        }

        if (index > 0)
        {
            for (s32 i = index - 1; i >= 0; i--)
            {
                if (m_state[i] == STATE_USED)
                {
                    return &m_elem[i];
                }
            }
        }

        return NULL;
    }

    /*!
        Returns the next value of the specified value.
        If the specified value has no next value, returns NULL.
        @param[in] elem A value in this fsBuf.
        @return The next value of the specified value.
    */
    T* getNextN(const T* elem) const
    {
        if (!m_elem)
        {
            fsThrow(ExceptionNotInitialized);
        }

        if (elem < m_elem || elem >= m_elem + m_size)
        {
            fsThrow(ExceptionInvalidArgument);
        }

        u16 index = static_cast<u16>(elem - m_elem);

        if (m_state[index] == STATE_FREE)
        {
            fsThrow(ExceptionInvalidArgument);
        }

        if (index + 1 < m_size)
        {
            for (s32 i = index + 1; i < m_size; i++)
            {
                if (m_state[i] == STATE_USED)
                {
                    return &m_elem[i];
                }
            }
        }

        return NULL;
    }

    /*!
        Allocates a value from this fsBuf.
        @return An allocated value.
    */
    T* newElement()
    {
        if (!m_elem)
        {
            fsThrow(ExceptionNotInitialized);
        }

        if (m_used_size >= m_size)
        {
            fsThrow(ExceptionOutOfBuffer);
        }

        u16 index = m_index_tbl[m_used_size];

        m_state[index] = STATE_USED;

        m_used_size++;

        T* elem = &m_elem[index];
        new(elem, NULL) T;

        return elem;
    }

    /*!
        Deletes the specified value in this fsBuf.
        @param[in] elem A value in this fsBuf.
    */
    void deleteElement(T* elem)
    {
        if (!m_elem)
        {
            fsThrow(ExceptionNotInitialized);
        }

        if (elem < m_elem || elem >= m_elem + m_size)
        {
            fsThrow(ExceptionInvalidArgument);
        }

        u16 index = static_cast<u16>(elem - m_elem);

        if (m_state[index] == STATE_FREE)
        {
            fsThrow(ExceptionInvalidArgument);
        }

        elem->~T();

        m_used_size--;

        m_index_tbl[m_used_size] = index;
        m_state[index] = STATE_FREE;
    }

    /*!
        Deletes all values in this fsBuf.
    */
    void clear()
    {
        if (!m_elem)
        {
            fsThrow(ExceptionNotInitialized);
        }

        m_used_size = 0;

        for (s32 i = 0; i < m_size; i++)
        {
            if (m_state[i] == STATE_USED)
            {
                m_elem[i].~T();
                m_state[i] = STATE_FREE;
            }

            m_index_tbl[i] = i;
        }
    }

private:
    enum ElemState
    {
        STATE_FREE, //
        STATE_USED
    };

    fsBuf(const fsBuf<T>&) {}
    void operator=(const fsBuf<T>&) {}

    T* m_elem;
    u8* m_state;
    u16* m_index_tbl;
    u16 m_size;
    u16 m_used_size;
};

#endif /* FS_BUF_H_ */
