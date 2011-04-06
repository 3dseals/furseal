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
    A font texture, the texture specialized for drawing font.
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
        Constructs this font texture.
    */
    fsFont();

    /*!
        Destructs this font texture.
    */
    ~fsFont();

    /*!
        Initializes this font texture.
        @param[in] width The width of this font texture.
        @param[in] height The width of this font texture.
    */
    void init(u16 width, u16 height);

    /*!
        Returns the ID of this font texture.
        @return The ID of this font texture.
    */
    fsID getTextureID() const;

    /*!
        Returns the width of this font texture.
        @return The width of this font texture.
    */
    u16 getWidth() const;

    /*!
        Returns the height of this font texture.
        @return The height of this font texture.
    */
    u16 getHeight() const;

    /*!
        Returns the raw image of this font texture as read-only.
        @return The raw image of this font texture.
    */
    const void* getImage() const;

    /*!
        Returns the size of the raw image of this font texture.
        @return The size of the raw image of this font texture.
    */
    u32 getImageSize() const;

    /*!
        Starts editing the raw image of this font texture.
        endEditImage must be called after editing.
        @return The raw image of this font texture.
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
        Sets the all pixels of this font texture to 0.
        beginEditImage must be called before this method.
    */
    void clearImage();

    /*!
        Renders the specified string.
        @param[in] x The x-coordinate of drawing position.
        @param[in] y The y-coordinate of drawing position.
        @param[in] str A string.
        @param[in] ... Arguments.
    */
    u16 drawString(s16 x, s16 y, const char* str, ...);

    /*!
        Renders the specified string.
        @param[in] x The x-coordinate of drawing position.
        @param[in] y The y-coordinate of drawing position.
        @param[in] str A string.
        @param[in] ... Arguments.
    */
    u16 drawString(s16 x, s16 y, const wchar_t* str, ...);

private:
    fsTex* m_tex;
};


#endif /* FS_FONT_H_ */
