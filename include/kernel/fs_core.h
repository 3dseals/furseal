/*
    Copyright (c) 2011 netsurfers
*/


#ifndef FS_CORE_H_
#define FS_CORE_H_


/*!
    @ingroup fsKen
    创建furseal引擎的管理器.
    @param[in] title 应用程序的标题.
    @param[in] width 创建窗口的宽度.
    @param[in] height 创建窗口的高度.
    @param[in] aim_fps 每秒保持的帧频.
    @param[in] sys_flag 系统标志位,默认为0.
*/
FS_API void fsCreateFurseal(const char* title, u16 width, u16 height, u16 aim_fps, u16 sys_flag = 0);

/*!
    @ingroup fsKen
    销毁furseal引擎的管理器.
*/
FS_API void fsDestroyFurseal();

/*!
    @ingroup fsKen
    开始执行furseal引擎管理器循环.
*/
FS_API void fsStartFurseal();

/*!
    @ingroup fsKen
    停止执行furseal引擎管理器循环.
*/
FS_API void fsStopFurseal();

/*!
    @ingroup fsKen
    发出furseal错误.
    @param msg 错误信息.
    @param ... 附加参数.
*/
FS_API void fsError(const char* msg, ...);


#endif // !FS_CORE_H_
