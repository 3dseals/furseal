/*
 * fs_low_level_draw_opengl.cpp
 *
 *  Created on: 2011-3-10
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */


#if defined(WINDOWS)
#include <windows.h>
#include <GL/gl.h>
#include <glext.h>
#elif defined(FS_LINUX)
#include <GL/glx.h>
#elif defined(FS_MACOSX)
#include <GLUT/glut.h>
#elif defined(FS_IPHONE)
#include <OpenGLES/ES1/gl.h>
#elif defined(FS_ANDROID)
#include <GLES/gl.h>
#else
#error
#endif

#include "base/fs_low_level_api.h"

#include "fs_gen_all.h" // for fsMat::MulR32x16


#if !defined(FS_GLES1) && (defined(FS_IPHONE) || defined(FS_ANDROID))
#define FS_GLES1
#endif

#if !defined(FS_GLSL) && !defined(FS_GLES1)
#define FS_GLSL
#endif


#if defined(WINDOWS) || defined(FS_LINUX)

PFNGLACTIVETEXTUREARBPROC fsglActiveTexture;
PFNGLATTACHOBJECTARBPROC fsglAttachShader;
PFNGLCOMPILESHADERARBPROC fsglCompileShader;
PFNGLCREATEPROGRAMOBJECTARBPROC fsglCreateProgram;
PFNGLCREATESHADEROBJECTARBPROC fsglCreateShader;
PFNGLDELETEOBJECTARBPROC fsglDeleteObject;
PFNGLDISABLEVERTEXATTRIBARRAYARBPROC fsglDisableVertexAttribArray;
PFNGLENABLEVERTEXATTRIBARRAYARBPROC fsglEnableVertexAttribArray;
PFNGLGETATTRIBLOCATIONARBPROC fsglGetAttribLocation;
PFNGLGETINFOLOGARBPROC fsglGetInfoLog;
PFNGLGETOBJECTPARAMETERIVARBPROC fsglGetObjectParameteriv;
PFNGLGETUNIFORMLOCATIONARBPROC fsglGetUniformLocation;
PFNGLLINKPROGRAMARBPROC fsglLinkProgram;
PFNGLSHADERSOURCEARBPROC fsglShaderSource;
PFNGLUNIFORM1FARBPROC fsglUniform1f;
PFNGLUNIFORM1IARBPROC fsglUniform1i;
PFNGLUNIFORMMATRIX4FVARBPROC fsglUniformMatrix4fv;
PFNGLUSEPROGRAMOBJECTARBPROC fsglUseProgram;
PFNGLVERTEXATTRIBPOINTERARBPROC fsglVertexAttribPointer;

#define fsglDeleteProgram fsglDeleteObject
#define fsglDeleteShader fsglDeleteObject
#define fsglGetProgramInfoLog fsglGetInfoLog
#define fsglGetProgramiv fsglGetObjectParameteriv
#define fsglGetShaderInfoLog fsglGetInfoLog
#define fsglGetShaderiv fsglGetObjectParameteriv

#define FSGL_COMPILE_STATUS GL_OBJECT_COMPILE_STATUS_ARB
#define FSGL_FRAGMENT_SHADER GL_FRAGMENT_SHADER_ARB
#define FSGL_INFO_LOG_LENGTH GL_OBJECT_INFO_LOG_LENGTH_ARB
#define FSGL_LINK_STATUS GL_OBJECT_LINK_STATUS_ARB
#define FSGL_TEXTURE0 GL_TEXTURE0_ARB
#define FSGL_TEXTURE1 GL_TEXTURE1_ARB
#define FSGL_TEXTURE2 GL_TEXTURE2_ARB
#define FSGL_VERTEX_SHADER GL_VERTEX_SHADER_ARB

#else // !(WINDOWS || FS_LINUX)

