/*
 * fs_shd.h
 *
 *  Created on: 2011-4-6
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */

#ifndef FS_SHD_H_
#define FS_SHD_H_


/*!
    @ingroup fsDraw
    A programmable shader for drawing element primitives.
*/
class FS_API fsShd
{
    friend class fsRend;
    friend class fsPrim;
    friend class fsSprt;
    friend class fsDrawMgr;

public:
    /*!
        Returns the previous shader. If the previous shader doesn't exist, returns NULL.
        @return The previous shader.
    */
    fsShd* getPrevN() const;

    /*!
        Returns the next shader. If the next shader doesn't exist, returns NULL.
        @return The next shader.
    */
    fsShd* getNextN() const;

    /*!
        Returns the shader ID.
        @return The shader ID.
    */
    fsID getID() const;

    /*!
        Returns the number of the uniforms.
        @return The number of the uniforms.
    */
    u8 getUniformNum() const;

    /*!
        Returns the number of the attributes.
        @return The number of the attributes.
    */
    u8 getAttribNum() const;

    /*!
        Returns the number of the textures.
        @return The number of the textures.
    */
    u8 getTextureNum() const;

    /*!
        Returns whether the shader code is valid.
        @return Whether the shader code is valid.
    */
    bool isValid() const;

private:
    enum ShdFlag
    {
        FLAG_UPLOAD, //
        FLAG_COMPILE_ERROR
    };

    fsShd(fsID shd_id, const char* vert_code, const char* frag_code, u8 uni_num, u8 att_num, u8 tex_num);
    ~fsShd();
    void operator=(const fsShd&);

    u32 getShdObj();

    fsID m_id;
    char* m_vert_code;
    char* m_frag_code;
    u8 m_uni_num;
    u8 m_att_num;
    u8 m_tex_num;
    u32 m_shd_obj;
    u32 m_local_to_screen_loc;
    u32 m_vertex_loc;
    u32 m_color_loc;
    u32 m_texcoord_loc;
    u32* m_uni_loc_tbl;
    u32* m_att_loc_tbl;
    u32 m_tex_loc_tbl[3];
    fsFlag<u8, ShdFlag> m_flag;
};


#endif /* FS_SHD_H_ */
