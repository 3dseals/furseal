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
    fsDefineException(ExceptionCannotOpenFile);
    fsDefineException(ExceptionCannotReadFile);
    fsDefineException(ExceptionCannotWriteFile);
    fsDefineException(ExceptionCreateFramebufferFailed);
    fsDefineException(ExceptionEndFurseal);
    fsDefineException(ExceptionInvalidArgument);
    fsDefineException(ExceptionNotInitialized);
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
        文件模式.
    */
    enum FileMode
    {
        FILE_MODE_READ, //!< Read-only.
        FILE_MODE_WRITE //!< Read and write.
    };


    /*!
        Writes a string.
        @param[in] str A string.
        @param[in] ... Additional arguments.
    */
    static void printf(const char* str, ...);


    /*!
        Writes a wchar string.
        @param[in] str A string.
        @param[in] ... Additional arguments.
    */
    static void wprintf(const wchar_t* str, ...);


    /*!
        Writes a string to the specified buffer.
        @param[out] buf A buffer.
        @param[in] buf_size The size of a buffer.
        @param[in] str A string.
        @param[in] ... Additional arguments.
    */
    static void sprintf(char* buf, u32 buf_size, const char* str, ...);


    /*!
        Writes a wchar string to the specified buffer.
        @param[out] buf A buffer.
        @param[in] buf_size The size of a buffer.
        @param[in] str A string.
        @param[in] ... Additional arguments.
    */
    static void swprintf(wchar_t* buf, u32 buf_size, const wchar_t* str, ...);


    /*!
        打开指定的文件.
        @param[in] filename 文件路径.
        @param[in] file_mode 打开模式.
        @return 文件指针.
    */
    static void* openFile(const char* filename, FileMode file_mode);


    /*!
        返回指定的文件大小.
        @param[in] file_文件的指针.
        @return 文件的大小.
    */
    static u32 getFileSize(void* file_handler);


    /*!
        读取指定的文件.
        @param[out] buf 读取到的缓存块.
        @param[in] offset 读取的位置.
        @param[in] size 读取的大小.
        @param[in] file_handler 文件指针.
    */
    static void readFile(void* buf, u32 offset, u32 size, void* file_handler);


    /*!
        写入指定的文件.
        @param[in] offset 写入的位置.
        @param[in] buf 写入的数据缓存.
        @param[in] size 写入的大小.
        @param[in] file_handler 文件指针.
    */
    static void writeFile(u32 offset, const void* buf, u32 size, void* file_handler);


    /*!
        关闭指定的文件.
        @param[in] file_handler 文件指针.
    */
    static void closeFile(void* file_handler);


    /*!
        返回当前时间.
    */
    static u64 getUsecTime();


    /*!
        睡眠一段时间.
        @param[in] usec 睡眠时间.
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
    static void createAfterMem(const char* title, u16 width, u16 height, u16 aim_fps, u16 sys_flag);


    /*!
        销毁引擎管理器,该方法只被引擎自己调用.
    */
    static void destroyBeforeMem();


    /*!
        返回帧缓冲区的宽度.
        @return 帧缓冲区的宽度.
    */
    static u16 getFramebufferWidth();

    /*!
        返回帧缓冲区的高度.
        @return 帧缓冲区的高度.
    */
    static u16 getFramebufferHeight();


    /*!
        Returns whether the size of the framebuffer is changed.
        @return Whether the size of the framebuffer is changed.
    */
    static bool isFramebufferSizeChanged();


    /*!
        更新引擎管理器,该方法只被引擎自己调用.
    */
    static void updateForEngine(bool render);

    /*!
		Reads the data in little endian.
		@param[out] dest The destination.
		@param[in] src The source.
		@param[in] size The size of the data.
	*/
	static void readLittleEndianForEngine(void* dest, const void* src, u32 size);

	/*!
		Reads the data in little endian. This method is only for system.
		@tparam T The class type.
		@param[in] data The source data.
	*/
	template<class T> static T readLittleEndianForEngine(const void* data)
	{
		T value;

		readLittleEndianForEngine(&value, data, sizeof(T));

		return value;
	}

	/*!
		Writes the data in little endian.
		@param[out] dest The destination.
		@param[in] src The source.
		@param[in] size The size of the data.
	*/
	static void writeLittleEndianForEngine(void* dest, const void* src, u32 size);

	/*!
		Writes the data in little endian. This method is only for system.
		@tparam T The class type.
		@param[out] dest The destination.
		@param[in] value The value.
	*/
	template<class T> static void writeLittleEndianForEngine(void* dest, const T& value)
	{
		writeLittleEndianForEngine(dest, &value, sizeof(T));
	}

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
