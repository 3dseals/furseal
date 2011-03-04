/*
 * fs_draw_mgr.h
 *
 *  Created on: 2011-3-4
 *      Author: administrator
 *
 *  Copyright (c) 2011 netsurfers
 */

#ifndef FS_DRAW_MGR_H_
#define FS_DRAW_MGR_H_


/*!
    @ingroup fsDraw
    引擎绘图管理器.
*/
class FS_API fsDrawMgr
{
public:
    //! @cond
    //! @endcond


	static const u32 MAX_RENDER_INTERVAL_MSEC_TIME = 100;


    /*!
        判断绘图管理器是否创建.
        @return 绘图管理器是否创建.
    */
    static bool isCreated();


    /*!
        Creates ckDrawMgr singleton.
    */
    static void createAfterRes();


    /*!
        Destroys ckDrawMgr singleton.
    */
    static void destroyBeforeRes();


    /*!
        Updates the framebuffer. This methos is only for system.
    */
    static void renderForEngine();


private:

    fsDrawMgr();
    ~fsDrawMgr();
    void operator=(const fsDrawMgr&);

    static fsDrawMgr* instance();

    bool m_is_render;

    static fsDrawMgr* m_instance;
};


#endif /* FS_DRAW_MGR_H_ */
