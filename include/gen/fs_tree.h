/*
 * fs_tree.h
 *
 *  Created on: 2011-3-4
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */

#ifndef FS_TREE_H_
#define FS_TREE_H_


/*!
    @ingroup fsGen
    一颗带有父节点和孩子节点的树型数据结构.
    @tparam T 存储树的数据类型.
*/
template<class T> class fsTree
{
public:
    //! @cond
    fsDefineException(ExceptionInvalidArgument);
    fsDefineException(ExceptionInvalidCall);
    fsDefineException(ExceptionNotInitialized);
    //! @endcond

    /*!
        构造一颗树结构fsTree.
    */
    fsTree()
    {
        m_self = NULL;
        m_parent = NULL;
        m_last_child = NULL;
        m_prev = m_next = NULL;
    }

    /*!
        销毁这棵树fsTree.
    */
    ~fsTree()
    {
        leave();
        clear();
    }

    /*!
        初始化树fsTree的这个节点.
        可以进行多次初始化.
        @param[in] self 树型数据的首地址.
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
        返回这个节点的数据.
        @return 这个节点的数据.
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
        判断这个节点是否有父节点.
        @return 这个节点是否有父节点.
    */
    bool hasParent() const
    {
        return m_parent ? true : false;
    }

    /*!
        返回这个节点的父节点.
        如果没有父节点则返回NULL.
        @return 这个节点的父节点.
    */
    fsTree<T>* getParentN() const
    {
        return m_parent;
    }

    /*!
        返回这个节点的前一个点.
        如果没有前一个点则返回NULL.
        这个方法用于有兄弟节点的树结构.
        @return 这个节点的前一个点.
    */
    fsTree<T>* getPrevAllN() const
    {
        return m_prev;
    }

    /*!
        返回这个节点的后一个点.
        如果没有后一个点则返回NULL.
        这个方法用于有兄弟节点的树结构.
        @return 这个节点的后一个点.
    */
    fsTree<T>* getNextAllN() const
    {
        return m_next;
    }

    /*!
        返回这个节点的前一个兄弟点.
        如果没有前一个兄弟点则返回NULL.
        @return 这个节点的前一个兄弟点.
    */
    fsTree<T>* getPrevSiblingN() const
    {
        if (!hasParent())
        {
            fsThrow(ExceptionInvalidCall);
        }

        if (m_prev != m_parent)
        {
            fsTree<T>* prev = m_prev;

            while (prev->m_parent != m_parent)
            {
                prev = prev->m_parent;
            }

            return prev;
        }
        else
        {
            return NULL;
        }
    }

    /*!
        返回这个节点的后一个兄弟点.
        如果没有后一个兄弟点则返回NULL.
        @return 这个节点的后一个兄弟点.
    */
    fsTree<T>* getNextSiblingN() const
    {
        if (!hasParent())
        {
            fsThrow(ExceptionInvalidCall);
        }

        fsTree<T>* desc = getLastDescendant();
        fsTree<T>* next = desc->m_next;

        return (next && next->m_parent == m_parent) ? next : NULL;
    }

    /*!
        将这个节点插入到一个树节点之前.
        @param[in] tree 一个fsTree树型节点.
    */
    void joinBefore(fsTree<T>* tree)
    {
        if (!tree || tree == this || !tree->hasParent())
        {
            fsThrow(ExceptionInvalidArgument);
        }

        for (fsTree<T>* parent = tree->m_parent; parent; parent = parent->m_parent)
        {
            if (parent == this)
            {
                fsThrow(ExceptionInvalidArgument);
            }
        }

        leave();

        fsTree<T>* desc = getLastDescendant();

        m_parent = tree->m_parent;
        m_prev = tree->m_prev;
        desc->m_next = tree;

        m_prev->m_next = this;
        desc->m_next->m_prev = desc;
    }

    /*!
        将这个节点插入到一个树节点之后.
        @param[in] tree 一个fsTree树型节点.
    */
    void joinAfter(fsTree<T>* tree)
    {
        if (!tree || tree == this || !tree->hasParent())
        {
            fsThrow(ExceptionInvalidArgument);
        }

        for (fsTree<T>* parent = tree->m_parent; parent; parent = parent->m_parent)
        {
            if (parent == this)
            {
                fsThrow(ExceptionInvalidArgument);
            }
        }

        leave();

        fsTree<T>* tree_desc = tree->getLastDescendant();
        fsTree<T>* this_desc = getLastDescendant();

        m_parent = tree->m_parent;
        m_prev = tree_desc;
        this_desc->m_next = tree_desc->m_next;

        m_prev->m_next = this;

        if (this_desc->m_next)
        {
            this_desc->m_next->m_prev = this_desc;
        }

        if (m_parent->m_last_child == tree)
        {
            m_parent->m_last_child = this;
        }
    }

    /*!
        从父节点上移除这个节点.
    */
    void leave()
    {
        if (hasParent())
        {
            fsTree<T>* desc = getLastDescendant();

            if (m_parent->m_last_child == this)
            {
                m_parent->m_last_child = getPrevSiblingN();
            }

            m_prev->m_next = desc->m_next;

            if (desc->m_next)
            {
                desc->m_next->m_prev = m_prev;
            }

            m_parent = NULL;
            m_prev = desc->m_next = NULL;
        }
    }

    /*!
         判断这个节点是否有孩子节点.
        @return 这个节点是否有孩子节点.
    */
    bool hasChild() const
    {
        return m_last_child ? true : false;
    }

    /*!
        返回这个节点的第一个孩子节点.
        如果这个节点没有孩子节点则返回NULL.
        @return 第一个孩子节点.
    */
    fsTree<T>* getFirstChildN() const
    {
        return hasChild() ? m_next : NULL;
    }

    /*!
        返回这个节点的最后一个孩子节点.
        如果这个节点没有孩子节点则返回NULL.
        @return 最后一个孩子节点.
    */
    fsTree<T>* getLastChildN() const
    {
        return m_last_child;
    }

    /*!
        返回这个节点的最后一个子孙节点.
        如果这个节点没有孩子节点则这个节点.
        @return 最后一个子孙节点.
    */
    fsTree<T>* getLastDescendant() const
    {
        fsTree<T>* desc = const_cast<fsTree<T>*>(this);

        while (desc->m_last_child)
        {
            desc = desc->m_last_child;
        }

        return desc;
    }

    /*!
        添加一颗子树,根节点作为第一个子节点.
        @param[in] child 子树fsTree的根节点.
    */
    void addFirst(fsTree<T>* child)
    {
        if (!child || child == this)
        {
            fsThrow(ExceptionInvalidArgument);
        }

        for (fsTree<T>* parent = m_parent; parent; parent = parent->m_parent)
        {
            if (parent == child)
            {
                fsThrow(ExceptionInvalidArgument);
            }
        }

        child->leave();

        fsTree<T>* child_desc = child->getLastDescendant();

        child->m_parent = this;
        child->m_prev = this;
        child_desc->m_next = m_next;

        child->m_prev->m_next = child;

        if (child_desc->m_next)
        {
            child_desc->m_next->m_prev = child_desc;
        }

        if (!m_last_child)
        {
            m_last_child = child;
        }
    }

    /*!
        添加一颗子树,根节点作为最后一个子节点.
        @param[in] child 子树fsTree的根节点.
    */
    void addLast(fsTree<T>* child)
    {
        if (!child || child == this)
        {
            fsThrow(ExceptionInvalidArgument);
        }

        for (fsTree<T>* parent = m_parent; parent; parent = parent->m_parent)
        {
            if (parent == child)
            {
                fsThrow(ExceptionInvalidArgument);
            }
        }

        child->leave();

        fsTree<T>* this_desc = getLastDescendant();
        fsTree<T>* child_desc = child->getLastDescendant();

        child->m_parent = this;
        child->m_prev = this_desc;
        child_desc->m_next = this_desc->m_next;

        child->m_prev->m_next = child;

        if (child_desc->m_next)
        {
            child_desc->m_next->m_prev = child_desc;
        }

        m_last_child = child;
    }

    /*!
        移除所有子节点.
    */
    void clear()
    {
        while (hasChild())
        {
            getFirstChildN()->leave();
        }
    }

private:
    fsTree(const fsTree<T>&) {}
    void operator=(const fsTree<T>&) {}

    T* m_self;
    fsTree<T>* m_parent;
    fsTree<T>* m_last_child;
    fsTree<T>* m_prev;
    fsTree<T>* m_next;
};


#endif /* FS_TREE_H_ */
