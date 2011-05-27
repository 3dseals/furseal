/*
 * fs_mdl.h
 *
 *  Created on: 2011-5-27
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */


#ifndef FS_MDL_H_
#define FS_MDL_H_


class fsRend_Light;


/*!
    @ingroup fsDraw
    A 3D model, which consists of drawing element nodes.
*/
class FS_API fsMdl
{
public:
    //! @cond
    fsDefineException(ExceptionInvalidArgument);
    fsDefineException(ExceptionNotInitialized);
    //! @endcond

    /*!
        Constructs a model.
    */
    fsMdl();

    /*!
        Destructs this model.
    */
    ~fsMdl();

    /*!
        Initializes this model.
        @param[in] mdl_data_id The ID of a model data.
        @param[in] scr_id The ID of a screen.
    */
    void init(fsID mdl_data_id, fsID scr_id);

    /*!
        Initializes this model.
        @param[in] mdl_data_id The ID of a model data.
        @param[in] parent The ID of a drawing object.
    */
    void init(fsID mdl_data_id, fsDraw* parent);

    /*!
        Returns the model data.
        @return The model data.
    */
    const fsMdlData* getModelData() const;

    /*!
        Returns the texture ID.
        @return The texture ID.
    */
    fsID getTextureID() const;

    /*!
        Sets the texture ID.
        @param[in] tex_id A texture ID.
    */
    void setTextureID(fsID tex_id);

    /*!
        Returns the ID of the light set.
        @return The ID of the light set.
    */
    fsID getLightSetID() const;

    /*!
        Sets the ID of the light set.
        @param[in] lts_id The ID of the light set.
    */
    void setLightSetID(fsID lts_id);

    /*!
        Returns the root drawing object.
        @return The root drawing object.
    */
    fsDraw* getRootDraw();

    /*!
        Returns the drawing object of the specified node.
        @param[in] node_index The index of a node.
        @return The drawing object of the specified node.
    */
    fsDraw* getNodeDraw(u16 node_index);

private:
    fsMdl(const fsMdl&);
    void operator=(const fsMdl&);

    void init2(fsID mdl_data_id, fsID scr_id, fsDraw* parent);
    void uninit();

    fsMdlData m_mdl_data;
    fsDraw** m_draw_ptr;
    fsRend_Light** m_rend_ptr;
    fsNode m_root_node;
    u16 m_node_num;
    fsID m_lts_id;
};


#endif /* FS_MDL_H_ */
