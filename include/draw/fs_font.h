/*
 * fs_font.h
 *
 *  Created on: 2011-4-6
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */

#ifndef FS_FONT_H_
#define FS_FONT_H_


/*!
    @ingroup fsDraw
    字体, 专门用来绘制字体的纹理.
*/
class FS_API fsFont
{
public:
    //! @cond
    fsDefineException(ExceptionDrawStringFailed);
    fsDefineException(ExceptionInvalidArgument);
    fsDefineException(ExceptionInvalidCall);
    fsDefineException(ExceptionNotInitialized);
    //! @endcond

    /*!
        构造字体.
    */
    fsFont();

    /*!
        析构字体.
    */
    ~fsFont();

    /*!
        初始化字体.
        @param[in] width 字体的宽度.
        @param[in] height 字体的高度.
    */
    void init(u16 width, u16 height);

    /*!
        返回字体的 ID.
        @return 字体的 ID.
    */
    fsID getTextureID() const;

    /*!
        返回字体的宽度.
        @return 字体的宽度.
    */
    u16 getWidth() const;

    /*!
        返回字体的高度.
        @return 字体的高度.
    */
    u16 getHeight() const;

    /*!
        返回字体的只读图像.
        @return 字体的图像.
    */
    const void* getImage() const;

    /*!
        返回字体的图像尺寸大小.
        @return 字体的图像尺寸大小.
    */
    u32 getImageSize() const;

    /*!
        开始编辑字体的图像.
        编辑完后必须调用结束编辑图像.
        @return 字体的图像.
    */
    void* beginEditImage();

    /*!
        结束编辑图像并更新整个区域.
    */
    void endEditImage();

    /*!
        结束编辑图像并更新指定区域.
        @param[in] x 修改区域的左侧坐标.
        @param[in] y 修改区域的顶部坐标.
        @param[in] 修改区域的宽度.
        @param[in] 修改区域的高度.
    */
    void endEditImage(u16 x, u16 y, u16 width, u16 height);

    /*!
        把字体的所有像素置 0.
        必须在之前调用开始编辑图像.
    */
    void clearImage();

    /*!
        渲染指定的字符串.
        @param[in] x 绘制坐标的 x 轴.
        @param[in] y 绘制坐标的 y 轴.
        @param[in] str 指定的字符串.
        @param[in] ... 参数.
    */
    u16 drawString(s16 x, s16 y, const char* str, ...);

    /*!
        渲染指定的宽字节字符串.
        @param[in] x 绘制坐标的 x 轴.
        @param[in] y 绘制坐标的 y 轴.
        @param[in] str 指定的宽字节字符串.
        @param[in] ... 参数.
    */
    u16 drawString(s16 x, s16 y, const wchar_t* str, ...);

private:
    fsTex* m_tex;
};


#endif /* FS_FONT_H_ */
