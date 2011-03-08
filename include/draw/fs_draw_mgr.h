/*
 * fs_draw_mgr.h
 *
 *  Created on: 2011-3-4
 *      Author: Lional King
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
        创建绘图管理器,该方法只被引擎自己调用.
    */
    static void createAfterRes();


    /*!
        销毁绘图管理器,该方法只被引擎自己调用.
    */
    static void destroyBeforeRes();


    /*!
        更新图帧缓存进行渲染,该方法只被引擎自己调用.
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
