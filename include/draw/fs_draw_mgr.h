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

    friend class ckLts;
    friend class ckScr;


public:
    //! @cond
    fsDefineException(ExceptionInvalidArgument);
    fsDefineException(ExceptionNotFound);
    fsDefineException(ExceptionSameIDExists);
    //! @endcond


    /*!
        The ID of the invisible screen.
    */
    static const fsID INVISIBLE_SCREEN_ID;

    /*!
        The ID of the default 3D screen.
    */
    static const fsID DEFAULT_3D_SCREEN_ID;

    /*!
        The ID of the defualt 2D screen.
    */
    static const fsID DEFAULT_2D_SCREEN_ID;

    /*!
        The ID of the default light set.
    */
    static const fsID DEFAULT_LIGHT_SET_ID;

    /*!
        The maximum rendering interval time [msec].
    */
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
        Returns the specified screen. If the specified screen doesn't exist, returns NULL.
        The new screen is linked to the last of the screen list.
        @param[in] scr_id A screen ID.
        @return A screen.
    */
    static fsScr* getScreen(fsID scr_id);

    /*!
        Creates new screen which has the specified ID.
        @param[in] scr_id A screen ID.
        @return The instance of new screen.
    */
    static fsScr* newScreen(fsID scr_id);


    /*!
        Returns the specified light-set.
        @param[in] lts_id A light-set ID.
        @return The specified light-set.
    */
    static fsLts* getLightSet(fsID lts_id);

    /*!
        Creates a new light-set.
        @param[in] lts_id A light-set ID.
        @return A light-set.
    */
    static fsLts* newLightSet(fsID lts_id);


    /*!
        Deletes the specified light-set.
        @param[in] lts_id A light-set ID.
    */
    static void deleteLightSet(fsID lts_id);


    /*!
        Deletes the all texture objects. This method is only for system.
    */
    static void deleteAllVramObjForEngine();


    /*!
        更新图帧缓存进行渲染,该方法只被引擎自己调用.
    */
    static void renderForEngine();


    static fsDrawMgr* instance();
    fsMap<fsID, fsScr*> m_scr_map;
    fsMap<fsID, fsLts*> m_lts_map;


private:

    static const u32 SCREEN_HASH_SIZE = 10;
    static const u32 LIGHTSET_HASH_SIZE = 10;

    fsDrawMgr();
    ~fsDrawMgr();
    void operator=(const fsDrawMgr&);

    static void renderScreen(fsScr* scr, fsDraw** sort_list, const fsMat& view);
    static void sortList(fsDraw** sorted_start, fsDraw** sorted_end, fsDraw* target_list);

    bool m_is_render;

    static fsDrawMgr* m_instance;
};


#endif /* FS_DRAW_MGR_H_ */
