/*
 * fs_state.h
 *
 *  Created on: 2011-11-16
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */

#ifndef FS_STATE_H_
#define FS_STATE_H_


/*!
    @ingroup fsState
    状态.
*/
class FS_API fsState
{

public:
    //! @cond
    fsDefineException(ExceptionInvalidArgument);
    fsDefineException(ExceptionInvalidCall);
    //! @endcond

    fsState();

    virtual ~fsState();


protected:
    //! @cond

    //! @endcond


private:

    fsState(const fsState&);
    void operator=(const fsState&);
};

#endif /* FS_STATE_H_ */
