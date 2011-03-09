/*
 * fs_res.h
 *
 *  Created on: 2011-3-9
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */

#ifndef FS_RES_H_
#define FS_RES_H_


class fsResMgr;


/*!
    @ingroup fsRes
    引擎资源.
*/
class FS_API fsRes
{
    friend class fsResMgr;

public:
    /*!
        构建一个资源.
    */
    fsRes();

    /*!
        返回资源的ID号.
        @return 资源的ID号.
    */
    fsID getID() const;

    /*!
        返回资源的扩展名.
        @return 资源的扩展名.
    */
    fsStr<char, 3> getExtension() const;

    /*!
        返回资源的数据.
        @tparam T 数据类型.
        @return 资源的数据.
    */
    template<class T> const T* getData() const
    {
        return reinterpret_cast<const T*>(m_data);
    }

    /*!
        返回资源的数据大小.
        @return 资源的数据大小.
    */
    u32 getDataSize() const;

    /*!
        返回资源的额外信息.
        @tparam T 额外信息的类型.
        @return 资源的额外信息.
    */
    template<class T> T* getExInfo() const
    {
        return reinterpret_cast<T*>(m_exinfo);
    }

    /*!
        返回是否自动的释放资源.
        @return 是否自动的释放资源.
    */
    bool isAutoFree() const;

private:
    void init(fsID id, fsStr<char, 3> ext, const void* data, u32 data_size, void* exinfo, bool is_auto_free);

    static fsStr<char, 3> toUppercase(fsStr<char, 3> str);

    fsID m_id;
    fsStr<char, 3> m_ext;
    const void* m_data;
    u32 m_data_size;
    void* m_exinfo;
    bool m_is_auto_free;
};


#endif /* FS_RES_H_ */
