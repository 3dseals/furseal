/*
 * fs_prim.h
 *
 *  Created on: 2011-3-10
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */

#ifndef FS_PRIM_H_
#define FS_PRIM_H_


/*!
    @ingroup fsDraw
    A drawing element primitive, which has several drawing ways.
*/
class FS_API fsPrim : public fsDraw
{
    friend class fsRend;

public:
    /*!
        Primitive data.
    */
    struct FS_API PrimData
    {
        fsVec pos; //!< Position.
        fsCol col; //!< Color.
        r32 u; //!< V value.
        r32 v; //!< U value.

        PrimData(); //!< Constructs a primitive data.
    };

    /*!
        Primitive modes.
    */
    enum PrimMode
    {
        MODE_POINTS, //!<
        MODE_LINES, //!<
        MODE_LINE_LOOP, //!<
        MODE_LINE_STRIP, //!<
        MODE_TRIANGLES, //!<
        MODE_TRIANGLE_STRIP, //!<
        MODE_TRIANGLE_FAN //!<
    };

    /*!
        Constructs a primitive.
    */
    fsPrim();

    /*!
        Destructs this primitive.
    */
    virtual ~fsPrim();

    /*!
        Initializes this primitive.
        @param[in] prim_mode A primitive mode.
        @param[in] max_data_num The maximum number of primitive data.
        @param[in] scr_id A screen ID.
    */
    void init(PrimMode prim_mode, u16 max_data_num, fsID scr_id);

    /*!
        Initializes this primitive.
        @param[in] prim_mode A primitive mode.
        @param[in] max_data_num The maximum number of primitive data.
        @param[in] parent A parent drawing element.
    */
    void init(PrimMode prim_mode, u16 max_data_num, fsDraw* parent);

    /*!
        Initializes this primitive.
        @param[in] prim_mode A primitive mode.
        @param[in] prim_data A shared primitive data.
        @param[in] max_data_num The maximum number of primitive data.
        @param[in] scr_id A screen ID.
    */
    void init(PrimMode prim_mode, PrimData* prim_data, u16 max_data_num, fsID scr_id);

    /*!
        Initializes this primitive.
        @param[in] prim_mode A primitive mode.
        @param[in] prim_data A shared primitive data.
        @param[in] max_data_num The maximum number of primitive data.
        @param[in] parent A parent drawing element.
    */
    void init(PrimMode prim_mode, PrimData* prim_data, u16 max_data_num, fsDraw* parent);

    /*!
        Returns the primitive mode.
        @return The primitive mode.
    */
    PrimMode getPrimMode() const;

    /*!
        Sets the primitive mode.
        @param[in] prim_mode A primitive mode.
    */
    void setPrimMode(PrimMode prim_mode);

    /*!
        Returns whether shering primitive data.
        @return Whether shering primitive data.
    */
    bool isShareData() const;

    /*!
        Returns the number of used primitive data.
        @return The number of used primitive data.
    */
    u16 getCurDataNum() const;

    /*!
        Sets the number of used primitive data.
        @param[in] cur_data_num The number of used primitive data.
    */
    void setCurDataNum(u16 cur_data_num);

    /*!
        Returns the maximum number of the primitive data.
    */
    u16 getMaxDataNum() const;

    /*!
        Changes the maximum number of the primitive data.
    */
    void reallocData(u16 max_data_num);

    /*!
        Copies the specified primitive data.
        @param[in] dest_index The index of destination primitive data.
        @param[in] src_prim A source primitive.
        @param[in] src_index The index of souce primitive data.
    */
    void copyData(u16 dest_index, const fsPrim* src_prim, u16 src_index);

    /*!
        Returns the reference to the position of the specified primitive data.
        @param[in] index The index of a primitive data.
        @return The reference to the position of the specified primitive data.
    */
    fsVec& dataPos(u16 index);

    /*!
        Returns the reference to the color of the specified primitive data.
        @param[in] index The index of a primitive data.
        @return The reference to the color of the specified primitive data.
    */
    fsCol& dataCol(u16 index);

    /*!
        Returns the reference to the u value of the specified primitive data.
        @param[in] index The index of a primitive data.
        @return The reference to the u value of the specified primitive data.
    */
    r32& dataU(u16 index);

    /*!
        Returns the reference to the v value of the specified primitive data.
        @param[in] index The index of a primitive data.
        @return The reference to the v value of the specified primitive data.
    */
    r32& dataV(u16 index);

    /*!
        Sets the uv values of the specified primitive data.
        @param[in] index The index of a primitive data.
        @param[in] u An u value.
        @param[in] v A V value.
    */
    void setDataUV(u16 index, r32 u, r32 v);

    /*!
        Sets the primitive data as a rectange. The prim mode must be MODE_TRIANGLE_FAN.
        @param[in] index The first index of a primitive data.
        @param[in] center The center of a rectangle.
        @param[in] width The width of a rectangle.
        @param[in] height The height of a rectangle.
        @param[in] col The color of a rectangle.
        @param[in] u1 The u value of the left-top vertex of a rectangle.
        @param[in] v1 The v value of the left-top vertex of a rectangle.
        @param[in] u2 The u value of the right-bottom vertex of a rectangle.
        @param[in] v2 The v value of the right-bottom vertex of a rectangle.
    */
    void setDataRect(u16 index, const fsVec& center, r32 width, r32 height, fsCol col, r32 u1, r32 v1, r32 u2, r32 v2);

private:
    void init2(PrimMode prim_mode, bool is_share_data, PrimData* prim_data, u16 max_data_num, fsID scr_id, fsDraw* parent);

    virtual void render(const fsMat& view);
    void render_soft(const fsMat& view);
    void render_shader(const fsMat& view);

    fsType<u8, PrimMode> m_prim_mode;
    fsType<u8, bool> m_is_share_data;
    PrimData* m_prim_data;
    u16 m_cur_data_num;
    u16 m_max_data_num;
    fsRend* m_rend;
};


#endif /* FS_PRIM_H_ */
