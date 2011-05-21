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

    friend class fsTex;
    friend class fsFont;
    friend class fsShd;
    friend class fsLts;
    friend class fsScr;


public:
    //! @cond
    fsDefineException(ExceptionCalcFontDrawWidthFailed);
    fsDefineException(ExceptionCreateFreeTypeFailed);
    fsDefineException(ExceptionDestroyFreeTypeFailed);
    fsDefineException(ExceptionFontFinalizerFailed);
    fsDefineException(ExceptionFontInitializerFailed);
    fsDefineException(ExceptionInvalidArgument);
    fsDefineException(ExceptionInvalidCall);
    fsDefineException(ExceptionNotFound);
    fsDefineException(ExceptionNotInitialized);
    fsDefineException(ExceptionSameIDExists);
    fsDefineException(ExceptionTextureInitializerFailed);
    //! @endcond


    /*!
        不可见屏幕的 ID.
    */
    static const fsID INVISIBLE_SCREEN_ID;

    /*!
        默认3D屏幕的 ID.
    */
    static const fsID DEFAULT_3D_SCREEN_ID;

    /*!
        默认2D屏幕的 ID.
    */
    static const fsID DEFAULT_2D_SCREEN_ID;

    /*!
        默认光线装置的 ID.
    */
    static const fsID DEFAULT_LIGHT_SET_ID;

    /*!
        默认没有纹理的着色器的 ID.
    */
    static const fsID DEFAULT_NO_TEXTURE_SHADER_ID;

    /*!
        默认 RGB 纹理的着色器的 ID.
    */
    static const fsID DEFAULT_RGB_TEXTURE_SHADER_ID;

    /*!
        默认 RGBA 纹理的着色器的 ID.
    */
    static const fsID DEFAULT_RGBA_TEXTURE_SHADER_ID;

    /*!
        默认透明纹理的着色器的 ID.
    */
    static const fsID DEFAULT_ALPHA_TEXTURE_SHADER_ID;

    /*!
        最大渲染的时间间隔 [单位: 毫秒].
    */
    static const u32 MAX_RENDER_INTERVAL_MSEC_TIME = 100;


    /*!
        返回是否启用了可编程的着色器.
        @return 是否启用了可编程的着色器.
    */
    static bool isShaderAvailable();


    /*!
        返回纹理所支持的做大像素长度.
        @return 纹理所支持的做大像素长度.
    */
    static u16 getMaxTextureLength();


    /*!
        返回指定纹理像素长度的有效长度.
        @return 指定纹理像素长度的有效长度.
    */
    static u16 getValidTextureLength(u16 length);


    /*!
        返回指定纹理格式的比特大小.
        @return 指定纹理格式的比特大小.
    */
    static u16 getTexturePixelSize(fsTex::TexFormat format);


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
        返回是否启用了渲染过程.
        @return 是否启用了渲染过程.
    */
    static bool isRender();


    /*!
        设置是否启用渲染过程.
        @param[in] is_render 是否启用渲染过程.
    */
    static void setRender(bool is_render);


    /*!
        判断指定的屏幕是否存在.
        @param[in] scr_id 屏幕 ID.
        @return 指定的屏幕是否存在.
    */
    static bool hasScreen(fsID scr_id);


    /*!
        返回值定的屏幕如果不存在则返回 NULL.
        新建的屏幕在屏幕链表的最后.
        @param[in] scr_id 屏幕 ID.
        @return 指定的屏幕.
    */
    static fsScr* getScreen(fsID scr_id);

    /*!
        创建新的指定 ID 的屏幕.
        @param[in] scr_id 屏幕 ID.
        @return 新建的屏幕.
    */
    static fsScr* newScreen(fsID scr_id);


    /*!
        删除指定屏幕.
        @param[in] scr_id 屏幕 ID.
    */
    static void deleteScreen(fsID scr_id);


    /*!
        返回第一个屏幕如果不存在则返回 NULL.
        @return 第一个屏幕.
    */
    static fsScr* getFirstScreenN();


    /*!
        返回最后一个屏幕如果不存在则返回 NULL.
        @return 最后一个屏幕.
    */
    static fsScr* getLastScreenN();


    /*!
        判断指定的纹理是否存在.
        @param[in] tex_id 纹理 ID.
        @return 指定的纹理是否存在.
    */
    static bool hasTexture(fsID tex_id);


    /*!
        返回指定的纹理如果不存在则返回 NULL.
        @param[in] tex_id 纹理 ID.
        @return 指定的纹理.
    */
    static fsTex* getTexture(fsID tex_id);


    /*!
        创建新的指定 ID 的纹理.
        @param[in] tex_id 纹理 ID.
        @param[in] width 纹理宽度.
        @param[in] height 纹理高度.
        @param[in] format 纹理格式.
        @return 新建的纹理.
    */
    static fsTex* newTexture(fsID tex_id, u16 width, u16 height, fsTex::TexFormat format);


    /*!
        从指定的图像中创建新的纹理.
        @param[in] tex_id 纹理 ID.
        @param[in] width 纹理的宽度.
        @param[in] height 纹理的高度.
        @param[in] format 纹理格式.
        @param[in] image 图像的指针.
        @param[in] image_size 图像的大小.
        @return 新建的纹理.
    */
    static fsTex* newTexture(fsID tex_id, u16 width, u16 height, fsTex::TexFormat format, const void* image, u32 image_size);


    /*!
        删除指定的纹理.
        @param[in] tex_id 纹理 ID.
    */
    static void deleteTexture(fsID tex_id);


    /*!
        返回第一个纹理如果不存在则返回 NULL.
        @return 第一个纹理.
    */
    static fsTex* getFirstTextureN();


    /*!
        返回最后一个纹理如果不存在则返回 NULL.
        @return 最后一个纹理.
    */
    static fsTex* getLastTextureN();


    /*!
        返回指定字体的索引号.
        @param[in] font_id 字体 ID.
        @return 指定字体的索引号.
    */
    static u16 getFontIndexNum(fsID font_id);


    /*!
        返回当前字体的 ID.
        @return 当前字体的 ID.
    */
    static fsID getFontID();


    /*!
        返回当前字体的索引号.
        @return 当前字体的索引号.
    */
    static u16 getFontIndex();


    /*!
        设置字体和索引号.
        @param[in] font_id 字体 ID.
        @param[in] font_index 字体索引号.
    */
    static void setFont(fsID font_id, u16 font_index);


    /*!
        返回当前字体尺寸.
        @return 当前字体尺寸.
    */
    static u16 getFontSize();


    /*!
        设置当前字体尺寸.
        字体尺寸指的是字体在像素中的最大高度.
        @param[in] font_size 字体尺寸.
    */
    static u16 setFontSize(u16 font_size);


    /*!
        返回字体在渲染时的宽度.
        @param[in] str 字符串.
        @param[in] ... 参数.
    */
    static u16 calcFontDrawWidth(const char* str, ...);


    /*!
        返回字体在渲染时的宽度.
        @param[in] str 宽字节字符串.
        @param[in] ... 参数.
    */
    static u16 calcFontDrawWidth(const wchar_t* str, ...);


    /*!
        判断指定的渲染器是否存在.
        @param[in] shd_id 渲染器 ID.
        @return 指定的渲染器是否存在.
    */
    static bool hasShader(fsID shd_id);


    /*!
        返回指定的渲染器.
        @param[in] shd_id 渲染器 ID.
        @return 指定的渲染器.
    */
    static fsShd* getShader(fsID shd_id);


    /*!
        创建新的指定 ID 的着色器.
        @param[in] shd_id 着色器 ID.
        @param[in] vert_code 定点着色器可编程代码.
        @param[in] frag_code 片段着色器可编程代码.
        @param[in] uni_num 联合体数目.
        @param[in] att_num 属性数目.
        @param[in] tex_num 纹理数目.
        @return 着色器.
    */
    static fsShd* newShader(fsID shd_id, const char* vert_code, const char* frag_code, u8 uni_num, u8 att_num, u8 tex_num);


    /*!
        删除指定的着色器.
        @param[in] shd_id 着色器 ID.
    */
    static void deleteShader(fsID shd_id);


    /*!
        返回第一个着色器如果不存在则返回 NULL.
        @return 第一个着色器.
    */
    static fsShd* getFirstShaderN();


    /*!
        返回最后一个着色器如果不存在则返回 NULL.
        @return 最后一个着色器.
    */
    static fsShd* getLastShaderN();


    /*!
        判断指定的光线装置是否存在.
        @param[in] lts_id 光线装置 ID.
        @return 指定的光线装置是否存在.
    */
    static bool hasLightSet(fsID lts_id);


    /*!
        返回指定的光线装置.
        @param[in] lts_id 光线装置 ID.
        @return 指定的光线装置.
    */
    static fsLts* getLightSet(fsID lts_id);

    /*!
        创建新的制定 ID 的光线装置.
        @param[in] lts_id 光线装置 ID.
        @return 新建的光线装置.
    */
    static fsLts* newLightSet(fsID lts_id);


    /*!
        删除指定的光线装置.
        @param[in] lts_id 光线装置 ID.
    */
    static void deleteLightSet(fsID lts_id);


    /*!
        返回第一个光线装置如果不存在则返回 NULL.
        @return 第一个光线装置.
    */
    static fsLts* getFirstLightSetN();

    /*!
        返回最后一个光线装置如果不存在则返回 NULL.
        @return 最后一个光线装置.
    */
    static fsLts* getLastLightSetN();


    /*!
        删除所有纹理对象,该方法只被引擎自己调用.
    */
    static void deleteAllVramObjForEngine();


    /*!
        更新图帧缓存进行渲染,该方法只被引擎自己调用.
    */
    static void renderForEngine();


