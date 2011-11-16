/*
 * fs_force.h
 *
 *  Created on: 2011-11-16
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */

#ifndef FS_FORCE_H_
#define FS_FORCE_H_


/*!
    @ingroup fsForce
    非抢占式任务.
*/
class FS_API fsForce
{
    friend class fsPhysicsMgr;

public:
    //! @cond
    fsDefineException(ExceptionInvalidArgument);
    fsDefineException(ExceptionInvalidCall);
    //! @endcond


    //virtual ~fsForce();


protected:
    //! @cond
    fsForce();
    virtual ~fsForce();
    //! @endcond


private:
    fsForce(const fsForce&);
    void operator=(const fsForce&);
};

#endif /* FS_FORCE_H_ */
