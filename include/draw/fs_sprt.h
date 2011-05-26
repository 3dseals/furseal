/*
 * fs_sprt.h
 *
 *  Created on: 2011-5-26
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */

#ifndef FS_SPRT_H_
#define FS_SPRT_H_


/*!
    @ingroup fsDraw
    A sprite, a rectangle which always faces the camera.
*/
class FS_API fsSprt : public fsDraw
{
public:
    /*!
        Sprite data.
    */
    struct FS_API SprtData
    {
        fsVec pos; //!< Position.
        fsCol col; //!< Color.
        r32 w; //!< Width.
        r32 h; //!< Height.
        r32 u1; //!< The u value of the left-top vertex.
        r32 v1; //!< The v value of the left-top vertex.
        r32 u2; //!< The u value of the right-bottom vertex.
        r32 v2; //!< The v value of the right-bottom vertex.
        s32 ang; //!< Z-rotation.

        SprtData(); //!< Constructs a sprite data.
    };

    /*!
        Constructs a sprite.
    */
    fsSprt();

    /*!
        Destructs this sprite.
    */
    virtual ~fsSprt();

    /*!
        Initializes this sprite.
        @param[in] max_data_num The maximum number of primitive data.
        @param[in] scr_id A screen ID.
    */
    void init(u16 max_data_num, fsID scr_id);

    /*!
        Initializes this sprite.
        @param[in] max_data_num The maximum number of primitive data.
        @param[in] parent A parent drawing element.
    */
    void init(u16 max_data_num, fsDraw* parent);

    /*!
        Initializes this sprite.
        @param[in] sprt_data A shared sprite data.
        @param[in] max_data_num The maximum number of primitive data.
        @param[in] scr_id A screen ID.
    */
    void init(SprtData* sprt_data, u16 max_data_num, fsID scr_id);

    /*!
        Initializes this sprite.
        @param[in] sprt_data A shared sprite data.
        @param[in] max_data_num The maximum number of primitive data.
        @param[in] parent A parent drawing element.
    */
    void init(SprtData* sprt_data, u16 max_data_num, fsDraw* parent);

    /*!
        Returns whether shering sprite data.
        @return Whether shering sprite data.
    */
    bool isShareData() const;

    /*!
        Returns the number of used sprite data.
        @return The number of used sprite data.
    */
    u16 getCurDataNum() const;

    /*!
        Sets the number of used sprite data.
        @param[in] cur_data_num The number of used sprite data.
    */
    void setCurDataNum(u16 cur_data_num);

    /*!
        Returns the maximum number of the sprite data.
    */
    u16 getMaxDataNum() const;

    /*!
        Changes the maximum number of the sprite data.
    */
    void reallocData(u16 max_data_num);

    /*!
        Copies the specified sprite data.
        @param[in] dest_index The index of destination sprite data.
        @param[in] src_sprt A source primitive.
        @param[in] src_index The index of souce primitive data. 
    */
    void copyData(u16 dest_index, const fsSprt* src_sprt, u16 src_index);

    /*!
        Returns the reference to the position of the specified sprite data.
        @param[in] index The index of a sprite data.
        @return The reference to the position of the specified sprite data.
    */
    fsVec& dataPos(u16 index);

    /*!
        Returns the reference to the color of the specified sprite data.
        @param[in] index The index of a sprite data.
        @return The reference to the color of the specified sprite data.
    */
    fsCol& dataCol(u16 index);

    /*!
        Returns the reference to the width of the specified sprite data.
        @param[in] index The index of a sprite data.
        @return The reference to the width of the specified sprite data.
    */
    r32& dataW(u16 index);

    /*!
        Returns the reference to the height of the specified sprite data.
        @param[in] index The index of a sprite data.
        @return The reference to the height of the specified sprite data.
    */
    r32& dataH(u16 index);

    /*!
        Returns the reference to the u value of the left-top vertex of the specified sprite data.
        @param[in] index The index of a sprite data.
        @return The reference to the u value of the left-top vertex of the specified sprite data.
    */
    r32& dataU1(u16 index);

    /*!
        Returns the reference to the v value of the left-top vertex of the specified sprite data.
        @param[in] index The index of a sprite data.
        @return The reference to the v value of the left-top vertex of the specified sprite data.
    */
    r32& dataV1(u16 index);

    /*!
        Returns the reference to the u value of the right-bottom vertex of the specified sprite data.
        @param[in] index The index of a sprite data.
        @return The reference to the u value of the right-bottom vertex of the specified sprite data.
    */
    r32& dataU2(u16 index);

    /*!
        Returns the reference to the v value of the right-bottom vertex of the specified sprite data.
        @param[in] index The index of a sprite data.
        @return The reference to the v value of the right-bottom vertex of the specified sprite data.
    */
    r32& dataV2(u16 index);

    /*!
        Returns the reference to the angle of the specified sprite data.
        @param[in] index The index of a sprite data.
        @return The reference to the angle of the specified sprite data.
    */
    s32& dataAng(u16 index);

    /*!
        Sets the size of the specified sprite data.
        @param[in] index The index of a sprite data.
        @param[in] width The width of a aprite data.
        @param[in] height The height of a aprite data.
    */
    void setDataSize(u16 index, r32 width, r32 height);

    /*!
        Sets the uv-value of the specified sprite data.
        @param[in] index The index of a sprite data.
        @param[in] u1 The u value of the left-top vertex of a sprite data.
        @param[in] v1 The v value of the left-top vertex of a sprite data.
        @param[in] u2 The u value of the right-bottom vertex of a sprite data.
        @param[in] v2 The v value of the right-bottom vertex of a sprite data.
    */
    void setDataUV(u16 index, r32 u1, r32 v1, r32 u2, r32 v2);

private:
    void init2(bool is_share_data, SprtData* sprt_data, u16 max_data_num, fsID scr_id, fsDraw* parent);

    virtual void render(const fsMat& view);
    void render_soft(const fsMat& view);
    void render_shader(const fsMat& view);

    fsType<u8, bool> m_is_share_data;
    SprtData* m_sprt_data;
    u16 m_cur_data_num;
    u16 m_max_data_num;
};

#endif /* FS_SPRT_H_ */
