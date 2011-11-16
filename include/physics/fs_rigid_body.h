/*
 * fs_rigid_body.h
 *
 *  Created on: 2011-11-16
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */

#ifndef FS_RIGID_BODY_H_
#define FS_RIGID_BODY_H_


/*!
    @ingroup fsRigidBody
    刚性物体.
*/
class FS_API fsRigidBody : public fsBody
{
    friend class fsPhysicsMgr;

public:
    //! @cond
    fsDefineException(ExceptionInvalidArgument);
    fsDefineException(ExceptionInvalidCall);
    //! @endcond

protected:
    //! @cond
    virtual ~fsRigidBody();
    //! @endcond


private:

    fsRigidBody();
    fsRigidBody(const fsRigidBody&);
    void operator=(const fsRigidBody&);
};

#endif /* FS_RIGID_BODY_H_ */
