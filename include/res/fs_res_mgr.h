/*
 * fs_res_mgr.h
 *
 *  Created on: 2011-3-9
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */

#ifndef FS_RES_MGR_H_
#define FS_RES_MGR_H_


/*!
    @ingroup fsRes
    引擎资源管理器.
*/
class FS_API fsResMgr
{
public:
    //! @cond
    fsDefineException(ExceptionCannotOpenFile);
    fsDefineException(ExceptionCannotReadFile);
    fsDefineException(ExceptionInvalidArgument);
    fsDefineException(ExceptionNotFound);
    fsDefineException(ExceptionNotInitialized);
    fsDefineException(ExceptionSameExtensionExists);
    fsDefineException(ExceptionSameIDExists);
    //! @endcond

    /*!
        资源初始化类型.
        @param[in] id 资源的ID号.
        @param[in] ext 资源的扩展名.
        @param[in] data 资源的数据.
        @param[in] data_size 资源的数据大小.
        @param[out] extinfo 资源额外的信息.
    */
    typedef void (*Initializer)(fsID id, fsStr<char, 3> ext, const void* data, u32 data_size, void** exinfo);

    /*!
        资源释放类型.
        @param[in] id 资源的ID号.
        @param[in] ext 资源的扩展名.
        @param[in] data 资源的数据.
        @param[out] extinfo 资源额外的信息.
    */
    typedef void (*Finalizer)(fsID id, fsStr<char, 3> ext, const void* data, u32 data_size, void* exinfo);

    /*!
        判断资源管理器是否创建.
        @return 资源管理器是否创建.
    */
    static bool isCreated();

    /*!
        创建资源管理器,该方法只被引擎自己调用.
    */
    static void createAfterTask();

    /*!
        销毁资源管理器,该方法只被引擎自己调用.
    */
    static void destroyBeforeSys();

    /*!
        返回注册类型的数目.
        @return The number of the registered types.
    */
    static u16 getTypeNum();

    /*!
        返回资源数目.
    */
    static u16 getResourceNum();

    /*!
        注册指定类型.
        @param[in] ext 类型的扩展名.
        @param[in] init 类型初始化函数.
        @param[in] final 类型释放函数.
    */
    static void addType(fsStr<char, 3> ext, Initializer init, Finalizer final);

    /*!
        注销指定类型.
        @param[in] ext 类型扩展名.
    */
    static void removeType(fsStr<char, 3> ext);

    /*!
        判断指定资源类型是否注册.
        @param[in] id 资源的ID号.
        @return 指定资源类型是否注册.
    */
    static bool hasResource(fsID id);

    /*!
        返回指定资源.
        @param[in] id 资源的ID号.
        @return The 指定的资源.
    */
    static fsRes getResource(fsID id);

    /*!
        注册指定的资源.
        @param[in] id 资源的ID号.
        @param[in] ext 资源的扩展名.
        @param[in] data 资源的数据.
        @param[in] data_size 资源的数据大小.
        @param[in] is_auto_free 是否自动的释放资源.
    */
    static void addResource(fsID id, fsStr<char, 3> ext, const void* data, u32 data_size, bool is_auto_free);

    /*!
        注销指定资源.
        @param[in] id 资源的ID号.
    */
    static void removeResource(fsID id);

    /*!
        载入指定资源.
        @param[in] filename 文件路径名.
        @param[in] is_type_detect 是否自动初始化/释放.
    */
    static void loadResource(const char* filename, bool is_type_detect);

    /*!
        载入指定ID的资源.
        @param[in] id 资源的ID号.
        @param[in] filename filename 文件路径名.
        @param[in] is_type_detect 是否自动初始化/释放.
    */
    static void loadResourceAs(fsID id, const char* filename, bool is_type_detect);

    /*!
        返回第一个注册类型,如果不存在则返回NULL.
        @return 第一个注册类型.
    */
    static const fsStr<char, 3>* getFirstTypeN();

    /*!
        返回下一个指定的注册类型,如果不存在则返回NULL.
        @param[in] ext 资源的扩展名.
    */
    static const fsStr<char, 3>* getNextTypeN(fsStr<char, 3> ext);

    /*!
        返回第一个资源,如果不存在则返回NULL.
        @return 第一个资源.
    */
    static const fsRes* getFirstResourceN();

    /*!
        返回下一个指定的资源,如果不存在则返回NULL.
        @param[in] id 资源的ID号.
    */
    static const fsRes* getNextResourceN(fsID id);

private:
    static const u32 TYPE_HASH_SIZE = 1;
    static const u32 RESOURCE_HASH_SIZE = 10;

    struct TypeInfo
    {
        fsStr<char, 3> ext;
        Initializer init;
        Finalizer final;
    };

    fsResMgr();
    ~fsResMgr();
    void operator=(const fsResMgr&);

    static fsResMgr* instance();

    fsMap<fsStr<char, 3>, TypeInfo> m_type_info_map;
    fsMap<fsID, fsRes> m_res_map;

    static fsResMgr* m_instance;
};


#endif /* FS_RES_MGR_H_ */
