/*
 * fs_mdl_data.h
 *
 *  Created on: 2011-5-27
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */


#ifndef FS_MDL_DATA_H_
#define FS_MDL_DATA_H_


/*!
    @ingroup fsDraw
    A 3D Model data, which is used by 3D models.
*/
class FS_API fsMdlData
{
public:
    //! @cond
    fsDefineException(ExceptionInvalidArgument);
    fsDefineException(ExceptionInvalidCall);
    fsDefineException(ExceptionInvalidData);
    fsDefineException(ExceptionNotInitialized);
    //! @endcond

    /*!
        The version number of the model data format.
    */
    static const u32 MODEL_DATA_VERSION = 50; // 0.5.0

    /*!
        The modes of the model data.
    */
    enum ModelDataMode
    {
        MODE_READ, //!< Read-only mode.
        MODE_WRITE //!< Read and write mode.
    };

    /*!
        Constructs a model data.
    */
    fsMdlData();

    /*!
        Destructs this model data.
    */
    ~fsMdlData();

    /*!
        Initializes this model data as read-only mode.
        @param[in] data Data.
        @param[in] data_size The size of data.
    */
    void initAsReader(const void* data, u32 data_size);

    /*!
        Initializes this model data as read-and-write mode.
        @param[in] node_num The number of the nodes.
        @param[in] vert_num The number of the verteces.
        @param[in] tex_id A texture ID.
        @param[in] has_normal Whether this model data has normals.
    */
    void initAsWriter(u16 node_num, u16 vert_num, fsID tex_id, bool has_normal);

    /*!
        Returns the model data mode.
        @return The model data mode.
    */
    ModelDataMode getMode() const;

    /*!
        Returns the number of the nodes.
        @return The number of the nodes.
    */
    u16 getNodeNum() const;

    /*!
        Returns the number of the vertices.
        @return The number of the vertices.
    */
    u16 getVertNum() const;

    /*!
        Returns whether this model data has normals.
        @return Whether this model data has normals.
    */
    bool hasNormal() const;

    /*!
        Returns the texture ID.
        @return The texture ID.
    */
    fsID getTextureID() const;

    /*!
        Returns the index of the parent node of the specified node.
        @param[in] node_index The index of a node.
    */
    u16 getNodeParentIndex(u16 node_index) const;

    /*!
        Sets the index of the parent node of the specified node.
        @param[in] node_index The index of a node.
        @param[in] parent_index The index of the parent node.
    */
    void setNodeParentIndex(u16 node_index, u16 parent_index);

    /*!
        Lets the specified node have no parent.
        @param[in] node_index The index of a node.
    */
    void setNodeParentIndex_noParent(u16 node_index);

    /*!
        Returns the local matrix of the specified node.
        @param[in] node_index The index of a node.
        @return The local matrix of the specified node.
    */
    const fsMat& getNodeLocal(u16 node_index) const;

    /*!
        Sets the local matrix of the specified node.
        @param[in] node_index The index of a node.
        @param[in] local A local matrix.
    */
    void setNodeLocal(u16 node_index, const fsMat& local);

    /*!
        Returns the prim mode of the specified node.
        @param[in] node_index The index of a node.
        @return The prim mode of the specified node.
    */
    fsPrim::PrimMode getNodePrimMode(u16 node_index) const;

    /*!
        Sets the prim mode of the specified node.
        @param[in] node_index The index of a node.
        @param[in] prim_mode A prim mode.
    */
    void setNodePrimMode(u16 node_index, fsPrim::PrimMode prim_mode);

    /*!
        Returns the first index of the vertices of the specified node.
        @param[in] node_index The index of a node.
        @return The first index of the vertices of the specified node.
    */
    u16 getNodeVertIndex(u16 node_index) const;

    /*!
        Returns the number of the vertices of the specified node.
        @param[in] node_index The index of a node.
        @return The number of the vertices of the specified node.
    */
    u16 getNodeVertNum(u16 node_index) const;

    /*!
        Sets the vertex information of the specified node.
        @param[in] node_index The index of a node.
        @param[in] vert_index The index of a vertex.
        @param[in] vert_num The number of the vertices.
    */
    void setNodeVertInfo(u16 node_index, u16 vert_index, u16 vert_num);

    /*!
        Returns the prim data of the specified node.
        @param[in] node_index The index of a node.
        @return The prim data of the specified node.
    */
    const fsPrim::PrimData* getNodePrimData(u16 node_index) const;