#define fsglActiveTexture glActiveTexture
#define fsglAttachShader glAttachShader
#define fsglCompileShader glCompileShader
#define fsglCreateProgram glCreateProgram
#define fsglCreateShader glCreateShader
#define fsglDeleteProgram glDeleteProgram
#define fsglDeleteShader glDeleteShader
#define fsglDisableVertexAttribArray glDisableVertexAttribArray
#define fsglEnableVertexAttribArray glEnableVertexAttribArray
#define fsglGetAttribLocation glGetAttribLocation
#define fsglGetProgramInfoLog glGetProgramInfoLog
#define fsglGetProgramiv glGetProgramiv
#define fsglGetShaderInfoLog glGetShaderInfoLog
#define fsglGetShaderiv glGetShaderiv
#define fsglGetUniformLocation glGetUniformLocation
#define fsglLinkProgram glLinkProgram
#define fsglShaderSource glShaderSource
#define fsglUniform1f glUniform1f
#define fsglUniform1i glUniform1i
#define fsglUniformMatrix4fv glUniformMatrix4fv
#define fsglUseProgram glUseProgram
#define fsglVertexAttribPointer glVertexAttribPointer

#define FSGL_COMPILE_STATUS GL_COMPILE_STATUS
#define FSGL_FRAGMENT_SHADER GL_FRAGMENT_SHADER
#define FSGL_INFO_LOG_LENGTH GL_INFO_LOG_LENGTH
#define FSGL_LINK_STATUS GL_LINK_STATUS
#define FSGL_TEXTURE0 GL_TEXTURE0
#define FSGL_TEXTURE1 GL_TEXTURE1
#define FSGL_TEXTURE2 GL_TEXTURE2
#define FSGL_VERTEX_SHADER GL_VERTEX_SHADER

#endif // WINDOWS || FS_LINUX


static bool s_is_shader_available = false;

static fsLowLevelAPI::DepthTest s_depth_test;
static fsLowLevelAPI::BlendMode s_blend_mode;
static bool s_is_draw_rgb, s_is_draw_alpha, s_is_draw_depth;
static bool s_is_backface_culling;
static u32 s_tex_obj[3];
static bool s_is_bilinear[3];
static u32 s_shd_obj;

#ifdef FS_GLSL
static r32 s_model_view[16];
static r32 s_projection[16];
#endif

#ifndef FS_GLES2
static bool s_is_vertex_array_state;
static bool s_is_color_array_state;
static bool s_is_texcoord_array_state;
#endif


static void resetDepthTest()
{
    s_depth_test = fsLowLevelAPI::DEPTH_TEST_ALWAYS;

    fsLowLevelAPI::setDepthTest(fsLowLevelAPI::DEPTH_TEST_GEQUAL);
}


static void resetBlendMode()
{
    s_blend_mode = fsLowLevelAPI::BLEND_HALF;

    fsLowLevelAPI::setBlendMode(fsLowLevelAPI::BLEND_OFF);
}


static void resetWriteMode()
{
    s_is_draw_rgb = false;
    s_is_draw_alpha = true;
    s_is_draw_depth = false;

    fsLowLevelAPI::setWriteMode(true, false, true);
}


static void resetBackfaceCulling()
{
    s_is_backface_culling = true;

    fsLowLevelAPI::setBackfaceCulling(false);
}


static void resetTexture()
{
    s_tex_obj[0] = s_tex_obj[1] = s_tex_obj[2] = 1;
    s_is_bilinear[0] = s_is_bilinear[1] = s_is_bilinear[2] = true;

    fsLowLevelAPI::setTexture(0, 0, 0, false);
}


static void resetShader()
{
    s_shd_obj = 1;

    fsLowLevelAPI::setShader(0);
}


#ifndef FS_GLES2

static void setVertexArrayState(bool is_vertex_array_state)
{
    if (s_is_vertex_array_state != is_vertex_array_state)
    {
        s_is_vertex_array_state = is_vertex_array_state;

        if (s_is_vertex_array_state)
        {
            glEnableClientState(GL_VERTEX_ARRAY);
        }
        else
        {
            glDisableClientState(GL_VERTEX_ARRAY);
        }
    }
}


static void setColorArrayState(bool is_color_array_state)
{
    if (s_is_color_array_state != is_color_array_state)
    {
        s_is_color_array_state = is_color_array_state;

        if (s_is_color_array_state)
        {
            glEnableClientState(GL_COLOR_ARRAY);
        }
        else
        {
            glDisableClientState(GL_COLOR_ARRAY);
        }
    }
}


static void setTexCoordArrayState(bool is_texcoord_array_state)
{
    if (s_is_texcoord_array_state != is_texcoord_array_state)
    {
        s_is_texcoord_array_state = is_texcoord_array_state;

        if (s_is_texcoord_array_state)
        {
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        }
        else
        {
            glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        }
    }
}