private:

    static const u32 SCREEN_HASH_SIZE = 10;
    static const u32 TEXTURE_HASH_SIZE = 10;
    static const u32 SHADER_HASH_SIZE = 10;
    static const u32 LIGHTSET_HASH_SIZE = 10;
    static const u32 FONT_NAME_BUFFER_SIZE = 64;

    fsDrawMgr();
    ~fsDrawMgr();
    void operator=(const fsDrawMgr&);

    static fsDrawMgr* instance();

    static void renderScreen(fsScr* scr, fsDraw** sort_list, const fsMat& view);
    static void sortList(fsDraw** sorted_start, fsDraw** sorted_end, fsDraw* target_list);

    static void textureInitializer(fsID id, fsStr<char, 3> ext, const void* data, u32 data_size, void** exinfo);
    static void textureFinalizer(fsID id, fsStr<char, 3> ext, const void* data, u32 data_size, void* exinfo);

    static void fontInitializer(fsID id, fsStr<char, 3> ext, const void* data, u32 data_size, void** exinfo);
    static void fontFinalizer(fsID id, fsStr<char, 3> ext, const void* data, u32 data_size, void* exinfo);

    fsMap<fsID, fsScr*> m_scr_map;
    fsMap<fsID, fsTex*> m_tex_map;
    fsMap<fsID, fsShd*> m_shd_map;
    fsMap<fsID, fsLts*> m_lts_map;

    bool m_is_render;
    fsID m_font_id;
    u16 m_font_index;
    u16 m_font_size;
    void* m_font_info;

    static fsDrawMgr* m_instance;
};


#endif /* FS_DRAW_MGR_H_ */
