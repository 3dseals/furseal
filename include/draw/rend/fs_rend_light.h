/*
 * fs_rend_light.h
 *
 *  Created on: 2011-5-27
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */


#ifndef FS_REND_LIGHT_H_
#define FS_REND_LIGHT_H_


/*!
    @ingroup fsDraw
    A custom renderer for lighting.
*/
class FS_API fsRend_Light : public fsRend
{
public:
    /*!
        A renderer data
    */
    struct RendData
    {
        fsVec normal; //!< The normal.
    };

    /*!
        Initializes this renderer.
        @param[in] prim A target primitive.
        @param[in] lts_id A light-set ID.
    */
    void init(fsPrim* prim, fsID lts_id);

    /*!
        Initializes this renderer.
        @param[in] prim A target primitive.
        @param[in] rend_data Shared renderer data.
        @param[in] lts_id A light-set ID.
    */
    void init(fsPrim* prim, RendData* rend_data, fsID lts_id);

    /*!
        Returns the light-set ID.
        @return The light-set ID.
    */
    fsID getLightSetID();

    /*!
        Sets the light-set ID.
        @param[in] lts_id A light-set ID.
    */
    void setLightSetID(fsID lts_id);

    /*!
        Returns the reference to the specified renderer data.
        @param[in] index The index of a renderer data.
        @return The reference to the specified renderer data.
    */
    fsVec& dataN(u16 index);

    /*!
        Calculates the normals as triangles.
        @param[in] is_smoothing If true, the normals are smoothed.
    */
    void calcNormalAsTriangles(bool is_smoothing);

private:
    struct RendBody
    {
        fsLts* lts;
    };

    virtual fsID getClassID();
    virtual void initData(void* data, u16 data_num);
    virtual void render(const fsMat& view);
    void render_soft(const fsMat& view);
    void render_shader(const fsMat& view);
};


#endif /* FS_REND_LIGHT_H_ */
