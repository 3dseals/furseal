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
    A light set, which manages point lights, parallel lights, and ambient lights.
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
