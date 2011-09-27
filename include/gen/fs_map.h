/*
 * fs_map.h
 *
 *  Created on: 2011-3-9
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */

#ifndef FS_MAP_H_
#define FS_MAP_H_


/*!
    @ingroup fsGen
    一个有序的数据结构,包含关键字和对应值.
    @tparam K 关键字类型,通过关键值可以找到对应值.
    @tparam D 对应值的类型,与关键字一一对应.
*/
template<class K, class D> class fsMap
{
public:
    //! @cond
    fsDefineException(ExceptionInvalidArgument);
    fsDefineException(ExceptionNotFound);
    fsDefineException(ExceptionNotInitialized);
    fsDefineException(ExceptionSameKeyExists);
    //! @endcond

    /*!
        Constructs a fsMap.
    */
    fsMap()
    {
        m_hash_list = NULL;
        m_last_item1 = m_last_item2 = m_last_item3 = NULL;
        m_hash_size = 0;
        m_data_num = 0;
    }

    /*!
        Destructs this fsMap.
    */
    ~fsMap()
    {
        clear();

        if (m_hash_list)
        {
            fsDeleteArray(m_hash_list, fsList<MapItem>);
        }
    }

    /*!
        Initializes this fsMap whose hash list has the specified size.
        The size of the hash list must be more than zero.
        @param[in] hash_size The size of the hash list of this fsMap.
    */
    void init(u16 hash_size)
    {
        if (hash_size == 0)
        {
            fsThrow(ExceptionInvalidArgument);
        }

        clear();

        if (m_hash_list)
        {
            fsDeleteArray(m_hash_list, fsList<MapItem>);
        }

        m_hash_size = hash_size;
        fsNewArray(m_hash_list, fsList<MapItem>, m_hash_size);
    }

    /*!
        Returns the size of the hash list of this fsMap.
        @return The size of the hash list of this fsMap.
    */
    u16 getHashSize() const
    {
        return m_hash_size;
    }

    /*!
        Returns the number of the key/value pairs in this fsMap.
        @return The number of the key/value pairs in this fsMap.
    */
    u16 getDataNum() const
    {
        return m_data_num;
    }

    /*!
        Returns the value which is associated with the specified key.
        @param[in] key A key in this fsMap.
        @return The value which is associated with the specified key.
    */
    D* get(K key)
    {
        D* data = getN(key);

        if (!data)
        {
            fsThrow(ExceptionNotFound);
        }

        return data;
    }

    /*!
        Returns the value which is associated with the specified key.
        If the specified key isn't in this fsMap, returns NULL.
        @param[in] key A key in this fsMap.
        @return The value which is associated with the specified key.
    */
    D* getN(K key)
    {
        if (!m_hash_list)
        {
            fsThrow(ExceptionNotInitialized);
        }

        if (m_last_item1 && m_last_item1->key == key)
        {
            return &m_last_item1->data;
        }
        else if (m_last_item2 && m_last_item2->key == key)
        {
            MapItem* item = m_last_item1;
            m_last_item1 = m_last_item2;
            m_last_item2 = item;

            return &m_last_item1->data;
        }
        else if (m_last_item3 && m_last_item3->key == key)
        {
            MapItem* item = m_last_item1;
            m_last_item1 = m_last_item3;
            m_last_item3 = m_last_item2;
            m_last_item2 = item;

            return &m_last_item1->data;
        }
        else
        {
            s32 index = key % m_hash_size;
            fsList<MapItem>* hash_list = &m_hash_list[(index < 0) ? -index : index];

            for (typename fsList<MapItem>::Item* item = hash_list->getFirstN(); item; item = item->getNextN())
            {
                if (item->getSelf()->key == key)
                {
                    m_last_item3 = m_last_item2;
                    m_last_item2 = m_last_item1;
                    m_last_item1 = item->getSelf();

                    hash_list->addFirst(item);

                    return &m_last_item1->data;
                }
            }

            return NULL;
        }
    }

    /*!
        Adds a key/value pair to this fsMap.
        @param[in] key A key.
        @param[in] data A value.
    */
    void add(K key, const D& data)
    {
        if (!m_hash_list)
        {
            fsThrow(ExceptionNotInitialized);
        }

        if (getN(key))
        {
            fsThrow(ExceptionSameKeyExists);
        }

        MapItem* new_item = fsNew(MapItem);

        new_item->order_item.init(new_item);
        new_item->hash_item.init(new_item);
        new_item->key = key;
        new_item->data = data;

        m_order_list.addLast(&new_item->order_item);

        s32 index = key % m_hash_size;
        m_hash_list[(index < 0) ? -index : index].addLast(&new_item->hash_item);

        m_data_num++;
    }

    /*!
        Removes the specified key/value pair from this fsMap.
        @param[in] key A key in this fsMap.
    */
    void remove(K key)
    {
        if (!m_hash_list)
        {
            fsThrow(ExceptionNotInitialized);
        }

        if (m_last_item1 && m_last_item1->key == key)
        {
            fsDelete(m_last_item1, MapItem);
            m_data_num--;

            m_last_item1 = m_last_item2;
            m_last_item2 = m_last_item3;
            m_last_item3 = NULL;
        }
        else if (m_last_item2 && m_last_item2->key == key)
        {
            fsDelete(m_last_item2, MapItem);
            m_data_num--;

            m_last_item2 = m_last_item3;
            m_last_item3 = NULL;
        }
        else if (m_last_item3 && m_last_item3->key == key)
        {
            fsDelete(m_last_item3, MapItem);
            m_data_num--;

            m_last_item3 = NULL;
        }
        else
        {
            s32 index = key % m_hash_size;
            fsList<MapItem>* hash_list = &m_hash_list[(index < 0) ? -index : index];

            for (typename fsList<MapItem>::Item* item = hash_list->getFirstN(); item; item = item->getNextN())
            {
                if (item->getSelf()->key == key)
                {
                    fsDelete(item->getSelf(), MapItem);
                    m_data_num--;

                    return;
                }
            }

            fsThrow(ExceptionNotFound);
        }
    }

    /*!
        Removes all key/value pairs from this fsMap.
    */
    void clear()
    {
        m_order_list.clear();

        if (m_hash_list)
        {
            for (s32 i = 0; i < m_hash_size; i++)
            {
                fsList<MapItem>* hash_list = &m_hash_list[i];

                while (hash_list->hasItem())
                {
                    fsDelete(hash_list->getFirstN()->getSelf(), MapItem);
                }
            }

            m_last_item1 = m_last_item2 = m_last_item3 = NULL;
            m_data_num = 0;
        }
    }

    /*!
        Returns the first key in this fsMap. If the fsMap has no key/value pair, returns NULL.
        @return The first key in this fsMap.
    */
    const K* getFirstKeyN() const
    {
        if (!m_hash_list)
        {
            fsThrow(ExceptionNotInitialized);
        }

        return m_order_list.hasItem() ? &m_order_list.getFirstN()->getSelf()->key : NULL;
    }

    /*!
        Returns the last key in this fsMap. If the fsMap has no key/value pair, returns NULL.
        @return The last key in this fsMap.
    */
    const K* getLastKeyN() const
    {
        if (!m_hash_list)
        {
            fsThrow(ExceptionNotInitialized);
        }

        return m_order_list.hasItem() ? &m_order_list.getLastN()->getSelf()->key : NULL;
    }

    /*!
        Returns the previous key of the specified key.
        If the specified key has no previous key, returns NULL.
        @param[in] key A key in this fsMap.
        @return The previous key of the specified key.
    */
    const K* getPrevKeyN(K key)
    {
        fsTry
        {
            typename fsList<MapItem>::Item* prev = reinterpret_cast<MapItem*>(get(key))->order_item.getPrevN();

            return prev ? &prev->getSelf()->key : NULL;
        }
        fsCatch(ExceptionNotFound)
        {
            fsThrow(ExceptionInvalidArgument);
        }

        return NULL;
    }

    /*!
        Returns the next key of the specified key.
        If the specified key has no next key, returns NULL.
        @param[in] key A key in this fsMap.
        @return The next key of the specified key.
    */
    const K* getNextKeyN(K key)
    {
        fsTry
        {
            typename fsList<MapItem>::Item* next = reinterpret_cast<MapItem*>(get(key))->order_item.getNextN();

            return next ? &next->getSelf()->key : NULL;
        }
        fsCatch(ExceptionNotFound)
        {
            fsThrow(ExceptionInvalidArgument);
        }

        return NULL;
    }

    /*!
        Move the specified key/value pair to the first in this fsMap.
        @param[in] key A key in this fsMap.
    */
    void moveFirst(K key)
    {
        fsTry
        {
            typename fsList<MapItem>::Item* item = &reinterpret_cast<MapItem*>(get(key))->order_item;

            m_order_list.addFirst(item);
        }
        fsCatch(ExceptionNotFound)
        {
            fsThrow(ExceptionInvalidArgument);
        }
    }

    /*!
        Move the specified key/value pair to the last in this fsMap.
        @param[in] key A key in this fsMap.
    */
    void moveLast(K key)
    {
        fsTry
        {
            typename fsList<MapItem>::Item* item = &reinterpret_cast<MapItem*>(get(key))->order_item;

            m_order_list.addLast(item);
        }
        fsCatch(ExceptionNotFound)
        {
            fsThrow(ExceptionInvalidArgument);
        }
    }

    /*!
        Move the specified key/value pair before the another pair.
        @param[in] key A key in this fsMap.
        @param[in] next_key An another key in this fsMap.
    */
    void moveBefore(K key, K next_key)
    {
        fsTry
        {
            typename fsList<MapItem>::Item* item = &reinterpret_cast<MapItem*>(get(key))->order_item;
            typename fsList<MapItem>::Item* next = &reinterpret_cast<MapItem*>(get(next_key))->order_item;

            item->joinBefore(next);
        }
        fsCatch(ExceptionNotFound)
        {
            fsThrow(ExceptionInvalidArgument);
        }
    }

    /*!
        Move the specified key/value pair after the another pair.
        @param[in] key A key in this fsMap.
        @param[in] prev_key An another key in this fsMap.
    */
    void moveAfter(K key, K prev_key)
    {
        fsTry
        {
            typename fsList<MapItem>::Item* item = &reinterpret_cast<MapItem*>(get(key))->order_item;
            typename fsList<MapItem>::Item* prev = &reinterpret_cast<MapItem*>(get(prev_key))->order_item;

            item->joinAfter(prev);
        }
        fsCatch(ExceptionNotFound)
        {
            fsThrow(ExceptionInvalidArgument);
        }
    }

private:
    struct MapItem
    {
        D data; // This data member must be at the top of this structure.
        typename fsList<MapItem>::Item order_item;
        typename fsList<MapItem>::Item hash_item;
        K key;
    };

    fsMap(const fsMap<K, D>&) {}
    void operator=(const fsMap<K, D>&) {}

    fsList<MapItem> m_order_list;
    fsList<MapItem>* m_hash_list;
    MapItem* m_last_item1;
    MapItem* m_last_item2;
    MapItem* m_last_item3;
    u16 m_hash_size;
    u16 m_data_num;
};