static void resetVertexArrayState()
{
    s_is_vertex_array_state = true;

    setVertexArrayState(false);
}


static void resetColorArrayState()
{
    s_is_color_array_state = true;

    setColorArrayState(false);
}


static void resetTexCoordArrayState()
{
    s_is_texcoord_array_state = true;

    setTexCoordArrayState(false);
}

#endif // !FS_GLES2


void fsLowLevelAPI::setupShaderAPI(bool is_shader_check)
{
    if (!is_shader_check)
    {
        s_is_shader_available = false;
        return;
    }

#if defined(WINDOWS) || defined(FS_LINUX)

#ifdef WINDOWS
#define fsglGetProcAddress(x) wglGetProcAddress(x)
#else // FS_LINUX
#define fsglGetProcAddress(x) (*glXGetProcAddress)((const GLubyte*)(x))
#endif

    fsglActiveTexture = reinterpret_cast<PFNGLACTIVETEXTUREARBPROC>(fsglGetProcAddress("glActiveTextureARB"));
    fsglAttachShader = reinterpret_cast<PFNGLATTACHOBJECTARBPROC>(fsglGetProcAddress("glAttachObjectARB"));
    fsglCompileShader = reinterpret_cast<PFNGLCOMPILESHADERARBPROC>(fsglGetProcAddress("glCompileShaderARB"));
    fsglCreateProgram = reinterpret_cast<PFNGLCREATEPROGRAMOBJECTARBPROC>(fsglGetProcAddress("glCreateProgramObjectARB"));
    fsglCreateShader = reinterpret_cast<PFNGLCREATESHADEROBJECTARBPROC>(fsglGetProcAddress("glCreateShaderObjectARB"));
    fsglDeleteObject = reinterpret_cast<PFNGLDELETEOBJECTARBPROC>(fsglGetProcAddress("glDeleteObjectARB"));
    fsglDisableVertexAttribArray = reinterpret_cast<PFNGLDISABLEVERTEXATTRIBARRAYARBPROC>(fsglGetProcAddress("glDisableVertexAttribArrayARB"));
    fsglEnableVertexAttribArray = reinterpret_cast<PFNGLENABLEVERTEXATTRIBARRAYARBPROC>(fsglGetProcAddress("glEnableVertexAttribArrayARB"));
    fsglGetAttribLocation = reinterpret_cast<PFNGLGETATTRIBLOCATIONARBPROC>(fsglGetProcAddress("glGetAttribLocationARB"));
    fsglGetInfoLog = reinterpret_cast<PFNGLGETINFOLOGARBPROC>(fsglGetProcAddress("glGetInfoLogARB"));
    fsglGetObjectParameteriv = reinterpret_cast<PFNGLGETOBJECTPARAMETERIVARBPROC>(fsglGetProcAddress("glGetObjectParameterivARB"));
    fsglGetUniformLocation = reinterpret_cast<PFNGLGETUNIFORMLOCATIONARBPROC>(fsglGetProcAddress("glGetUniformLocationARB"));
    fsglLinkProgram = reinterpret_cast<PFNGLLINKPROGRAMARBPROC>(fsglGetProcAddress("glLinkProgramARB"));
    fsglShaderSource = reinterpret_cast<PFNGLSHADERSOURCEARBPROC>(fsglGetProcAddress("glShaderSourceARB"));
    fsglUniform1f = reinterpret_cast<PFNGLUNIFORM1FARBPROC>(fsglGetProcAddress("glUniform1fARB"));
    fsglUniform1i = reinterpret_cast<PFNGLUNIFORM1IARBPROC>(fsglGetProcAddress("glUniform1iARB"));
    fsglUniformMatrix4fv = reinterpret_cast<PFNGLUNIFORMMATRIX4FVARBPROC>(fsglGetProcAddress("glUniformMatrix4fvARB"));
    fsglUseProgram = reinterpret_cast<PFNGLUSEPROGRAMOBJECTARBPROC>(fsglGetProcAddress("glUseProgramObjectARB"));
    fsglVertexAttribPointer = reinterpret_cast<PFNGLVERTEXATTRIBPOINTERARBPROC>(fsglGetProcAddress("glVertexAttribPointerARB"));

    if (fsglActiveTexture && //
        fsglAttachShader && //
        fsglCompileShader && //
        fsglCreateProgram && //
        fsglCreateShader && //
        fsglDeleteObject && //
        fsglDisableVertexAttribArray && //
        fsglEnableVertexAttribArray && //
        fsglGetAttribLocation && //
        fsglGetInfoLog && //
        fsglGetObjectParameteriv && //
        fsglGetUniformLocation && //
        fsglLinkProgram && //
        fsglShaderSource && //
        fsglUniform1f && //
        fsglUniform1i && //
        fsglUniformMatrix4fv && //
        fsglUseProgram && //
        fsglVertexAttribPointer)
    {
        s_is_shader_available = true;
    }
    else
    {
        s_is_shader_available = false;
    }

#elif defined(FS_GLSL)

    s_is_shader_available = true;

#else // !FS_GLSL

    s_is_shader_available = false;

#endif // WINDOWS || FS_LINUX
}


