/*
 * fs_node.h
 *
 *  Created on: 2011-5-27
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */


#ifndef FS_NODE_H_
#define FS_NODE_H_


/*!
    @ingroup fsDraw
    A drawing element node, which has no physical appearance.
*/
class FS_API fsNode : public fsDraw
{
public:
    //! @cond
    fsDefineException(ExceptionInvalidArgument);
    //! @endcond

    /*!
        Constructs a node.
    */
    fsNode();

    /*!
        Destructs this node.
    */
    virtual ~fsNode();

    /*!
        Initializes this node.
        @param[in] scr_id A screen ID.
    */
    void init(fsID scr_id);

    /*!
        Initializes this node.
        @param[in] parent A parent drawing element.
    */
    void init(fsDraw* parent);
};


#endif /* FS_NODE_H_ */