/*!
    @ingroup fsGen
    一个有序的数据结构,包含关键字和对应值.
    @tparam K 关键字类型,通过关键值可以找到对应值.
    @tparam D 对应值的类型,与关键字一一对应.
*/
template<class K, class D> class fsMultiMap
{
public:
	//! @cond
	fsDefineException(ExceptionInvalidArgument);
	fsDefineException(ExceptionNotFound);
	fsDefineException(ExceptionNotInitialized);
	//! @endcond

	/*!
	Constructs a fsMultiMap.
	*/
	fsMultiMap()
	{
		m_hash_list = NULL;
		m_last_item1 = m_last_item2 = m_last_item3 = NULL;
		m_hash_size = 0;
		m_data_num = 0;
	}

	/*!
	Destructs this fsMap.
	*/
	~fsMultiMap()
	{
		clear();

		if (m_hash_list)
		{
			fsDeleteArray(m_hash_list, fsList<MultiMapItem>);
		}
	}

	/*!
	Initializes this fsMap whose hash list has the specified size.
	The size of the hash list must be more than zero.
	@param[in] hash_size The size of the hash list of this fsMap.
	*/
	void init(u16 hash_size)
	{
		if (hash_size == 0)
		{
			fsThrow(ExceptionInvalidArgument);
		}

		clear();

		if (m_hash_list)
		{
			fsDeleteArray(m_hash_list, fsList<MultiMapItem>);
		}

		m_hash_size = hash_size;
		fsNewArray(m_hash_list, fsList<MultiMapItem>, m_hash_size);
	}

	/*!
	Returns the size of the hash list of this fsMap.
	@return The size of the hash list of this fsMap.
	*/
	u16 getHashSize() const
	{
		return m_hash_size;
	}

	/*!
	Returns the number of the key/value pairs in this fsMap.
	@return The number of the key/value pairs in this fsMap.
	*/
	u16 getDataNum() const
	{
		return m_data_num;
	}

	/*!
	Returns the value which is associated with the specified key.
	@param[in] key A key in this fsMap.
	@return The value which is associated with the specified key.
	*/
	fsList<D>* get(K key)
	{
		fsList<D>* data = getN(key);

		if (!data)
		{
			fsThrow(ExceptionNotFound);
		}

		return data;
	}

	/*!
	Returns the value which is associated with the specified key.
	If the specified key isn't in this fsMap, returns NULL.
	@param[in] key A key in this fsMap.
	@return The value which is associated with the specified key.
	*/
	fsList<D>* getN(K key)
	{
		if (!m_hash_list)
		{
			fsThrow(ExceptionNotInitialized);
		}

		if (m_last_item1 && m_last_item1->key == key)
		{
			return &m_last_item1->data;
		}
		else if (m_last_item2 && m_last_item2->key == key)
		{
			MultiMapItem* item = m_last_item1;
			m_last_item1 = m_last_item2;
			m_last_item2 = item;

			return &m_last_item1->data;
		}
		else if (m_last_item3 && m_last_item3->key == key)
		{
			MultiMapItem* item = m_last_item1;
			m_last_item1 = m_last_item3;
			m_last_item3 = m_last_item2;
			m_last_item2 = item;

			return &m_last_item1->data;
		}
		else
		{
			s32 index = key % m_hash_size;
			fsList<MultiMapItem>* hash_list = &m_hash_list[(index < 0) ? -index : index];

			for (typename fsList<MultiMapItem>::Item* item = hash_list->getFirstN(); item; item = item->getNextN())
			{
				if (item->getSelf()->key == key)
				{
					m_last_item3 = m_last_item2;
					m_last_item2 = m_last_item1;
					m_last_item1 = item->getSelf();

					hash_list->addFirst(item);

					return &m_last_item1->data;
				}
			}

			return NULL;
		}
	}

	/*!
	Adds a key/value pair to this fsMap.
	@param[in] key A key.
	@param[in] data A value.
	*/
	void add(K key, const D& data)
	{
		if (!m_hash_list)
		{
			fsThrow(ExceptionNotInitialized);
		}

		if (getN(key))
		{
			MultiMapItem* new_item = fsNew(MultiMapItem);

			new_item->self = data;
			new_item->list_item.init(&new_item->self);
			new_item->order_item.init(new_item);
			new_item->hash_item.init(new_item);
			new_item->key = key;
			get(key)->addLast(&new_item->list_item);

			m_order_list.addLast(&new_item->order_item);

			s32 index = key % m_hash_size;
			m_hash_list[(index < 0) ? -index : index].addLast(&new_item->hash_item);
		}
		else
		{
			MultiMapItem* new_item = fsNew(MultiMapItem);

			new_item->self = data;
			new_item->list_item.init(&new_item->self);
			new_item->order_item.init(new_item);
			new_item->hash_item.init(new_item);
			new_item->key = key;
			new_item->data.addLast(&new_item->list_item);

			m_order_list.addLast(&new_item->order_item);

			s32 index = key % m_hash_size;
			m_hash_list[(index < 0) ? -index : index].addLast(&new_item->hash_item);
		}

		m_data_num++;
	}

	/*!
	Removes the specified key/value pair from this fsMap.
	@param[in] key A key in this fsMap.
	*/
	void remove(K key)
	{
		if (!m_hash_list)
		{
			fsThrow(ExceptionNotInitialized);
		}

		if (m_last_item1 && m_last_item1->key == key)
		{
			fsDelete(m_last_item1, MultiMapItem);
			m_data_num--;

			m_last_item1 = m_last_item2;
			m_last_item2 = m_last_item3;
			m_last_item3 = NULL;
		}
		else if (m_last_item2 && m_last_item2->key == key)
		{
			fsDelete(m_last_item2, MultiMapItem);
			m_data_num--;

			m_last_item2 = m_last_item3;
			m_last_item3 = NULL;
		}
		else if (m_last_item3 && m_last_item3->key == key)
		{
			fsDelete(m_last_item3, MultiMapItem);
			m_data_num--;

			m_last_item3 = NULL;
		}
		else
		{
			s32 index = key % m_hash_size;
			fsList<MultiMapItem>* hash_list = &m_hash_list[(index < 0) ? -index : index];

			for (typename fsList<MultiMapItem>::Item* item = hash_list->getFirstN(); item; item = item->getNextN())
			{
				if (item->getSelf()->key == key)
				{
					fsDelete(item->getSelf(), MultiMapItem);
					m_data_num--;

					return;
				}
			}

			fsThrow(ExceptionNotFound);
		}
	}

	/*!
	Removes all key/value pairs from this fsMap.
	*/
	void clear()
	{
		m_order_list.clear();

		if (m_hash_list)
		{
			for (s32 i = 0; i < m_hash_size; i++)
			{
				fsList<MultiMapItem>* hash_list = &m_hash_list[i];

				while (hash_list->hasItem())
				{
					fsDelete(hash_list->getFirstN()->getSelf(), MultiMapItem);
				}
			}

			m_last_item1 = m_last_item2 = m_last_item3 = NULL;
			m_data_num = 0;
		}
	}

	/*!
	Returns the first key in this fsMap. If the fsMap has no key/value pair, returns NULL.
	@return The first key in this fsMap.
	*/
	const K* getFirstKeyN() const
	{
		if (!m_hash_list)
		{
			fsThrow(ExceptionNotInitialized);
		}

		return m_order_list.hasItem() ? &m_order_list.getFirstN()->getSelf()->key : NULL;
	}

	/*!
	Returns the last key in this fsMap. If the fsMap has no key/value pair, returns NULL.
	@return The last key in this fsMap.
	*/
	const K* getLastKeyN() const
	{
		if (!m_hash_list)
		{
			fsThrow(ExceptionNotInitialized);
		}

		return m_order_list.hasItem() ? &m_order_list.getLastN()->getSelf()->key : NULL;
	}

	/*!
	Returns the previous key of the specified key.
	If the specified key has no previous key, returns NULL.
	@param[in] key A key in this fsMap.
	@return The previous key of the specified key.
	*/
	const K* getPrevKeyN(K key)
	{
		fsTry
		{
			typename fsList<MultiMapItem>::Item* prev = reinterpret_cast<MultiMapItem*>(get(key))->order_item.getPrevN();

			return prev ? &prev->getSelf()->key : NULL;
		}
		fsCatch(ExceptionNotFound)
		{
			fsThrow(ExceptionInvalidArgument);
		}

		return NULL;
	}

	/*!
	Returns the next key of the specified key.
	If the specified key has no next key, returns NULL.
	@param[in] key A key in this fsMap.
	@return The next key of the specified key.
	*/
	const K* getNextKeyN(K key)
	{
		fsTry
		{
			typename fsList<MultiMapItem>::Item* next = reinterpret_cast<MultiMapItem*>(get(key))->order_item.getNextN();

			return next ? &next->getSelf()->key : NULL;
		}
		fsCatch(ExceptionNotFound)
		{
			fsThrow(ExceptionInvalidArgument);
		}

		return NULL;
	}

	/*!
	Move the specified key/value pair to the first in this fsMap.
	@param[in] key A key in this fsMap.
	*/
	void moveFirst(K key)
	{
		fsTry
		{
			typename fsList<MultiMapItem>::Item* item = &reinterpret_cast<MultiMapItem*>(get(key))->order_item;

			m_order_list.addFirst(item);
		}
		fsCatch(ExceptionNotFound)
		{
			fsThrow(ExceptionInvalidArgument);
		}
	}

	/*!
	Move the specified key/value pair to the last in this fsMap.
	@param[in] key A key in this fsMap.
	*/
	void moveLast(K key)
	{
		fsTry
		{
			typename fsList<MultiMapItem>::Item* item = &reinterpret_cast<MultiMapItem*>(get(key))->order_item;

			m_order_list.addLast(item);
		}
		fsCatch(ExceptionNotFound)
		{
			fsThrow(ExceptionInvalidArgument);
		}
	}

	/*!
	Move the specified key/value pair before the another pair.
	@param[in] key A key in this fsMap.
	@param[in] next_key An another key in this fsMap.
	*/
	void moveBefore(K key, K next_key)
	{
		fsTry
		{
			typename fsList<MultiMapItem>::Item* item = &reinterpret_cast<MultiMapItem*>(get(key))->order_item;
			typename fsList<MultiMapItem>::Item* next = &reinterpret_cast<MultiMapItem*>(get(next_key))->order_item;

			item->joinBefore(next);
		}
		fsCatch(ExceptionNotFound)
		{
			fsThrow(ExceptionInvalidArgument);
		}
	}

	/*!
	Move the specified key/value pair after the another pair.
	@param[in] key A key in this fsMap.
	@param[in] prev_key An another key in this fsMap.
	*/
	void moveAfter(K key, K prev_key)
	{
		fsTry
		{
			typename fsList<MultiMapItem>::Item* item = &reinterpret_cast<MultiMapItem*>(get(key))->order_item;
			typename fsList<MultiMapItem>::Item* prev = &reinterpret_cast<MultiMapItem*>(get(prev_key))->order_item;

			item->joinAfter(prev);
		}
		fsCatch(ExceptionNotFound)
		{
			fsThrow(ExceptionInvalidArgument);
		}
	}

private:
	struct MultiMapItem
	{
		D self;
		fsList<D> data; // This data member must be at the top of this structure.
		typename fsList<D>::Item list_item;
		typename fsList<MultiMapItem>::Item order_item;
		typename fsList<MultiMapItem>::Item hash_item;
		K key;
	};

	fsMultiMap(const fsMultiMap<K, D>&) {}
	void operator=(const fsMultiMap<K, D>&) {}

	fsList<MultiMapItem> m_order_list;
	fsList<MultiMapItem>* m_hash_list;
	MultiMapItem* m_last_item1;
	MultiMapItem* m_last_item2;
	MultiMapItem* m_last_item3;
	u16 m_hash_size;
	u16 m_data_num;
};


#endif /* FS_MAP_H_ */
