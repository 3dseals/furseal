/*
 * fs_contact.h
 *
 *  Created on: 2011-11-16
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */

#ifndef FS_CONTACT_H_
#define FS_CONTACT_H_


/*!
    @ingroup fsContact
    非抢占式任务.
*/
class FS_API fsContact
{
    friend class fsPhysicsMgr;

public:
    //! @cond
    fsDefineException(ExceptionInvalidArgument);
    fsDefineException(ExceptionInvalidCall);


    virtual ~fsContact();


private:
    fsContact();
    fsContact(const fsContact&);
    void operator=(const fsContact&);
};

#endif /* FS_CONTACT_H_ */
