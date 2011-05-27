/*
 * fs_lit.h
 *
 *  Created on: 2011-3-10
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */

#ifndef FS_LIT_H_
#define FS_LIT_H_


class fsLts;


/*!
    @ingroup fsDraw
    点光线.
*/
class FS_API fsLit
{
    friend class fsLts;

public:
    //! @cond
    fsDefineException(ExceptionInvalidArgument);
    //! @endcond

    /*!
        Returns the previous point light. If the previous point light doesn't exist, returns NULL.
        @return The previous point light.
    */
    fsLit* getPrevN() const;

    /*!
        Returns the next point light. If the next point light doesn't exist, returns NULL.
        @return The next point light.
    */
    fsLit* getNextN() const;

    /*!
        Returns the ID of this point light.
        @return The ID of this point light.
    */
    fsID getID() const;

    /*!
        Returns whether this point light is active.
        @return Whether this point light is active.
    */
    bool isActive() const;

    /*!
        Determines whether this point light is active.
        @param[in] is_active Whether this point light is active.
    */
    void setActive(bool is_active);

    /*!
        Returns the position of this point light.
        @return The position of this point light.
    */
    const fsVec& getPos() const;

    /*!
        Sets the position of this point light.
        @param[in] pos A position.
    */
    void setPos(const fsVec& pos);

    /*!
        Returns the inner radius of this point light.
        @return The inner radius of this point light.
    */
    r32 getInnerRadius() const;

    /*!
        Returns the outer radius of this point light.
        @return The outer radius of this point light.
    */
    r32 getOuterRadius() const;

    /*!
        Sets the radii of this point light.
        @param[in] inner_rad The inner radius.
        @param[in] outer_rad The outer radius.
    */
    void setRadius(r32 inner_rad, r32 outer_rad);

    /*!
        Returns the color of this point light.
        @return The color of this point light.
    */
    fsCol getColor() const;

    /*!
        Sets the color of this point light.
        @param[in] col A color.
    */
    void setColor(fsCol col);


private:
    fsLit(fsID lts_id);
    void operator=(const fsLit&);

    void updateBound();

    fsList<fsLit>::Item m_item;
    fsID m_id;
    fsType<u8, bool> m_is_active;
    fsVec m_pos;
    r32 m_inner_rad;
    r32 m_outer_rad;
    fsCol m_col;
    u16 m_col_int;
    fsVec m_min_bound;
    fsVec m_max_bound;
    r32 m_sq_inner_rad;
    r32 m_sq_outer_rad;
};


#endif /* FS_LIT_H_ */
