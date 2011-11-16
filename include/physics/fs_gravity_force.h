/*
 * fs_gravity_force.h
 *
 *  Created on: 2011-11-16
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */

#ifndef FS_GRAVITY_FORCE_H_
#define FS_GRAVITY_FORCE_H_


/*!
    @ingroup fsGravityForce
    重力效应.
*/
class FS_API fsGravityForce : public fsForce
{
public:
    //! @cond
    fsDefineException(ExceptionInvalidArgument);
    fsDefineException(ExceptionInvalidCall);
    //! @endcond


protected:
    //! @cond
    virtual ~fsGravityForce();
    //! @endcond


private:

    fsGravityForce();
    fsGravityForce(const fsGravityForce&);
    void operator=(const fsGravityForce&);
};

#endif /* FS_GRAVITY_FORCE_H_ */