void fsLowLevelAPI::resetDrawState()
{
    glEnable(GL_SCISSOR_TEST);
    glEnable(GL_DEPTH_TEST);

#ifndef FS_GLES2
    glShadeModel(GL_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
#endif

#if defined(FS_GLES1) || defined(FS_GLES2)
    glClearDepthf(-1.0f);
#else
    glClearDepth(-1.0f);
#endif

    resetDepthTest();
    resetBlendMode();
    resetWriteMode();
    resetBackfaceCulling();
    resetTexture();
    resetShader();

#ifndef FS_GLES2
    resetVertexArrayState();
    resetColorArrayState();
    resetTexCoordArrayState();
#endif
}


bool fsLowLevelAPI::isShaderAvailable()
{
    return s_is_shader_available;
}


void fsLowLevelAPI::setViewport(s16 left, s16 top, u16 width, u16 height)
{
    s16 bottom = getFramebufferHeight() - (top + height);

    glViewport(left, bottom, width, height);
    glScissor(left, bottom, width, height);
}


void fsLowLevelAPI::clearFramebuffer(bool is_clear_color, bool is_clear_depth, const u8* clear_color)
{
    GLbitfield mask = 0;

    if (is_clear_color)
    {
        glClearColor( //
            static_cast<r32>(clear_color[0]) / 255.0f, //
            static_cast<r32>(clear_color[1]) / 255.0f, //
            static_cast<r32>(clear_color[2]) / 255.0f, //
            static_cast<r32>(clear_color[3]) / 255.0f);

        mask |= GL_COLOR_BUFFER_BIT;
    }

    if (is_clear_depth)
    {
        mask |= GL_DEPTH_BUFFER_BIT;
    }

    setWriteMode(is_clear_color, is_clear_color, is_clear_depth);

    glClear(mask);
}


void fsLowLevelAPI::setDepthTest(DepthTest depth_test)
{
    if (s_depth_test != depth_test)
    {
        s_depth_test = depth_test;

        static GLenum gl_depth_test[] =
        {
            GL_ALWAYS, //
            GL_LESS, //
            GL_GREATER, //
            GL_LEQUAL, //
            GL_GEQUAL
        };

        glDepthFunc(gl_depth_test[s_depth_test]);
    }
}


void fsLowLevelAPI::setBlendMode(BlendMode blend_mode)
{
    if (s_blend_mode != blend_mode)
    {
        s_blend_mode = blend_mode;

        switch (s_blend_mode)
        {
        case BLEND_OFF:
            glDisable(GL_BLEND);
            break;

        case BLEND_HALF:
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            break;

        case BLEND_ADD:
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE);
            break;

        case BLEND_DEST_ALPHA:
            glEnable(GL_BLEND);
            glBlendFunc(GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA);
            break;
        }
    }
}


void fsLowLevelAPI::setWriteMode(bool is_draw_rgb, bool is_draw_alpha, bool is_draw_depth)
{
    if (s_is_draw_rgb != is_draw_rgb || s_is_draw_alpha != is_draw_alpha)
    {
        s_is_draw_rgb = is_draw_rgb;
        s_is_draw_alpha = is_draw_alpha;

        glColorMask(s_is_draw_rgb, s_is_draw_rgb, s_is_draw_rgb, s_is_draw_alpha);
    }

    if (s_is_draw_depth != is_draw_depth)
    {
        s_is_draw_depth = is_draw_depth;

        glDepthMask(s_is_draw_depth);
    }
}


