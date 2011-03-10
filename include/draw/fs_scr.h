/*
 * fs_scr.h
 *
 *  Created on: 2011-3-10
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */

#ifndef FS_SCR_H_
#define FS_SCR_H_


/*!
    @ingroup fsDraw
    A screen, which has a rectangle area and an order for drawing.
*/
class FS_API fsScr
{
    friend class fsDraw;
    friend class fsDrawMgr;

public:
    //! @cond
    fsDefineException(ExceptionInvalidArgument);
    fsDefineException(ExceptionInvalidCall);
    //! @endcond

    /*!
        The default focal length.
    */
    static const u32 DEFAULT_FOCUS_DIST = 1000;

    /*!
        The default distance of near clipping.
    */
    static const u32 DEFAULT_NEAR_CLIP_DIST = 10;

    /*!
        The default distance of far clipping.
    */
    static const u32 DEFAULT_FAR_CLIP_DIST = 100000;

    /*!
        The number of the guest screens.
    */
    static const u32 GUEST_SCREEN_NUM = 4;


    /*!
        Changes the order of this screen to the last.
    */
    void moveLast();


    /*!
        Sets the size of this screen in the framebuffer.
        @param[in] left The left position of a screen.
        @param[in] top The top position of a screen.
        @param[in] width The width of a screen.
        @param[in] height The height of a screen.
    */
    void setAreaInFramebuffer(s16 left, s16 top, u16 width, u16 height);


    /*!
        Sets the size of the view.
        @param[in] width The width of the view.
        @param[in] height The height of the view.
    */
    void setViewSize(r32 width, r32 height);


    /*!
        Returns whether this screen is active.
        @return Whether this screen is active.
    */
    bool isActive() const;

    /*!
        Determines whether this screen is active.
        @param[in] is_active Whether this screen is active.
    */
    void setActive(bool is_active);


    /*!
        Returns whether clears color when this screen is updated.
        @return Whether clears color when this screen is updated.
    */
    bool isClearColor() const;

    /*!
        Returns whether clears depthbuffer when this screen is updated.
        @return Whether clears depthbuffer when this screen is updated.
    */
    bool isClearDepth() const;

    /*!
        Sets the clear modes.
        @param[in] is_clear_color Whether clears color.
        @param[in] is_clear_depth Whether clears depthbuffer.
    */
    void setClearMode(bool is_clear_color, bool is_clear_depth);

    /*!
        Returns the clear color.
        @return The clear color.
    */
    fsCol getClearColor() const;

    /*!
        Sets the clear color.
        @param[in] col A clear color.
    */
    void setClearColor(fsCol col);

    /*!
        Returns whether perspective is on.
        @return Whether perspective is on.
    */
    bool isPerspective() const;

    /*!
        Determines whether pserspetive is on.
        @param[in] is_perspective Whether pserspetive is on.
    */
    void setPerspective(bool is_perspective);


    /*!
        Returns the focal length.
        @return The focal length.
    */
    r32 getFocusDist() const;

    /*!
        Sets the focal length.
        @param[in] focus_dist A focal length.
    */
    void setFocusDist(r32 focus_dist);

    /*!
        Returns the distance of near clipping.
        @return The distance of near clipping.
    */
    r32 getNearClipDist() const;

    /*!
        Returns the distance of far clipping.
        @return The distance of far clipping.
    */
    r32 getFarClipDist() const;

    /*!
        Sets the distances of clipping.
        @param[in] near The distance of near clipping.
        @param[in] far The distance of far clipping.
    */
    void setClipDist(r32 near, r32 far);

    /*!
        Returns the reference to the view of this screen.
        @return The reference to the view of this screen.
    */
    fsMat& view();


    /*!
        Deletes the texture of this screen.
    */
    void detachScreenTexture();


private:
    enum ScrFlag
    {
        FLAG_ACTIVE, //
        FLAG_CLEAR_COLOR, //
        FLAG_CLEAR_DEPTH, //
        FLAG_PERSPECTIVE, //
        FLAG_COPY_SCREEN
    };

    fsScr(fsID scr_id);
    ~fsScr();
    void operator=(const fsScr&);

    void calcProjection();
    void setupProjection();
    fsVec worldToClip_noCalcProjection(const fsVec& pos_in_world) const;
    bool canBoundClip_noCalcProjection(const fsMat& world, const fsVec& bound_max, const fsVec& bound_min) const;
    void copyScreenTexture();

    fsFlag<u8, ScrFlag> m_flag;
    fsID m_id;
    s16 m_left_in_framebuffer;
    s16 m_top_in_framebuffer;
    u16 m_width_in_framebuffer;
    u16 m_height_in_framebuffer;
    r32 m_view_width;
    r32 m_view_height;
    fsCol m_clear_col;
    r32 m_focus_dist;
    r32 m_near_clip_dist;
    r32 m_far_clip_dist;
    fsMat m_view;
    fsDraw m_root_draw;
    fsID m_guest_id[GUEST_SCREEN_NUM];
    r32 m_view_to_clip[16];
    r32 m_world_to_view[16];
    r32 m_world_to_clip[16];
};


#endif /* FS_SCR_H_ */
