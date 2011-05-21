/*
 * fs_tex.h
 *
 *  Created on: 2011-4-6
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */

#ifndef FS_TEX_H_
#define FS_TEX_H_


class fsRend;
class fsPrim;
class fsSprt;
class fsScr;
class fsDrawMgr;


/*!
    @ingroup fsDraw
    纹理, 绘图用的图像容器.
*/
class FS_API fsTex
{
    friend class fsRend;
    friend class fsPrim;
    friend class fsSprt;
    friend class fsScr;
    friend class fsDrawMgr;

public:
    //! @cond
    fsDefineException(ExceptionInvalidArgument);
    fsDefineException(ExceptionInvalidCall);
    fsDefineException(ExceptionVolatileTextureUploaded);
    fsDefineException(ExceptionEndEditImageNotCalled);
    //! @endcond

    /*!
        The texture formats.
    */
    enum TexFormat
    {
        FORMAT_RGB, //!< R8G8B8
        FORMAT_RGBA, //!< R8G8B8A8
        FORMAT_ALPHA, //!< A8

        FORMAT_PNG_RGB, //!< R8G8B8 from PNG
        FORMAT_PNG_RGBA, //!< R8G8B8A8 from PNG
        FORMAT_PNG_ALPHA //!< A8 from PNG
    };

    /*!
        The texture modes.
    */
    enum TexMode
    {
        MODE_READ_ONLY, //!< Read-only
        MODE_READ_WRITE, //!< Read and write.
        MODE_FRAMEBUFFER, //!< Copied from the framebuffer.
        MODE_VOLATILE //!< Vram only.
    };

    /*!
        Returns the previous texture. If the previous texture doesn't exist, returns NULL.
        @return The previous texture.
    */
    fsTex* getPrevN() const;

    /*!
        Returns the next texture. If the next texture doesn't exist, returns NULL.
        @return The next texture.
    */
    fsTex* getNextN() const;

    /*!
        Returns the texture ID.
        @return The texture ID.
    */
    fsID getID() const;

    /*!
        Returns the width of this texture.
        @return The width of this texture.
    */
    u16 getWidth() const;

    /*!
        Returns the height of this texture.
        @return The height of this texture.
    */
    u16 getHeight() const;

    /*!
        Returns the format of this texture.
        @return The format of this texture.
    */
    TexFormat getFormat() const;

    /*!
        Returns the texture mode.
        @return The texture mode.
    */
    TexMode getMode() const;

    /*!
        Returns the raw image of this texture as read-only.
        @return The raw image of this texture.
    */
    const void* getImage() const;

    /*!
        Returns the size of this texture.
        @return The size of this texture.
    */
    u32 getImageSize() const;

    /*!
        Starts editing the raw image of this texture.
        endEditImage must be called after editing.
        @return The raw image of this texture.
    */
    void* beginEditImage();

    /*!
        Ends editing the raw image and updates the whole area.
    */
    void endEditImage();

    /*!
        Ends editing the raw image and updates the specified area.
        @param[in] x The left position of the modified area.
        @param[in] y The top position of the modified area.
        @param[in] width The width of the modified area.
        @param[in] height The height of the modified area.
    */
    void endEditImage(u16 x, u16 y, u16 width, u16 height);

    /*!
        Sets the all pixels of this texture to 0.
        This method can't be called during editing the raw image.
    */
    void clearImage(fsCol col);

    /*!
        Changes the size of this texture.
        @param[in] width The width of a texture.
        @param[in] height The height of a texture.
    */
    void resizeImage(u16 width, u16 height);

    /*!
        Deletes the verbose images on the main memory.
    */
    void setVolatile();

    /*!
        Returns the proxy texture ID.
        @return The proxy texture ID.
    */
    fsID getProxyTextureID() const;

    /*!
        Sets the proxy texture.
        @param[in] tex_id A proxy texture ID.
    */
    void setProxyTextureID(fsID tex_id);

private:
    enum TexFlag
    {
        FLAG_EDIT, //
        FLAG_UPLOAD, //
        FLAG_UV_ADJUST
    };

    fsTex(fsID tex_id, u16 width, u16 height, TexFormat format, TexMode mode, const void* image, u32 image_size);
    ~fsTex();
    void operator=(const fsTex&);

    u32 getTexObj();
    void expandAndRegisterTexture_ctx();
    void expandAndRegisterTexture_png();

    fsID m_id;
    u16 m_width;
    u16 m_height;
    fsType<u8, TexFormat> m_format;
    fsType<u8, TexMode> m_mode;
    fsFlag<u8, TexFlag> m_flag;
    const void* m_image;
    u32 m_image_size;
    u32 m_tex_obj;
    r32 m_u_param_a;
    r32 m_u_param_b;
    r32 m_v_param_a;
    r32 m_v_param_b;
    fsTex* m_proxy_tex;
};


#endif /* FS_TEX_H_ */
