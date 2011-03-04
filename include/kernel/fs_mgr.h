/*
 * fs_mgr.h
 *
 *  Created on: 2011-3-2
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
*/


#ifndef FS_MGR_H_
#define FS_MGR_H_


/*!
    @ingroup fsKen
    引擎内核管理.
*/
class FS_API fsMgr
{
public:
    //! @cond
    fsDefineException(ExceptionCreateFramebufferFailed);
    fsDefineException(ExceptionEndCatcake);
    fsDefineException(ExceptionInvalidArgument);
    //! @endcond


    /*!
        系统标志.
    */
    enum SysFlag
    {
        FLAG_FULLSCREEN_START = 0x0001, //!< 全屏模式.
        FLAG_VARIABLE_SIZE = 0x0002, //!< 可拉伸窗口模式.
        FLAG_DISABLE_SHADER = 0x0004 //!< 禁用openGL阴影模式.
    };


    /*!
        Returns the current time in usec.
    */
    static u64 getUsecTime();


    /*!
        Sleeps for the specified time.
        @param[in] usec The period to sleep in usec.
    */
    static void sleepUsec(u64 usec);


    /*!
        判断引擎管理器是否创建.
        @return 引擎管理器是否创建.
    */
    static bool isCreated();


    /*!
        创建引擎管理器,该方法只被引擎自己调用.
        @param[in] title 应用程序的标题.
        @param[in] width 创建窗口的宽度.
        @param[in] height 创建窗口的高度.
        @param[in] sys_flag 系统标志位,默认为0,1 全屏模式,2 可拉伸窗口模式,4 禁用openGL阴影模式.
    */
    static void createAfterMem(const char* title, u16 width, u16 height, u16 sys_flag);


    /*!
        销毁引擎管理器,该方法只被引擎自己调用.
    */
    static void destroyBeforeMem();


    /*!
        更新引擎管理器,该方法只被引擎自己调用.
    */
    static void updateForEngine();


    /*!
        设置全局初始化,该方法只被引擎自己调用.
        @param[in] argc 主函数参数数目.
        @param[in] argv 主函数参数数组.
    */
    static void setInitialDirectoryForEngine(s32 argc, char** argv);


private:
    fsMgr(const char* title, u16 width, u16 height, u16 sys_flag);
    ~fsMgr();
    void operator=(const fsMgr&);

    static fsMgr* instance();

    static fsMgr* m_instance;
};

#endif // !FS_MGR_H_