void fsLowLevelAPI::setBackfaceCulling(bool is_backface_culling)
{
    if (s_is_backface_culling != is_backface_culling)
    {
        s_is_backface_culling = is_backface_culling;

        if (s_is_backface_culling)
        {
            glEnable(GL_CULL_FACE);
        }
        else
        {
            glDisable(GL_CULL_FACE);
        }
    }
}


u16 fsLowLevelAPI::getMaxTextureLength()
{
    return 2048;
}


u16 fsLowLevelAPI::getValidTextureLength(u16 length)
{
    if (length >= getMaxTextureLength())
    {
        return getMaxTextureLength();
    }

    u16 valid_length = 8;

    while (length > valid_length)
    {
        valid_length *= 2;
    }

    return valid_length;
}


u16 fsLowLevelAPI::getTexturePixelSize(TextureFormat format)
{
    static const u16 s_pixel_size[] =
    {
        3, 4, 1
    };

    return s_pixel_size[format];
}


u32 fsLowLevelAPI::registerTexture(u16 width, u16 height, TextureFormat format, const void* image)
{
    glEnable(GL_TEXTURE_2D);

    u32 tex_obj;

    glGenTextures(1, &tex_obj);
    glBindTexture(GL_TEXTURE_2D, tex_obj);

    glGetError();

    if (image)
    {
        static const GLint s_gl_internal_format[] =
        {
            GL_RGB, GL_RGBA, GL_ALPHA
        };

        glTexImage2D(GL_TEXTURE_2D, 0, s_gl_internal_format[format], width, height, 0, s_gl_internal_format[format], GL_UNSIGNED_BYTE, image);
    }

    if (glGetError() != GL_NO_ERROR)
    {
        tex_obj = 0;
    }

    resetTexture();

    return tex_obj;
}


void fsLowLevelAPI::replaceSubTexture(u32 tex_obj, u16 x, u16 y, u16 width, u16 height, TextureFormat format, const void* sub_image)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tex_obj);

    if (sub_image)
    {
        static const GLint s_gl_internal_format[] =
        {
            GL_RGB, GL_RGBA, GL_ALPHA
        };

        glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, width, height, s_gl_internal_format[format], GL_UNSIGNED_BYTE, sub_image);
    }

    resetTexture();
}


void fsLowLevelAPI::unregisterTexture(u32 tex_obj)
{
    glDeleteTextures(1, &tex_obj);

    resetTexture();
}


void fsLowLevelAPI::setTexture(u32 tex_obj1, u32 tex_obj2, u32 tex_obj3, bool is_bilinear)
{
#define SETUP_TEXTURE(index, tex_obj) \
    do \
    { \
        if (s_tex_obj[index] != tex_obj || (s_tex_obj[index] != 0 && s_is_bilinear[index] != is_bilinear)) \
        { \
            s_tex_obj[index] = tex_obj; \
            s_is_bilinear[index] = is_bilinear; \
    \
            if (tex_obj == 0) \
            { \
                glDisable(GL_TEXTURE_2D); \
            } \
            else \
            { \
                glEnable(GL_TEXTURE_2D); \
    \
                glBindTexture(GL_TEXTURE_2D, tex_obj); \
    \
                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); \
                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); \
    \
                if (is_bilinear) \
                { \
                    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); \
                    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); \
                } \
                else \
                { \
                    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); \
                    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); \
                } \
            } \
        } \
    } \
    while (false)

    SETUP_TEXTURE(0, tex_obj1);

#ifdef FS_GLSL
    if (s_is_shader_available)
    {
        fsglActiveTexture(FSGL_TEXTURE1);
        SETUP_TEXTURE(1, tex_obj2);

        fsglActiveTexture(FSGL_TEXTURE2);
        SETUP_TEXTURE(2, tex_obj3);

        fsglActiveTexture(FSGL_TEXTURE0);
    }
#endif

#undef SETUP_TEXTURE
}


