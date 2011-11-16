/*
 * fs_body.h
 *
 *  Created on: 2011-11-16
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */

#ifndef FS_BODY_H_
#define FS_BODY_H_


/*!
    @ingroup fsBody
    一个物体.
*/
class FS_API fsBody
{
    friend class fsPhysicsMgr;

public:
    //! @cond
    fsDefineException(ExceptionInvalidArgument);
    fsDefineException(ExceptionInvalidCall);
    //! @endcond


    virtual ~fsBody();


protected:
    //! @cond
    fsBody();
    fsBody(int body_count, int pos_num, int vel_num);
    //! @endcond


    int m_body_count;
    float* m_pos;
    float* m_vel;
    int m_start_index_pos;
    int m_start_index_vel;
    bool changed_pos;
    bool changed_vel;

private:

    fsBody(const fsBody&);
    void operator=(const fsBody&);
};

#endif /* FS_BODY_H_ */
