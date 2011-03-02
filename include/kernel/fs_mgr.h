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