    /*!
        Returns the normal data of the specified node.
        @param[in] node_index The index of a node.
        @return The normal data of the specified node.
    */
    const fsVec* getNodeNormalData(u16 node_index) const;

    /*!
        Returns the blend mode of the specified node.
        @param[in] node_index The index of a node.
        @return The blend mode of the specified node.
    */
    fsPrim::BlendMode getNodeBlendMode(u16 node_index) const;

    /*!
        Sets the blend mode of the specified node.
        @param[in] node_index The index of a node.
        @param[in] blend_mode A blend mode.
    */
    void setNodeBlendMode(u16 node_index, fsPrim::BlendMode blend_mode);

    /*!
        Returns the minimum coordinates of the bounding box of the specified node.
        @param[in] node_index The index of a node.
        @return The minimum coordinates of the bounding box of the specified node.
    */
    const fsVec& getNodeClipBoundMinForReader(u16 node_index) const;

    /*!
        Returns the maximum coordinates of the bounding box of the specified node.
        @param[in] node_index The index of a node.
        @return The maximum coordinates of the bounding box of the specified node.
    */
    const fsVec& getNodeClipBoundMaxForReader(u16 node_index) const;

    /*!
        Returns the position of the specified vertex.
        @param[in] vert_index The index of a vertex.
        @return The position of the specified vertex.
    */
    const fsVec& getVertPos(u16 vert_index) const;

    /*!
        Sets the position of the specified vertex.
        @param[in] vert_index The index of a vertex.
        @param[in] pos The position of a vertex.
    */
    void setVertPos(u16 vert_index, const fsVec& pos);

    /*!
        Returns the color of the specified vertex.
        @param[in] vert_index The index of a vertex.
        @return The color of the specified vertex.
    */
    fsCol getVertCol(u16 vert_index) const;

    /*!
        Sets the color of the specified vertex.
        @param[in] vert_index The index of a vertex.
        @param[in] col The color of a vertex.
    */
    void setVertCol(u16 vert_index, fsCol col);

    /*!
        Returns the u value of the specified vertex.
        @param[in] vert_index The index of a vertex.
        @return The u value of the specified vertex.
    */
    r32 getVertU(u16 vert_index) const;

    /*!
        Returns the v value of the specified vertex.
        @param[in] vert_index The index of a vertex.
        @return The v value of the specified vertex.
    */
    r32 getVertV(u16 vert_index) const;

    /*!
        Sets the uv value of the specified vertex.
        @param[in] vert_index The index of a vertex.
        @param[in] u An u value.
        @param[in] v An v value.
    */
    void setVertUV(u16 vert_index, r32 u, r32 v);

    /*!
        Returns the normal of the specified vertex.
        @param[in] vert_index The index of a vertex.
        @return The normal of the specified vertex.
    */
    const fsVec& getVertN(u16 vert_index) const;

    /*!
        Sets the normal of the specified vertex.
        @param[in] vert_index The index of a vertex.
        @param[in] n a normal.
    */
    void setVertN(u16 vert_index, const fsVec& n);

    /*!
        Calculates the normals as triangles.
        @param[in] is_smoothing If true, the normals are smoothed.
    */
    void calcNormalAsTriangles(bool is_smoothing);

    /*!
        Registers this model data as a resource.
        @param[in] res_id The ID of the resource.
    */
    void registerAsResource(fsID res_id);

private:
    enum ModelDataFlag
    {
        FLAG_HAS_NORMAL
    };

    struct ModelDataHeader
    {
        fsID format_id;
        u16 format_version;
        u16 node_num;
        u16 vert_num;
        fsFlag<u16, ModelDataFlag> flag;
        fsID tex_id;
        u32 prim_data_offset;
        u32 normal_data_offset;
    };

    struct NodeInfo
    {
        fsMat local;
        fsVec bound_min;
        fsVec bound_max;
        u16 parent_index;
        fsType<u8, fsPrim::PrimMode> prim_mode;
        fsType<u8, fsPrim::BlendMode> blend_mode;
        u16 vert_index;
        u16 vert_num;
    };

    fsMdlData(const fsMdlData&);
    void operator=(const fsMdlData&);

    fsType<u8, ModelDataMode> m_mode;
    u8* m_data;
    u32 m_data_size;
    ModelDataHeader* m_mdl_data_header;
    NodeInfo* m_node_info;
    fsPrim::PrimData* m_prim_data;
    fsVec* m_normal_data;
};


#endif /* FS_MDL_DATA_H_ */
