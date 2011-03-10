/*
 * fs_draw.h
 *
 *  Created on: 2011-3-10
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */

#ifndef FS_DRAW_H_
#define FS_DRAW_H_


/*!
    @ingroup fsDraw
    A drawing element, the base class of the drawing elements.
*/
class FS_API fsDraw
{
    friend class fsScr;
    friend class fsDrawMgr;

public:
    //! @cond
    fsDefineException(ExceptionInvalidArgument);
    fsDefineException(ExceptionInvalidCall);
    fsDefineException(ExceptionNotInitialized);
    //! @endcond

    /*!
        The types of the drawable-objects.
    */
    enum DrawType
    {
        TYPE_NODE, //!< The node drawable-object.
        TYPE_PRIM, //!< The primitive drawable-object.
        TYPE_SPRT //!< The sprite drawable-object.
    };

    /*!
        The types of the depth tests.
    */
    enum DepthTest
    {
        DEPTH_TEST_ALWAYS, //!< Always passes.
        DEPTH_TEST_LESS, //!< Passes if the incoming depth value is less than the stored value.
        DEPTH_TEST_GREATER, //!< Passes if the incoming depth value is greater than the stored value.
        DEPTH_TEST_LEQUAL, //!< Passes if the incoming depth value is less than or equal to the stored value.
        DEPTH_TEST_GEQUAL //!< Passes if the incoming depth value is greater than or equal to the stored value.
    };

    /*!
        The types of the alpha-blending mode.
    */
    enum BlendMode
    {
        BLEND_OFF, //!< (Rs, Gs, Bs, As)
        BLEND_HALF, //!< (Rs, Gs, Bs, As) * As + (Rd, Gd, Bd, Ad) * (1 - As)
        BLEND_ADD, //!< (Rs, Gs, Bs, As) + (Rd, Gd, Bd, Ad)
        BLEND_DEST_ALPHA //!< (Rs, Gs, Bs, As) * Ad + (Rd, Gd, Bd, Ad) * (1 - Ad)
    };

    /*!
        The flags which specify the drawing attributes.
    */
    enum DrawFlag
    {
        FLAG_BOUND_CLIP, //!< Enables bound-clipping.
        FLAG_SORT, //!< Sorts the drawing object by the distance from the viewpoint.
        FLAG_WRITE_RGB, //!< Writes RGB components to the framebuffer.
        FLAG_WRITE_ALPHA, //!< Writes an alpha components to the framebuffer.
        FLAG_WRITE_DEPTH, //!< Writes a depth value to the depth-buffer.
        FLAG_BAfsFACE_CULLING, //!< Enables bafsface-culling.
        FLAG_BILINEAR //!< Enables bilinear filter.
    };

    /*!
        Destructs this fsDraw.
    */
    virtual ~fsDraw();


    /*!
        Returns whether this ckDraw belongs to a screen directly.
        @return Whether this ckDraw belongs to a screen directly.
    */
    bool hasScreen() const;


    /*!
        Returns the ID of the screen which this ckDraw belongs to directly.
        If this ckDraw belongs to no screen, an exception occurs.
        @return The ID of the screen which this ckDraw belongs to directly.
    */
    fsID getScreenID() const;


    /*!
        Makes this ckDraw belong to the specified screen.
        @param[in] scr_id The id of a screen.
    */
    void setScreenID(fsID scr_id);


    /*!
        Returns whether this ckDraw has a parent ckDraw.
        @return Whether this ckDraw has a parent ckDraw.
    */
    bool hasParent() const;


    /*!
        Returns the parent ckDraw of this ckDraw.
        If this ckDraw has no parent ckDraw, returns NULL.
        @return The parent ckDraw of this ckDraw.
    */
    fsDraw* getParentN() const;


    /*!
        Sets the parent ckDraw of this ckDraw.
        This ckDraw belongs to the specified ckDraw as the last child.
        @param[in] parent A parent ckDraw.
    */
    void setParent(fsDraw* parent);


    /*!
        Returns whether this fsDraw has a child fsDraw.
        @return Whether this fsDraw has a child fsDraw.
    */
    bool hasChild() const;


    /*!
        Returns the first child fsDraw of this fsDraw.
        If this fsDraw has no child fsDraw, returns NULL.
        @return The first child fsDraw of this fsDraw.
    */
    fsDraw* getFirstChildN() const;


    /*!
        Returns whether this fsDraw is visible.
        @return Whether this fsDraw is visible.
    */
    bool isVisible() const;


    /*!
        Determines whether this fsDraw is visible.
        @param[in] is_visible Whether this fsDraw is visible.
    */
    void setVisible(bool is_visible);


    /*!
        Returns the type of depth test.
        @return The type of depth test.
    */
    DepthTest getDepthTest() const;


    /*!
        Sets the type of depth test.
        @param[in] depth_test The type of depth test.
    */
    void setDepthTest(DepthTest depth_test);


    /*!
        Returns the type of blend mode.
        @return The type of blend mode.
    */
    BlendMode getBlendMode() const;


    /*!
        Sets the type of blend mode.
        @param[in] blend_mode The type of blend mode.
        @param[in] is_preset_setting If true, sets other draw flags and the type of depth test automatically.
    */
    void setBlendMode(BlendMode blend_mode, bool is_preset_setting);


    /*!
        Returns whether the specified draw flag is on.
        @param[in] draw_flag A draw flag.
        @return Whether the specified draw flag is on.
    */
    bool isDrawFlag(DrawFlag draw_flag) const;


    /*!
        Sets the specified draw flag to the specified value.
        @param[in] draw_flag A draw flag.
        @param[in] is_on If true, the flag is set on. Otherwise, the flag is set off.
    */
    void setDrawFlag(DrawFlag draw_flag, bool is_on);


protected:
    //! @cond
    enum PrivateFlag
    {
        FLAG_INITIALIZED, //
        FLAG_VISIBLE
    };

    fsDraw();

    void setupWorld();
    void setupFinalColor();
    void setupSortValue(const fsMat& view);
    void setupDrawState();

    virtual void render(const fsMat& view);

    fsTree<fsDraw> m_tree;
    fsType<u8, DrawType> m_type;
    fsType<u8, DepthTest> m_depth_test;
    fsType<u8, BlendMode> m_blend_mode;
    fsFlag<u8, PrivateFlag> m_private_flag;
    fsFlag<u16, DrawFlag> m_draw_flag;
    fsID m_scr_id;
    fsMat m_local;
    fsMat m_world;
    fsCol m_local_col;
    fsCol m_final_col;
    fsVec m_bound_min;
    fsVec m_bound_max;
    fsVec m_sort_center;
    r32 m_sort_offset;
    r32 m_sort_value;
    fsDraw* m_next_sort;
    //! @endcond

private:
    fsDraw(const fsDraw&);
    void operator=(const fsDraw&);
};


#endif /* FS_DRAW_H_ */
