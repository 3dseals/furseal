/*
 * fs_tree.h
 *
 *  Created on: 2011-3-4
 *      Author: administrator
 *
 *  Copyright (c) 2011 netsurfers
 */

#ifndef FS_TREE_H_
#define FS_TREE_H_


/*!
    @ingroup fsGen
    A tree container which can have a parent and children.
    @tparam T The type which is stored in the tree.
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
        Constructs a fsTree.
    */
    fsTree()
    {
        m_self = NULL;
        m_parent = NULL;
        m_last_child = NULL;
        m_prev = m_next = NULL;
    }

    /*!
        Destructs this fsTree.
    */
    ~fsTree()
    {
        leave();
        clear();
    }

    /*!
        Initializes this fsTree to have the specified value.
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
        Returns the value of this fsTree.
        @return The value of this fsTree.
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
        Returns whether this fsTree has a parent fsTree.
        @return Whether this fsTree has a parent fsTree.
    */
    bool hasParent() const
    {
        return m_parent ? true : false;
    }

    /*!
        Returns the parent fsTree of this fsTree.
        If this fsTree has no parent fsTree, returns NULL.
        @return The parent fsTree of this fsTree.
    */
    fsTree<T>* getParentN() const
    {
        return m_parent;
    }

    /*!
        Returns the previous fsTree of this fsTree.
        If this fsTree has no previous fsTree, returns NULL.
        This method is used to follow the whole tree as list structure.
        @return The previous fsTree of this fsTree.
    */
    fsTree<T>* getPrevAllN() const
    {
        return m_prev;
    }

    /*!
        Returns the next fsTree of this fsTree.
        If this fsTree has no next fsTree, returns NULL.
        This method is used to follow the whole tree as list structure.
        @return The next fsTree of this fsTree.
    */
    fsTree<T>* getNextAllN() const
    {
        return m_next;
    }

    /*!
        Returns the previous sibling fsTree of this fsTree.
        If this fsTree has no previous sibling fsTree, returns NULL.
        @return The previous sibling fsTree of this fsTree.
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
        Returns the next sibling fsTree of this fsTree.
        If this fsTree has no next sibling fsTree, returns NULL.
        @return The next sibling fsTree of this fsTree.
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
        Insertes this fsTree before the specified fsTree.
        @param[in] tree A fsTree.
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
        Insertes this fsTree after the specified fsTree.
        @param[in] tree A fsTree.
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
        Removes this fsTree from the parent fsTree.
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
        Returns Whether this fsTree has any child fsTree.
        @return Whether this fsTree has any child fsTree.
    */
    bool hasChild() const
    {
        return m_last_child ? true : false;
    }

    /*!
        Returns the first child fsTree of this fsTree.
        If this fsTree has no child fsTree, returns NULL.
        @return The first child fsTree of this fsTree.
    */
    fsTree<T>* getFirstChildN() const
    {
        return hasChild() ? m_next : NULL;
    }

    /*!
        Returns the last child fsTree of this fsTree.
        If this fsTree has no child fsTree, returns NULL.
        @return The last child fsTree of this fsTree.
    */
    fsTree<T>* getLastChildN() const
    {
        return m_last_child;
    }

    /*!
        Returns the last descendant of this fsTree.
        If this fsTree has no child, returns itself.
        @return The last descendant of this fsTree.
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
        Adds the specified fsTree to this fsTree as the first child fsTree.
        @param[in] child A fsTree.
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
        Adds the specified fsTree to this fsTree as the last child fsTree.
        @param[in] child A fsTree.
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
        Removes all child fsTrees from this fsTree.
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
