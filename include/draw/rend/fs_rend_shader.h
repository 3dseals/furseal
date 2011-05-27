/*
 * fs_rend_shader.h
 *
 *  Created on: 2011-5-27
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */


#ifndef FS_REND_SHADER_H_
#define FS_REND_SHADER_H_


/*!
    @ingroup fsDraw
    A custom renderer for using programmable shaders.
*/
class FS_API fsRend_Shader : public fsRend
{
public:
    /*!
        Initializes this renderer.
        @param[in] prim A target primitive.
        @param[in] shd_id A shader ID.
    */
    void init(fsPrim* prim, fsID shd_id);

    /*!
        Initializes this renderer.
        @param[in] prim A target primitive.
        @param[in] rend_data Shared renderer data.
        @param[in] shd_id A shader ID.
    */
    void init(fsPrim* prim, r32* rend_data, fsID shd_id);

    /*!
        Returns the shader ID.
        @return The shader ID.
    */
    fsID getShaderID();

    /*!
        Returns the second texture ID.
        @return The second texture ID.
    */
    fsID get2ndTextureID();

    /*!
        Sets the second texture ID.
        @param[in] tex_id A texture ID.
    */
    void set2ndTextureID(fsID tex_id);

    /*!
        Returns the third texture ID.
        @return The third texture ID.
    */
    fsID get3rdTextureID();

    /*!
        Sets the third texture ID.
        @param[in] tex_id A texture ID.
    */
    void set3rdTextureID(fsID tex_id);

    /*!
        Returns the reference to the specified uniform.
        @param[in] uni_index The index of an uniform.
        @return The reference to the specified uniform.
    */
    r32& uniform(u8 uni_index);

    /*!
        Sets the specified uniforms as a vector.
        @param[in] uni_index the first index of uniforms.
        @param[in] vec A vector.
    */
    void setUniformVec(u8 uni_index, const fsVec& vec);

    /*!
        Sets the specified uniforms as a 3-element color.
        @param[in] uni_index the first index of uniforms.
        @param[in] col A color.
    */
    void setUniformCol3(u8 uni_index, fsCol col);

    /*!
        Sets the specified uniforms as a 4-element color.
        @param[in] uni_index The first index of uniforms.
        @param[in] col A color.
    */
    void setUniformCol4(u8 uni_index, fsCol col);

    /*!
        Returns the reference to the specified attribute.
        @param[in] data_index The index of renderer data.
        @param[in] att_index The index of an attribute.
        @return The reference to the specified attribute.
    */
    r32& attrib(u16 data_index, u8 att_index);

    /*!
        Sets the specified attributes as a vector.
        @param[in] data_index The index of renderer data.
        @param[in] att_index The first index of attributes.
        @param[in] vec A vector.
    */
    void setAttribVec(u16 data_index, u8 att_index, const fsVec& vec);

    /*!
        Sets the specified attributes as a 3-element color.
        @param[in] data_index The index of renderer data.
        @param[in] att_index The first index of attributes.
        @param[in] col A color.
    */
    void setAttribCol3(u16 data_index, u8 att_index, fsCol col);

    /*!
        Sets the specified attributes as a 4-element color.
        @param[in] data_index The index of renderer data.
        @param[in] att_index The first index of attributes.
        @param[in] col A color.
    */
    void setAttribCol4(u16 data_index, u8 att_index, fsCol col);

private:
    struct RendBody
    {
        fsShd* shd;
        fsTex* tex2;
        fsTex* tex3;
        r32* uni;
        r32* att;
        u32 rend_data_size;
    };

    virtual fsID getClassID();
    virtual void initData(void* data, u16 data_num);
    virtual void render(const fsMat& view);
};


#endif /* FS_REND_SHADER_H_ */
