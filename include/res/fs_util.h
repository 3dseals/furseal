/*
 * fs_util.h
 *
 *  Created on: 2011-3-9
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */

#ifndef FS_UTIL_H_
#define FS_UTIL_H_


/*!
    @ingroup fsRes
    资源管理器的实用工具.
*/
class FS_API fsUtil
{
public:
    //! @cond
    fsDefineException(ExceptionInvalidArgument);
    fsDefineException(ExceptionInvalidData);


    /*!
        Returns the length of the specified string.
        @param[in] str A string.
        @return The length of the specified string.
    */
    static u32 strlen(const char* str);


    /*!
        Returns the length of the specified wchar string.
        @param[in] str A wchar string.
        @return The length of the specified wchar string.
    */
    static u32 wcslen(const wchar_t* str);


    /*!
        Converts a string to a wchar string.
        @param[out] buf The output buffer.
        @param[in] buf_size The size of a buffer.
        @param[in] str A string.
    */
    static void charToWchar(wchar_t* buf, u32 buf_size, const char* str);


    /*!
        从文件路径名中返回文件名.
        @param[in] filename 文件路径名.
        @return 文件名.
    */
    static const char* getBasename(const char* filename);


    /*!
        从文件路径名中返回扩展名.
        @param[in] filename 文件路径名.
        @return 扩展名.
    */
    static const char* getExtension(const char* filename);


    /*!
        Calculates the normals as triangles from the specified vertices.
        @param[out] normal The calculated normals.
        @param[in] prim_data A primitive data.
        @param[in] vert_num The number of the vertices.
        @param[in] is_smoothing Whether smoothing is on.
    */
    static void calcNormalAsTriangles(fsVec* normal, const fsPrim::PrimData* prim_data, u16 vert_num, bool is_smoothing);

    /*!
        Reads the information of the png image.
        @param[out] width The width of an image.
        @param[out] height The height of an image.
        @param[out] format The pixel format.
        @param[in] data The data of A png image.
        @param[in] data_size The size of the data.
    */
    static bool readPNGInfo(u16* width, u16* height, fsTex::TexFormat* format, const void* data, u32 data_size);

    /*!
        Decodes the png image.
        @param[out] buf The destination buffer.
        @param[in] buf_size The size of a buffer.
        @param[in] buf_line_size The size of buffer per line.
        @param[in] data The data of A png image.
        @param[in] data_size The size of the data.
    */
    static bool readPNGImage(void* buf, u32 buf_size, u32 buf_line_size, const void* data, u32 data_size);

    /*!
        Loads windows font. This medhod is only for Windows.
        @param[in] filename The file name of a windows font.
    */
    static void loadWindowsFont(const char* filename);

    /*!
        Loads a shader.
        @param[in] shd_id The ID of a shader.
        @param[in] vert_file The file name of a vertex shader.
        @param[in] frag_file The file name of a fragment shader.
        @param[in] uni_num The number of the uniforms.
        @param[in] att_num The number of the attributes.
        @param[in] tex_num The number of the textures.
    */
    static void loadShader(fsID shd_id, const char* vert_file, const char* frag_file, u8 uni_num, u8 att_num, u8 tex_num);
	

    /*!
        Loads a pixel art model.
        @param[in] res_id The resource ID of a pixel art model.
        @param[in] ckl_file The file name of a configuration script.
        @param[in] png_file The file name of a png image.
        @param[in] scale The scaling factor of a pixel art model.
    */
    static void loadPixelArtModelAs(fsID res_id, const char* fsl_file, const char* png_file, r32 scale);

    /*!
        Imports a 3DS model.
        @param[in] filename The file name.
        @param[in] tex_id The texture ID.
        @param[in] has_normal Whether a 3DS model has normals.
        @param[in] is_smoothing_normal Whether smoothing is on.
        @param[in] scale The scaling factor of a 3DS model.
    */
    static void import3DSModel(const char* filename, fsID tex_id, bool has_normal, bool is_smoothing_normal, r32 scale);

    /*!
        Imports a 3DS model as the specified resource ID.
        @param[in] res_id The resource ID of a 3DS model.
        @param[in] filename The file name.
        @param[in] tex_id The texture ID.
        @param[in] has_normal Whether a 3DS model has normals.
        @param[in] is_smoothing_normal Whether smoothing is on.
        @param[in] scale The scaling factor of a 3DS model.
    */
    static void import3DSModelAs(fsID res_id, const char* filename, fsID tex_id, bool has_normal, bool is_smoothing_normal, r32 scale);
};


#endif /* FS_UTIL_H_ */
