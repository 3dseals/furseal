/*
 * fs_util.h
 *
 *  Created on: 2011-3-9
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */

#ifndef FS_UTIL_H_
#define FS_UTIL_H_


/*!
    @ingroup fsRes
    资源管理器的实用工具.
*/
class FS_API fsUtil
{
public:
    //! @cond
    fsDefineException(ExceptionInvalidArgument);


    /*!
        从文件路径名中返回文件名.
        @param[in] filename 文件路径名.
        @return 文件名.
    */
    static const char* getBasename(const char* filename);


    /*!
        从文件路径名中返回扩展名.
        @param[in] filename 文件路径名.
        @return 扩展名.
    */
    static const char* getExtension(const char* filename);
};


#endif /* FS_UTIL_H_ */
