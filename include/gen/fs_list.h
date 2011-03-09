/*
 * fs_list.h
 *
 *  Created on: 2011-3-9
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */

#ifndef FS_LIST_H_
#define FS_LIST_H_


/*!
    @ingroup fsGen
    一连串的元素储存于链表数据结构.
    @tparam T 存储链表的数据类型.
*/
template<class T> class fsList
{
public:
    //! @cond
    fsDefineException(ExceptionInvalidArgument);
    //! @endcond

    /*!
        An element of fsList.
    */
    class Item
    {
        friend class fsList<T>;

    public:
        //! @cond
        fsDefineException(ExceptionInvalidArgument);
        fsDefineException(ExceptionInvalidCall);
        fsDefineException(ExceptionNotInitialized);
        //! @endcond

        /*!
            Constructs a fsList::Item.
        */
        Item()
        {
            m_self = NULL;
            m_list = NULL;
            m_prev = m_next = NULL;
        }

        /*!
            Destructs this fsList::Item.
        */
        ~Item()
        {
            leave();
        }

        /*!
            Initializes this fsList::Item to have the specified value.
            This method can be called more than once.
            @param[in] self The pointer to a value.
        */
        void init(T* self)
        {
            if (!self)
            {
                fsThrow(ExceptionInvalidArgument);
            }

            m_self = self;
        }

        /*!
            Returns the value of this fsList::Item.
            @return The value of this fsList::Item.
        */
        T* getSelf() const
        {
            if (!m_self)
            {
                fsThrow(ExceptionNotInitialized);
            }

            return m_self;
        }

        /*!
            Returns the previous fsList::Item of this fsList::Item.
            If the fsList::Item has no previous fsList::Item, returns NULL.
            @return The previous fsList::Item of this fsList::Item.
        */
        Item* getPrevN() const
        {
            if (!hasList())
            {
                fsThrow(ExceptionInvalidCall);
            }

            return (m_prev != &m_list->m_start) ? m_prev : NULL;
        }

        /*!
            Returns the next fsList::Item of this fsList::Item.
            If the fsList::Item has no next fsList::Item, returns NULL.
            @return The next fsList::Item of this fsList::Item.
        */
        Item* getNextN() const
        {
            if (!hasList())
            {
                fsThrow(ExceptionInvalidCall);
            }

            return (m_next != &m_list->m_end) ? m_next : NULL;
        }

        /*!
            Returns whether this fsList::Item belongs to a fsList.
            @return Wheter this fsList::Item belongs to a fsList.
        */
        bool hasList() const
        {
            return m_list ? true : false;
        }

        /*!
            Returns the fsList which this fsList::Item belongs to.
            If ths fsList::Item does not belong to any fsList, returns NULL.
            @return The fsList which this fsList::Item belongs to.
        */
        fsList<T>* getListN() const
        {
            return m_list;
        }

        /*!
            Insertes this fsList::Item before the specified fsList::Item.
            @param[in] item A fsList::Item.
        */
        void joinBefore(Item* item)
        {
            if (!item || item == this || !item->hasList())
            {
                fsThrow(ExceptionInvalidArgument);
            }

            leave();

            m_list = item->m_list;

            m_prev = item->m_prev;
            m_next = item;

            m_prev->m_next = m_next->m_prev = this;

            m_list->m_item_num++;
        }

        /*!
            Insertes this fsList::Item after the specified fsList::Item.
            @param[in] item A fsList::Item.
        */
        void joinAfter(Item* item)
        {
            if (!item || item == this || !item->hasList())
            {
                fsThrow(ExceptionInvalidArgument);
            }

            leave();

            m_list = item->m_list;

            m_prev = item;
            m_next = item->m_next;

            m_prev->m_next = m_next->m_prev = this;

            m_list->m_item_num++;
        }

        /*!
            Removes this fsList::Item from the fsList which the fsList::Item belongs to.
        */
        void leave()
        {
            if (m_prev && m_next)
            {
                m_list->m_item_num--;

                m_prev->m_next = m_next;
                m_next->m_prev = m_prev;

                m_list = NULL;
                m_prev = m_next = NULL;
            }
        }

    private:
        Item(const Item&) {}
        void operator=(const Item&) {}

        T* m_self;
        fsList<T>* m_list;
        Item* m_prev;
        Item* m_next;
    };

    /*!
        Constructs a fsList with no item.
    */
    fsList()
    {
        m_item_num = 0;

        m_start.m_list = m_end.m_list = this;
        m_start.m_next = &m_end;
        m_end.m_prev = &m_start;
    }

    /*!
        Destructs this fsList.
    */
    ~fsList()
    {
        clear();
    }

    /*!
        Returns the number of fsList::Items in this fsList.
        @return The number of fsList::Items in this fsList.
    */
    u32 getItemNum() const
    {
        return m_item_num;
    }

    /*!
        Returns whether this fsList has any fsList::Item.
        @return Whether this fsList has any fsList::Item.
    */
    bool hasItem() const
    {
        return getFirstN() ? true : false;
    }

    /*!
        Returns the first fsList::Item in this fsList.
        If this fsList has no fsList::Item, returns NULL.
        @return The first fsList::Item in this fsList.
    */
    Item* getFirstN() const
    {
        return (m_start.m_next != &m_end) ? m_start.m_next : NULL;
    }

    /*!
        Returns the last fsList::Item in this fsList.
        If this list has no fsList::Item, returns NULL.
        @return The last fsList::Item in this fsList.
    */
    Item* getLastN() const
    {
        return (m_end.m_prev != &m_start) ? m_end.m_prev : NULL;
    }

    /*!
        Adds the specified fsList::Item to this fsList as the first fsList::Item.
        @param[in] item A fsList::Item.
    */
    void addFirst(Item* item)
    {
        if (!item)
        {
            fsThrow(ExceptionInvalidArgument);
        }

        item->joinAfter(&m_start);
    }

    /*!
        Adds the specified fsList::Item to this fsList as the last fsList::Item.
        @param[in] item A fsList::Item.
    */
    void addLast(Item* item)
    {
        if (!item)
        {
            fsThrow(ExceptionInvalidArgument);
        }

        item->joinBefore(&m_end);
    }

    /*!
        Removes all fsList::Items from this fsList.
    */
    void clear()
    {
        while (hasItem())
        {
            getFirstN()->leave();
        }
    }

private:
    fsList(const fsList<T>&) {}
    void operator=(const fsList<T>&) {}

    Item m_start;
    Item m_end;
    u32 m_item_num;
};


#endif /* FS_LIST_H_ */
