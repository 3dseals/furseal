/*
    Copyright (c) 2011 netsurfers
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
    fsDefineException(ExceptionEndCatcake);
    //! @endcond


    /*!
        系统标志.
    */
    enum SysFlag
    {
        FLAG_FULLSCREEN_START = 0x0001, //!< 全屏模式.
        FLAG_VARIABLE_SIZE = 0x0002, //!< 可拉伸窗口模式.
        FLAG_DISABLE_SHADER = 0x0004 //!< 禁用窗口阴影模式.
    };


    /*!
        设置全局初始化,该方法只被引擎自己调用.
        @param[in] argc 主函数参数数目.
        @param[in] argv 主函数参数数组.
    */
    static void setInitialDirectoryForEngine(s32 argc, char** argv);


private:
    fsMgr(const char* title, u16 width, u16 height, u16 sys_flag);
    ~fsMgr();
};

#endif // !FS_MGR_H_
