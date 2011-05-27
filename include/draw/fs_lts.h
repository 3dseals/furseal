/*
 * fs_lts.h
 *
 *  Created on: 2011-3-10
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */


#ifndef FS_LTS_H_
#define FS_LTS_H_


/*!
    @ingroup fsDraw
    光线装置, 管理点光线, 直线光线和环境光线.
*/
class FS_API fsLts
{
    friend class fsDrawMgr;

public:
    //! @cond
    fsDefineException(ExceptionInvalidArgument);
    fsDefineException(ExceptionNotFound);
    fsDefineException(ExceptionSameIDExists);


    /*!
        The maximum number of parallel lights.
    */
    static const u32 MAX_PARA_LIGHT_NUM = 3;

    /*!
        The muximum number of valid lights.
    */
    static const u32 MAX_NEAR_LIGHT_NUM = 3;

    /*!
        Returns the previous light set.
        @return The previous light set.
    */
    fsLts* getPrevN() const;

    /*!
        Returns the next light set.
        @return The next light set.
    */
    fsLts* getNextN() const;

    /*!
        Returns the ID of this light set.
        @return The ID of this light set.
    */
    fsID getID() const;

    /*!
        Returns the ambient color.
        @return The ambient color.
    */
    fsCol getAmbientColor() const;

    /*!
        Sets the ambient color.
        @param[in] col A color.
    */
    void setAmbientColor(fsCol col);

    /*!
        Returns whether the specified parallel light is active.
        @param[in] index The index of a parallel light.
        @return Whether the specified parallel light is active.
    */
    bool isParaLightActive(u8 index) const;

    /*!
        Determines whether the specified parallel light is active.
        @param[in] index The index of a parallel light.
        @param[in] is_active Whether a parallel light is active.
    */
    void setParaLightActive(u8 index, bool is_active);

    /*!
        Returns the direction of the specified parallel light.
        @param[in] index The index of a parallel light.
        @return The direction of the specified parallel light.
    */
    const fsVec& getParaLightDir(u8 index) const;

    /*!
        Sets the direction of the specified parallel light.
        @param[in] index The index of a parallel light.
        @param[in] dir The direction of a parallel light.
    */
    void setParaLightDir(u8 index, const fsVec& dir);

    /*!
        Returns the color of the specified parallel light.
        @param[in] index The index of a parallel light.
        @return The color of the specified parallel light.
    */
    fsCol getParaLightColor(u8 index) const;

    /*!
        Sets the color of the specified parallel light.
        @param[in] index The index of a parallel light.
        @param[in] col The color of a parallel light.
    */
    void setParaLightColor(u8 index, fsCol col);

    /*!
        Returns the number of point lights.
        @return The number of point lights.
    */
    u32 getPointLightNum() const;

    /*!
        Returns the specified point light.
        @param[in] lit_id The ID of a point light.
        @return The specified point light.
    */
    fsLit* getPointLight(fsID lit_id);

    /*!
        Creates a point light.
        @param[in] lit_id The ID of a point light.
        @return A point light.
    */
    fsLit* newPointLight(fsID lit_id);

    /*!
        Deletes the specified point light.
        @param[in] lit_id The ID of a point light.
    */
    void deletePointLight(fsID lit_id);

    /*!
        Returns the first point light. If the first point light doesn't exist, returns NULL.
        @return The first point light.
    */
    fsLit* getFirstPointLightN() const;

    /*!
        Returns the last point light. If the last point light doesn't exist, returns NULL.
        @return The last point light.
    */
    fsLit* getLastPointLightN() const;

    /*!
        Finds the nearest lights of the specified position.
        @param[in] pos A position.
    */
    void findNearLight(const fsVec& pos);

    /*!
        Returns the number of the nearest lights.
    */
    u8 getNearLightNum() const;

    /*!
        Returns the direction of the specified nearest light.
        @param[in] index The index of a nearest light.
        @return The direction of the specified nearest light.
    */
    const fsVec& getNearLightDir(u8 index) const;

    /*!
        Returns the color of the specified nearest light.
        @param[in] index The index of a nearest light.
        @return The color of the specified nearest light.
    */
    fsCol getNearLightColor(u8 index) const;

    /*!
        Removes the all point lights.
    */
    void clearPointLight();

private:
    struct ParaLight
    {
        fsType<u8, bool> is_active;
        fsVec dir;
        fsCol col;
        u16 col_int;
    };

    struct NearLight
    {
        fsVec dir;
        fsCol col;
        u16 col_int;
    };

    fsLts(fsID lts_id);
    ~fsLts();
    void operator=(const fsLts&);

    fsLit* getPointLightN(fsID lit_id);

    void addLightToNearLight(const fsVec& dir, fsCol col, u16 col_int);

    fsID m_id;
    fsCol m_amb_col;
    ParaLight m_para_lit[MAX_PARA_LIGHT_NUM];
    u8 m_near_lit_num;
    u8 m_near_lit_index_tbl[MAX_NEAR_LIGHT_NUM];
    NearLight m_near_lit[MAX_NEAR_LIGHT_NUM];
    fsList<fsLit> m_lit_list;
};


#endif /* FS_LTS_H_ */
