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
        The ID of the invisible screen.
    */
    static const fsID INVISIBLE_SCREEN_ID;

    /*!
        The ID of the default 3D screen.
    */
    static const fsID DEFAULT_3D_SCREEN_ID;

    /*!
        The ID of the defualt 2D screen.
    */
    static const fsID DEFAULT_2D_SCREEN_ID;

    /*!
        The ID of the default light set.
    */
    static const fsID DEFAULT_LIGHT_SET_ID;

    /*!
        The ID of the default no-texture shader.
    */
    static const fsID DEFAULT_NO_TEXTURE_SHADER_ID;

    /*!
        The ID of the default rgb-texture shader.
    */
    static const fsID DEFAULT_RGB_TEXTURE_SHADER_ID;

    /*!
        The ID of the default rgba-texture shader.
    */
    static const fsID DEFAULT_RGBA_TEXTURE_SHADER_ID;

    /*!
        The ID of the default alpha-texture shader
    */
    static const fsID DEFAULT_ALPHA_TEXTURE_SHADER_ID;

    /*!
        The maximum rendering interval time [msec].
    */
    static const u32 MAX_RENDER_INTERVAL_MSEC_TIME = 100;


    /*!
        Returns whether programmable shader is available.
        @return Whether programmable shader is available.
    */
    static bool isShaderAvailable();


    /*!
        Returns the maximum pixel length of a side of a texture.
        @return The maximum pixel length of a side of a texture.
    */
    static u16 getMaxTextureLength();


    /*!
        Returns the valid length of the specified texture length.
        @return The valid length of the specified texture length.
    */
    static u16 getValidTextureLength(u16 length);


    /*!
        Returns the byte size of the specified pixel format.
        @return The byte size of the specified pixel format.
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
        Returns whether rendering process is enabled.
        @return Whether rendering process is enabled.
    */
    static bool isRender();


    /*!
        Determines whether the rendering process is enabled.
        @param[in] is_render Whether the rendering process is enabled.
    */
    static void setRender(bool is_render);


    /*!
        Returns whether the specified screen exists.
        @param[in] scr_id A screen ID.
        @return Whether the specified screen exists.
    */
    static bool hasScreen(fsID scr_id);


    /*!
        Returns the specified screen. If the specified screen doesn't exist, returns NULL.
        The new screen is linked to the last of the screen list.
        @param[in] scr_id A screen ID.
        @return A screen.
    */
    static fsScr* getScreen(fsID scr_id);

    /*!
        Creates new screen which has the specified ID.
        @param[in] scr_id A screen ID.
        @return The instance of new screen.
    */
    static fsScr* newScreen(fsID scr_id);


    /*!
        Deletes the specified screen.
        @param[in] scr_id A screen ID.
    */
    static void deleteScreen(fsID scr_id);


    /*!
        Returns the first screen. If the first screen doesn't exist, returns NULL.
        @return The first screen.
    */
    static fsScr* getFirstScreenN();


    /*!
        Returns the last screen. If the last screen doesn't exist, return NULL.
        @return The last screen.
    */
    static fsScr* getLastScreenN();


    /*!
        Returns whether the specified texture exists.
        @param[in] tex_id A texture ID.
        @return Whether the specified texture exists.
    */
    static bool hasTexture(fsID tex_id);


    /*!
        Returns the specified texture. If the specified texture doesn't exist, returns NULL.
        @param[in] tex_id A texture ID.
        @return The specified texture.
    */
    static fsTex* getTexture(fsID tex_id);


    /*!
        Creates new texture which has the specified ID.
        @param[in] tex_id A texture ID.
        @param[in] width The width of a texture.
        @param[in] height The height of a texture.
        @param[in] format The format of a texture.
        @return The instance of new texture.
    */
    static fsTex* newTexture(fsID tex_id, u16 width, u16 height, fsTex::TexFormat format);


    /*!
        Creates new texture from the specified raw-image.
        @param[in] tex_id A texture ID.
        @param[in] width The width of a texture.
        @param[in] height The height of a texture.
        @param[in] format The format of a texture.
        @param[in] image The pointer of the raw-image.
        @param[in] image_size The size of the raw-image.
        @return The instance of new texture.
    */
    static fsTex* newTexture(fsID tex_id, u16 width, u16 height, fsTex::TexFormat format, const void* image, u32 image_size);


    /*!
        Deletes the specified texture.
        @param[in] tex_id A texture ID.
    */
    static void deleteTexture(fsID tex_id);


    /*!
        Returns the first texture. If the first texture doesn't exist, returns NULL.
        @return The first texture.
    */
    static fsTex* getFirstTextureN();


    /*!
        Returns the last texture. If the last texture doesn't exist, return NULL.
        @return The last texture.
    */
    static fsTex* getLastTextureN();


    /*!
        Returns the number of the indices of the specified font.
        @param[in] font_id A font ID.
        @return The number of the indices of the specified font.
    */
    static u16 getFontIndexNum(fsID font_id);


    /*!
        Returns the ID of the current font.
        @return The ID of the current font.
    */
    static fsID getFontID();


    /*!
        Returns the current font-index.
        @return The current font-index.
    */
    static u16 getFontIndex();


    /*!
        Sets the current font and font-index.
        @param[in] font_id A font ID.
        @param[in] font_index A font-index.
    */
    static void setFont(fsID font_id, u16 font_index);


    /*!
        Returns the current font size.
        @return The current font size.
    */
    static u16 getFontSize();


    /*!
        Sets the current font size.
        Font size represents the max height of the fonts in pixel.
        @param[in] font_size The size of font.
    */
    static u16 setFontSize(u16 font_size);


    /*!
        Returns the width of the specified string when rendered.
        @param[in] str A string.
        @param[in] ... Arguments.
    */
    static u16 calcFontDrawWidth(const char* str, ...);


    /*!
        Returns the width of the specified string when rendered.
        @param[in] str A string.
        @param[in] ... Arguments.
    */
    static u16 calcFontDrawWidth(const wchar_t* str, ...);


    /*!
        Returns whether the specified shader exists.
        @param[in] shd_id A shader ID.
        @return Whether the specified shader exists.
    */
    static bool hasShader(fsID shd_id);


    /*!
        Returns the shader.
        @param[in] shd_id A shader ID.
        @return A shader.
    */
    static fsShd* getShader(fsID shd_id);


    /*!
        Creates a new shader.
        @param[in] shd_id A shader ID.
        @param[in] vert_code Vertex shader code.
        @param[in] frag_code fragment shader code.
        @param[in] uni_num The number of an uniforms.
        @param[in] att_num The number of an attributes.
        @param[in] tex_num The number of textures.
        @return A shader.
    */
    static fsShd* newShader(fsID shd_id, const char* vert_code, const char* frag_code, u8 uni_num, u8 att_num, u8 tex_num);


    /*!
        Deletes the specified shader.
        @param[in] shd_id A shader ID.
    */
    static void deleteShader(fsID shd_id);


    /*!
        Returns the first shader. If the first texture doesn't exist, returns NULL.
        @return The first shader.
    */
    static fsShd* getFirstShaderN();


    /*!
        Returns the last shader. If the last texture doesn't exist, returns NULL.
        @return The last shader.
    */
    static fsShd* getLastShaderN();


    /*!
        Returns whether the specified light-set exits.
        @param[in] lts_id A light-set ID.
        @return Whether the specified light-set exits.
    */
    static bool hasLightSet(fsID lts_id);


    /*!
        Returns the specified light-set.
        @param[in] lts_id A light-set ID.
        @return The specified light-set.
    */
    static fsLts* getLightSet(fsID lts_id);

    /*!
        Creates a new light-set.
        @param[in] lts_id A light-set ID.
        @return A light-set.
    */
    static fsLts* newLightSet(fsID lts_id);


    /*!
        Deletes the specified light-set.
        @param[in] lts_id A light-set ID.
    */
    static void deleteLightSet(fsID lts_id);


    /*!
        Returns the first light-set. If the first light-set doesn't exist, returns NULL.
        @return The first light-set.
    */
    static fsLts* getFirstLightSetN();

    /*!
        Returns the last light-set. If the last light-set doesn't exist, returns NULL.
        @return The last light-set.
    */
    static fsLts* getLastLightSetN();


    /*!
        Deletes the all texture objects. This method is only for system.
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