void fsLowLevelAPI::copyImageFromFramebufferToTexture( //
    u32 tex_obj, u16 tex_width, u16 tex_height, TextureFormat tex_format, u16 left_in_framebuffer, u16 top_in_framebuffer)
{
    if ((tex_obj != 0 && tex_format == FORMAT_RGB) || tex_format == FORMAT_RGBA)
    {
        setTexture(tex_obj, 0, 0, true);

        glCopyTexImage2D(GL_TEXTURE_2D, 0, (tex_format == FORMAT_RGB) ? GL_RGB : GL_RGBA, //
            left_in_framebuffer, getFramebufferHeight() - (top_in_framebuffer + tex_height), tex_width, tex_height, 0);
    }
}


void fsLowLevelAPI::setProjection(const r32* projection)
{
#ifndef FS_GLES2
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(projection);
#endif

#ifdef FS_GLSL
    memcpy(s_projection, projection, sizeof(s_projection));
#endif
}


void fsLowLevelAPI::setModelView(const r32* model_view)
{
#ifndef FS_GLES2
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(model_view);
#endif

#ifdef FS_GLSL
    memcpy(s_model_view, model_view, sizeof(s_model_view));
#endif
}


void fsLowLevelAPI::setVertexPointer(u32 stride, const r32* vert)
{
#ifndef FS_GLES2
    if (vert)
    {
        setVertexArrayState(true);
        glVertexPointer(3, GL_FLOAT, static_cast<GLsizei>(stride), reinterpret_cast<const GLvoid*>(vert));
    }
    else
    {
        setVertexArrayState(false);
    }
#endif
}


void fsLowLevelAPI::setColorPointer(u32 stride, const u8* color)
{
#ifndef FS_GLES2
    if (color)
    {
        setColorArrayState(true);
        glColorPointer(4, GL_UNSIGNED_BYTE, static_cast<GLsizei>(stride), reinterpret_cast<const GLvoid*>(color));
    }
    else
    {
        setColorArrayState(false);
    }
#endif
}


void fsLowLevelAPI::setTexCoordPointer(u32 stride, const r32* uv)
{
#ifndef FS_GLES2
    if (uv)
    {
        setTexCoordArrayState(true);
        glTexCoordPointer(2, GL_FLOAT, static_cast<GLsizei>(stride), reinterpret_cast<const GLvoid*>(uv));
    }
    else
    {
        setTexCoordArrayState(false);
    }
#endif
}


void fsLowLevelAPI::drawArrays(DrawMode draw_mode, u16 first, u16 count)
{
    static const GLenum s_gl_draw_mode[] =
    {
        GL_POINTS, //
        GL_LINES, //
        GL_LINE_LOOP, //
        GL_LINE_STRIP, //
        GL_TRIANGLES, //
        GL_TRIANGLE_STRIP, //
        GL_TRIANGLE_FAN, //
    };

    glDrawArrays(s_gl_draw_mode[draw_mode], static_cast<GLint>(first), static_cast<GLsizei>(count));
}


u32 fsLowLevelAPI::registerShader(const char* vert_code, const char* frag_code)
{
#ifdef FS_GLSL

#define PRINT_LOG(type, obj, msg) \
    do \
    { \
        GLint buf_size; \
        fsglGet##type##iv(obj, FSGL_INFO_LOG_LENGTH, &buf_size); \
    \
        if (buf_size > 1) \
        { \
            char* buf = static_cast<char*>(fsLowLevelAPI::malloc(buf_size)); \
            GLint wrote_size; \
    \
            fsglGet##type##InfoLog(obj, buf_size, &wrote_size, buf); \
    \
            fsLowLevelAPI::printf(msg); \
            fsLowLevelAPI::printf("\n"); \
            fsLowLevelAPI::printf(buf); \
    \
            fsLowLevelAPI::free(buf); \
        } \
    } \
    while (false)

    if (s_is_shader_available)
    {
        u32 vert_obj = 0;

        if (vert_code)
        {
            vert_obj = fsglCreateShader(FSGL_VERTEX_SHADER);
            fsglShaderSource(vert_obj, 1, &vert_code, NULL);
            fsglCompileShader(vert_obj);

            GLint is_compile;
            fsglGetShaderiv(vert_obj, FSGL_COMPILE_STATUS, &is_compile);

            if (!is_compile)
            {
                PRINT_LOG(Shader, vert_obj, "*** vertex shader compile error ***");
                fsglDeleteShader(vert_obj);

                return 0;
            }
        }

        u32 frag_obj = 0;

        if (frag_code)
        {
            frag_obj = fsglCreateShader(FSGL_FRAGMENT_SHADER);
            fsglShaderSource(frag_obj, 1, &frag_code, NULL);
            fsglCompileShader(frag_obj);

            GLint is_compile;
            fsglGetShaderiv(frag_obj, FSGL_COMPILE_STATUS, &is_compile);

            if (!is_compile)
            {
                PRINT_LOG(Shader, frag_obj, "*** fragment shader compile error ***");
                fsglDeleteShader(frag_obj);

                if (vert_code)
                {
                    fsglDeleteShader(vert_obj);
                }

                return 0;
            }
        }

        u32 prog_obj = fsglCreateProgram();

        if (vert_code)
        {
            fsglAttachShader(prog_obj, vert_obj);
            fsglDeleteShader(vert_obj);
        }

        if (frag_code)
        {
            fsglAttachShader(prog_obj, frag_obj);
            fsglDeleteShader(frag_obj);
        }

        fsglLinkProgram(prog_obj);

        GLint is_link;
        fsglGetProgramiv(prog_obj, FSGL_LINK_STATUS, &is_link);

        if (!is_link)
        {
            PRINT_LOG(Program, prog_obj, "*** shader link error ***");
            fsglDeleteProgram(prog_obj);

            return 0;
        }

        return prog_obj;
    }
    else
    {
        return 0;
    }

#undef PRINT_LOG

#else

    return 0;

#endif // FS_GLSL
}


