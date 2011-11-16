/*
 * fs_physics_mgr.h
 *
 *  Created on: 2011-11-16
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */

#ifndef FS_PHYSICS_MGR_H_
#define FS_PHYSICS_MGR_H_


/*!
    @ingroup fsPhysicsMgr
    物理引擎管理器.
*/
class FS_API fsPhysicsMgr
{
	friend class fsBody;
	friend class fsContact;
	friend class fsForce;
	friend class fsState;

public:
    //! @cond
    fsDefineException(ExceptionInvalidArgument);
    fsDefineException(ExceptionNotInitialized);
    //! @endcond


    /*!
        判断物理引擎管理器是否创建.
        @return 物理引擎管理器是否创建.
    */
    static bool isCreated();


    /*!
        创建物理引擎管理器,该方法只被引擎自己调用.
        @param[in] aim_fps An aim fps.
    */
    static void createAfterTask();


    /*!
        销毁物理引擎管理器,该方法只被引擎自己调用.
    */
    static void destroyBeforeSys();


    /*!
        更新物理引擎管理器,该方法只被引擎自己调用.
    */
    static void updateForEngine();


private:

    fsPhysicsMgr();
    ~fsPhysicsMgr();
    void operator=(const fsPhysicsMgr&);

    static fsPhysicsMgr* instance();

    fsList<fsBody> m_bodys;
    fsList<fsContact> m_cantacts;
    fsList<fsForce> m_forces;

    bool m_is_executing;
    bool m_bodies_modified;
    float m_error_tolerance;
    float m_next_step_size;
    fsState m_state1, m_state2, m_state3, m_state4;

    static fsPhysicsMgr* m_instance;
};


#endif /* FS_PHYSICS_MGR_H_ */
