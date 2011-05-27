/*
 * fs_rend.h
 *
 *  Created on: 2011-3-10
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */

#ifndef FS_REND_H_
#define FS_REND_H_


class fsPrim;


/*!
    @ingroup fsDraw
    用户自定义绘图类型的渲染类.
*/
class FS_API fsRend
{
    friend class fsPrim;

public:
    //! @cond
    fsDefineException(ExceptionInvalidArgument);
    fsDefineException(ExceptionInvalidCall);
    fsDefineException(ExceptionNotInitialized);
    //! @endcond

    /*!
        Destructs this renderer.
    */
    virtual ~fsRend();

    /*!
        Returns whether this renderer is active.
        @return Whether this renderer is active.
    */
    bool isActive() const;

    /*!
        Determines whether this renderer is active.
        @param[in] is_active Whether this renderer is active.
    */
    void setActive(bool is_active);

protected:
    //! @cond
    fsRend();
    void init(fsPrim* prim, u32 body_size, u32 data_size);
    void init(fsPrim* prim, u32 body_size, u32 data_size, void* rend_data);

    fsPrim* getPrim() const;

    template<class T> T* getRendBody()
    {
        if (!m_prim)
        {
            fsThrow(ExceptionNotInitialized);
        }

        return static_cast<T*>(m_rend_body);
    }

    template<class T> T* getRendData(u16 index)
    {
        if (!m_prim)
        {
            fsThrow(ExceptionNotInitialized);
        }

        if (index >= getMaxDataNum())
        {
            fsThrow(ExceptionInvalidArgument);
        }

        return static_cast<T*>(m_rend_data) + index;
    }

    static void renderAllocBuffer(fsVec** pos_buf, fsCol** col_buf, r32** uv_buf, fsVec** normal_buf, //
        const fsPrim* prim, bool has_pos_buf, bool has_col_buf, bool has_uv_buf, bool has_normal_buf);

    static const fsMat& renderGetPrimWorld(const fsPrim* prim);
    static fsTex* renderGetPrimTextureN(const fsPrim* prim);
    static fsCol renderGetPrimFinalColor(const fsPrim* prim);

    static bool renderIsTextureUVAdjustNeeded(fsTex* tex);
    static void renderGetTextureParam(r32* u_param_a, r32* u_param_b, r32* v_param_a, r32* v_param_b, const fsTex* tex);

    static void renderCalcColorBuffer(fsCol* col_buf, const fsPrim* prim);
    static void renderCalcUVBuffer(r32* uv_buf, const fsPrim* prim);

    static void renderSetTexture(fsTex* tex1, fsTex* tex2, fsTex* tex3, bool is_bilinear);
    static void renderSetVertexPointer(u32 stride, const r32* vert);
    static void renderSetColorPointer(u32 stride, const u8* color);
    static void renderSetTexCoordPointer(u32 stride, const r32* uv);

    static void renderSetTexture(const fsPrim* prim);
    static void renderSetVertexPointer(const fsPrim* prim);
    static void renderSetColorPointer(const fsPrim* prim);
    static void renderSetTexCoordPointer(const fsPrim* prim);

    static void renderDrawArrays(u8 prim_mode, u16 first, u16 count);

    static u32 renderGetShaderUniformLocation(const fsShd* shd, u8 index);
    static u32 renderGetShaderAttribLocation(const fsShd* shd, u8 index);
    static u32 renderGetShaderTextureLocation(const fsShd* shd, u8 index);
    static void renderSetShader(fsShd* shd);
    static void renderSetUniform_s32(u32 location, s32 uniform);
    static void renderSetUniform_r32(u32 location, r32 uniform);
    static void renderSetUniform_localToScreen(const fsShd* shd);
    static void renderSetAttribPointer_r32(u32 location, u8 size, u32 stride, const r32* attrib);
    static void renderSetAttribPointer_vertex(const fsShd* shd, u32 stride, const r32* vert);
    static void renderSetAttribPointer_color(const fsShd* shd, u32 stride, const u8* color);
    static void renderSetAttribPointer_texCoord(const fsShd* shd, u32 stride, const r32* uv);
    static void renderSetAttribPointer_vertex(const fsShd* shd, const fsPrim* prim);
    static void renderSetAttribPointer_color(const fsShd* shd, const fsPrim* prim);
    static void renderSetAttribPointer_texCoord(const fsShd* shd, const fsPrim* prim);
    static void renderDisableAttribPointers(const fsShd* shd);

    static void renderCallPrimRenderWithDestroyingBuffer(fsPrim* prim, const fsMat& view);
    //! @endcond

private:
    fsRend(const fsRend&);
    void operator=(const fsRend&);

    void init2(fsPrim* prim, u32 body_size, u32 data_size, bool is_share_data, void* rend_data);
    void uninit();

    u16 getMaxDataNum() const;

    void reallocData(u16 max_data_num);
    void copyData(u16 dest_index, const fsPrim* src_prim, u16 src_index);

    virtual fsID getClassID() = 0;
    virtual void initData(void* data, u16 data_num) = 0;
    virtual void render(const fsMat& view) = 0;

    fsType<u8, bool> m_is_active;
    u32 m_rend_body_size;
    u32 m_rend_data_size;
    fsPrim* m_prim;
    void* m_rend_body;
    void* m_rend_data;
};


#endif /* FS_REND_H_ */