void fsLowLevelAPI::unregisterShader(u32 shd_obj)
{
#ifdef FS_GLSL
    if (s_is_shader_available)
    {
        fsglDeleteShader(shd_obj);
    }
#endif
}


void fsLowLevelAPI::setShader(u32 shd_obj)
{
#ifdef FS_GLSL
    if (s_is_shader_available)
    {
        if (s_shd_obj != shd_obj)
        {
            s_shd_obj = shd_obj;

            fsglUseProgram(s_shd_obj);
        }
    }
#endif
}


u32 fsLowLevelAPI::getUniformLocation(u32 shd_obj, const char* name)
{
#ifdef FS_GLSL
    return s_is_shader_available ? fsglGetUniformLocation(shd_obj, name) : 0;
#else
    return 0;
#endif
}


void fsLowLevelAPI::setUniform_s32(u32 location, s32 uniform)
{
#ifdef FS_GLSL
    if (s_is_shader_available)
    {
        fsglUniform1i(location, uniform);
    }
#endif
}


void fsLowLevelAPI::setUniform_r32(u32 location, r32 uniform)
{
#ifdef FS_GLSL
    if (s_is_shader_available)
    {
        fsglUniform1f(location, uniform);
    }
#endif
}


void fsLowLevelAPI::setUniform_localToScreen(u32 location)
{
#ifdef FS_GLSL
    if (s_is_shader_available)
    {
        r32 local_to_screen[16];

        fsMat::mulR32x16(local_to_screen, s_projection, s_model_view);

        fsglUniformMatrix4fv(location, 1, GL_FALSE, local_to_screen);
    }
#endif
}


u32 fsLowLevelAPI::getAttribLocation(u32 shd_obj, const char* name)
{
#ifdef FS_GLSL
    return s_is_shader_available ? fsglGetAttribLocation(shd_obj, name) : 0;
#else
    return 0;
#endif
}


void fsLowLevelAPI::setAttribPointer_r32(u32 location, u8 size, u32 stride, const r32* attrib)
{
#ifdef FS_GLSL
    if (s_is_shader_available)
    {
        fsglEnableVertexAttribArray(location);
        fsglVertexAttribPointer(location, size, GL_FLOAT, GL_FALSE, stride, attrib);
    }
#endif
}


void fsLowLevelAPI::setAttribPointer_color(u32 location, u32 stride, const u8* attrib)
{
#ifdef FS_GLSL
    if (s_is_shader_available)
    {
        fsglEnableVertexAttribArray(location);
        fsglVertexAttribPointer(location, 4, GL_UNSIGNED_BYTE, GL_TRUE, stride, attrib);
    }
#endif
}


void fsLowLevelAPI::disableAttribPointer(u32 location)
{
#ifdef FS_GLSL
    if (s_is_shader_available)
    {
        fsglDisableVertexAttribArray(location);
    }
#endif
}
