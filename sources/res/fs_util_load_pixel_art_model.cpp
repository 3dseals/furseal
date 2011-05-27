/*
 * fs_util_load_pixel_art_model.cpp
 *
 *  Created on: 2011-5-27
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */


#include "fs_draw_all.h"

#include "fs_res_all.h"
#include "fs_script_all.h"


enum DirectionType
{
    DIRECTION_FRONT, //
    DIRECTION_BACK, //
    DIRECTION_LEFT, //
    DIRECTION_RIGHT, //
    DIRECTION_TOP, //
    DIRECTION_BOTTOM, //
    DIRECTION_NUM
};


class PixelImage
{
public:
    static const u32 PIXEL_SIZE = 3;

    PixelImage(u16 width, u16 height)
    {
        m_side_length = width / DIRECTION_NUM;
        m_pattern_num = height / m_side_length;
        m_pattern_size = PIXEL_SIZE * width * m_side_length;

        m_image_data_line_size = PIXEL_SIZE * width;
        m_image_data_size = PIXEL_SIZE * width * height;
        m_image_data = static_cast<u8*>(fsMalloc(m_image_data_size));

        m_cur_direction = DIRECTION_FRONT;

        setCurPattern(0);
    }

    ~PixelImage()
    {
        fsFree(m_image_data);
    }

    u16 getSideLength() const
    {
        return m_side_length;
    }

    u16 getPatternNum() const
    {
        return m_pattern_num;
    }

    u8* getImageData()
    {
        return m_image_data;
    }

    u32 getImageDataSize() const
    {
        return m_image_data_size;
    }

    u32 getImageDataLineSize() const
    {
        return m_image_data_line_size;
    }

    void setCurPattern(u16 pattern)
    {
        m_cur_pattern = pattern;
        m_cur_pattern_data = m_image_data + m_pattern_size * m_cur_pattern;

        setCurDirection(m_cur_direction);
    }

    void setCurDirection(DirectionType direction)
    {
        m_cur_direction = direction;
        m_cur_direction_data = m_cur_pattern_data + PIXEL_SIZE * m_side_length * m_cur_direction;
    }

    fsCol getPixelColor(u16 x, u16 y)
    {
        u8* col = m_cur_direction_data + m_image_data_line_size * y + PIXEL_SIZE * x;

        return fsCol(*(col + 0), *(col + 1), *(col + 2));
    }

private:
    u16 m_side_length;
    u16 m_pattern_num;
    u32 m_pattern_size;

    u8* m_image_data;
    u32 m_image_data_size;
    u32 m_image_data_line_size;

    u16 m_cur_pattern;
    u8* m_cur_pattern_data;
    DirectionType m_cur_direction;
    u8* m_cur_direction_data;
};


class PixelCube
{
public:
    PixelCube(u16 side_length, u16 pattern_num)
    {
        m_side_length = side_length;
        m_sq_side_length = m_side_length * m_side_length;
        m_pattern_num = pattern_num;

        m_cube_data_size = sizeof(Cube) * m_sq_side_length * m_side_length * m_pattern_num;
        m_cube_data = static_cast<Cube*>(fsMalloc(m_cube_data_size));

        fsMemHelper::memset(m_cube_data, 1, m_cube_data_size);

        setCurPattern(0);
    }

    ~PixelCube()
    {
        fsFree(m_cube_data);
    }

    void setCurPattern(u16 pattern)
    {
        m_cur_pattern = pattern;
        m_cur_pattern_data = m_cube_data + m_sq_side_length * m_side_length * m_cur_pattern;
    }

    void eraseCube(DirectionType direction, u16 image_x, u16 image_y, u16 image_z)
    {
        fsMemHelper::memset(getCube(direction, image_x, image_y, image_z), 0, sizeof(Cube));
    }

    fsCol getCubeColor(DirectionType direction, u16 image_x, u16 image_y, u16 image_z)
    {
        return getCube(direction, image_x, image_y, image_z)->col;
    }

    void setCubeColor(DirectionType direction, u16 image_x, u16 image_y, u16 image_z, fsCol col)
    {
        getCube(direction, image_x, image_y, image_z)->col = col;
    }

    bool hasFace(DirectionType direction, u16 image_x, u16 image_y, u16 image_z)
    {
        return (getCube(direction, image_x, image_y, image_z)->face[direction].width > 0);
    }

    void eraseFace(DirectionType direction, u16 image_x, u16 image_y, u16 image_z)
    {
        fsMemHelper::memset(getCube(direction, image_x, image_y, image_z)->face + direction, 0, sizeof(Face));
    }

    void expandFace(DirectionType direction, u16 image_x, u16 image_y, u16 image_z)
    {
        Cube* cube1 = getCube(direction, image_x, image_y, image_z);
        Face* face1 = cube1->face + direction;

        if (face1->width != 1 || face1->height != 1)
        {
            return;
        }

        for (s32 i = image_x + 1; i < m_side_length; i++)
        {
            Cube* cube2 = getCube(direction, i, image_y, image_z);
            Face* face2 = cube2->face + direction;

            if (face2->width == 1 && face2->height == 1 && cube1->col == cube2->col)
            {
                face1->width++;
                face2->width = face2->height = 0;
            }
            else
            {
                break;
            }
        }

        for (s32 i = image_y + 1; i < m_side_length; i++)
        {
            s32 max_image_x = image_x + face1->width;
            bool can_merge = true;

            for (s32 j = image_x; j < max_image_x; j++)
            {
                Cube* cube2 = getCube(direction, j, i, image_z);
                Face* face2 = cube2->face + direction;

                if (face2->width != 1 || face2->height != 1 || cube1->col != cube2->col)
                {
                    can_merge = false;
                    break;
                }
            }

            if (!can_merge)
            {
                break;
            }

            face1->height++;

            for (s32 j = image_x; j < max_image_x; j++)
            {
                Face* face2 = getCube(direction, j, i, image_z)->face + direction;

                face2->width = face2->height = 0;
            }
        }
    }

    void calcVertPos(fsVec* vert, DirectionType direction, u16 image_x, u16 image_y, u16 image_z)
    {
        Cube* cube = getCube(direction, image_x, image_y, image_z);
        Face* face = cube->face + direction;

        const r32 SIZE_MARGIN = 0.001f;

        r32 image_x2 = static_cast<r32>(image_x) - 0.5f - SIZE_MARGIN;
        r32 image_y2 = static_cast<r32>(image_y) - 0.5f - SIZE_MARGIN;
        r32 image_z2 = static_cast<r32>(image_z) - 0.5f - SIZE_MARGIN;

        r32 width2 = face->width + SIZE_MARGIN * 2.0f;
        r32 height2 = face->height + SIZE_MARGIN * 2.0f;

        r32 cube_x, cube_y, cube_z;

        calcCubePos(&cube_x, &cube_y, &cube_z, direction, image_x2, image_y2, image_z2);
        vert[0].set(cube_x, cube_y, cube_z);

        calcCubePos(&cube_x, &cube_y, &cube_z, direction, image_x2, image_y2 + height2, image_z2);
        vert[1].set(cube_x, cube_y, cube_z);

        calcCubePos(&cube_x, &cube_y, &cube_z, direction, image_x2 + width2, image_y2 + height2, image_z2);
        vert[2].set(cube_x, cube_y, cube_z);

        calcCubePos(&cube_x, &cube_y, &cube_z, direction, image_x2 + width2, image_y2, image_z2);
        vert[5].set(cube_x, cube_y, cube_z);

        fsVec offset = fsVec(1.0f, 1.0f, 1.0f) * (static_cast<r32>(m_side_length - 1) * 0.5f);

        vert[0] -= offset;
        vert[1] -= offset;
        vert[2] -= offset;
        vert[5] -= offset;

        vert[3] = vert[0];
        vert[4] = vert[2];
    }

private:
    struct Face
    {
        u8 width;
        u8 height;
    };

    struct Cube
    {
        Face face[DIRECTION_NUM];
        fsCol col;
    };

    void calcCubePos(r32* cube_x, r32* cube_y, r32* cube_z, DirectionType direction, r32 image_x, r32 image_y, r32 image_z)
    {
        switch (direction)
        {
        case DIRECTION_FRONT:
            *cube_x = m_side_length - 1.0f - image_x;
            *cube_y = m_side_length - 1.0f - image_y;
            *cube_z = image_z;
            break;

        case DIRECTION_BACK:
            *cube_x = image_x;
            *cube_y = m_side_length - 1.0f - image_y;
            *cube_z = m_side_length - 1.0f - image_z;
            break;

        case DIRECTION_LEFT:
            *cube_x = m_side_length - 1.0f - image_z;
            *cube_y = m_side_length - 1.0f - image_y;
            *cube_z = m_side_length - 1.0f - image_x;
            break;

        case DIRECTION_RIGHT:
            *cube_x = image_z;
            *cube_y = m_side_length - 1.0f - image_y;
            *cube_z = image_x;
            break;

        case DIRECTION_TOP:
            *cube_x = m_side_length - 1.0f - image_x;
            *cube_y = m_side_length - 1.0f - image_z;
            *cube_z = m_side_length - 1.0f - image_y;
            break;

        case DIRECTION_BOTTOM:
            *cube_x = m_side_length - 1.0f - image_x;
            *cube_y = image_z;
            *cube_z = image_y;
            break;

        default:
            *cube_x = *cube_y = *cube_z = 0.0f;
            break;
        }
    }

    Cube* getCube(u16 cube_x, u16 cube_y, u16 cube_z)
    {
        return m_cur_pattern_data + m_sq_side_length * cube_x + m_side_length * cube_y + cube_z;
    }

    Cube* getCube(DirectionType direction, u16 image_x, u16 image_y, u16 image_z)
    {
        r32 cube_x, cube_y, cube_z;
        calcCubePos(&cube_x, &cube_y, &cube_z, direction, image_x, image_y, image_z);

        return getCube(static_cast<u16>(cube_x), static_cast<u16>(cube_y), static_cast<u16>(cube_z));
    }

    u16 m_side_length;
    u16 m_sq_side_length;
    u16 m_pattern_num;

    Cube* m_cube_data;
    u32 m_cube_data_size;

    u16 m_cur_pattern;
    Cube* m_cur_pattern_data;
};


static void calcNodeLocalRecursively(fsMdlData* mdl_data, u16 node_index)
{
    u16 node_num = mdl_data->getNodeNum();

    for (s32 i = 0; i < node_num; i++)
    {
        if (i != node_index && mdl_data->getNodeParentIndex(i) == node_index)
        {
            calcNodeLocalRecursively(mdl_data, i);
        }
    }

    u16 parent_index = mdl_data->getNodeParentIndex(node_index);

    if (parent_index != node_index)
    {
        fsMat local = mdl_data->getNodeLocal(node_index);
        local.trans -= mdl_data->getNodeLocal(parent_index).trans;

        mdl_data->setNodeLocal(node_index, local);
    }
}


void fsUtil::loadPixelArtModelAs(fsID res_id, const char* fsl_file, const char* png_file, r32 scale)
{
    if (scale <= 0.0f)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    /*
        read parameters
    */
    fsID conf_res_id = fsID::genID();

    fsResMgr::loadResourceAs(conf_res_id, fsl_file, false);

    fsRes conf_res = fsResMgr::getResource(conf_res_id);
    fsScript* conf = fsScriptMgr::newConfig(fsID::genID(), conf_res.getData<void>(), conf_res.getDataSize());

    fsResMgr::removeResource(conf_res_id);

    fsScriptEntry* color_ent = conf->getEntryFromFirstN("transparent_color");

    if (!color_ent || color_ent->getValueNum() < 3)
    {
        fsScriptMgr::deleteConfig(conf->getID());
        fsThrow(ExceptionInvalidData);
    }

    fsCol transparent_color = fsCol(color_ent->getValue_s32(0), color_ent->getValue_s32(1), color_ent->getValue_s32(2));

    /*
        load an image
    */
    fsID png_res_id = fsID::genID();

    fsResMgr::loadResourceAs(png_res_id, png_file, false);

    fsRes png_res = fsResMgr::getResource(png_res_id);

    const u8* png_data = png_res.getData<u8>();
    const u32 png_data_size = png_res.getDataSize();

    u16 image_width, image_height;
    fsTex::TexFormat tex_format;

    if (!fsUtil::readPNGInfo(&image_width, &image_height, &tex_format, png_data, png_data_size) || tex_format != fsTex::FORMAT_PNG_RGB || //
        image_width == 0 || image_height == 0 || image_width % DIRECTION_NUM != 0 || image_height % (image_width / DIRECTION_NUM) != 0)
    {
        fsScriptMgr::deleteConfig(conf->getID());
        fsResMgr::removeResource(png_res_id);

        fsThrow(ExceptionInvalidData);
    }

    PixelImage pixel_image(image_width, image_height);

    if (!fsUtil::readPNGImage(pixel_image.getImageData(), pixel_image.getImageDataSize(), pixel_image.getImageDataLineSize(), png_data, png_data_size))
    {
        fsScriptMgr::deleteConfig(conf->getID());
        fsResMgr::removeResource(png_res_id);
        fsThrow(ExceptionInvalidData);
    }

    fsResMgr::removeResource(png_res_id);

    /*
        setup a pixel cube
    */
    u16 vert_num = 0;
    u16 side_length = pixel_image.getSideLength();
    u16 pattern_num = pixel_image.getPatternNum();

    PixelCube pixel_cube(side_length, pattern_num);

    for (s32 pattern = 0; pattern < pattern_num; pattern++)
    {
        pixel_image.setCurPattern(pattern);
        pixel_cube.setCurPattern(pattern);

        /*
            erase cells
        */
        for (DirectionType direction = DIRECTION_FRONT; direction < DIRECTION_NUM; direction = static_cast<DirectionType>(direction + 1))
        {
            pixel_image.setCurDirection(direction);

            for (s32 image_x = 0; image_x < side_length; image_x++)
            {
                for (s32 image_y = 0; image_y < side_length; image_y++)
                {
                    fsCol pixel_color = pixel_image.getPixelColor(image_x, image_y);

                    if (pixel_color == transparent_color)
                    {
                        for (s32 image_z = 0; image_z < side_length; image_z++)
                        {
                            pixel_cube.eraseCube(direction, image_x, image_y, image_z);
                        }
                    }
                }
            }
        }

        /*
            paint cubes
        */
        for (DirectionType direction = DIRECTION_BOTTOM; direction >= DIRECTION_FRONT; direction = static_cast<DirectionType>(direction - 1))
        {
            pixel_image.setCurDirection(direction);

            for (s32 image_x = 0; image_x < side_length; image_x++)
            {
                for (s32 image_y = 0; image_y < side_length; image_y++)
                {
                    fsCol pixel_color = pixel_image.getPixelColor(image_x, image_y);

                    if (pixel_color == transparent_color)
                    {
                        continue;
                    }

                    for (s32 image_z = side_length - 1; image_z >= 0; image_z--)
                    {
                        if (pixel_cube.hasFace(direction, image_x, image_y, image_z))
                        {
                            if (image_z == 0 || !pixel_cube.hasFace(direction, image_x, image_y, image_z - 1))
                            {
                                pixel_cube.setCubeColor(direction, image_x, image_y, image_z, pixel_color);
                            }
                            else
                            {
                                pixel_cube.eraseFace(direction, image_x, image_y, image_z);
                            }
                        }
                    }
                }
            }
        }

        /*
            merge faces
        */
        for (DirectionType direction = DIRECTION_FRONT; direction < DIRECTION_NUM; direction = static_cast<DirectionType>(direction + 1))
        {
            for (s32 image_y = 0; image_y < side_length; image_y++)
            {
                for (s32 image_x = 0; image_x < side_length; image_x++)
                {
                    for (s32 image_z = 0; image_z < side_length; image_z++)
                    {
                        if (pixel_cube.hasFace(direction, image_x, image_y, image_z))
                        {
                            pixel_cube.expandFace(direction, image_x, image_y, image_z);

                            vert_num += 6;
                        }
                    }
                }
            }
        }
    }

    /*
        setup a model data
    */
    fsMdlData mdl_data;
    mdl_data.initAsWriter(pattern_num, vert_num, fsID::ZERO, true);

    u16 vert_index = 0;
    fsScriptEntry* parent_ent = conf->getEntryFromFirstN("node_parent");
    fsScriptEntry* center_ent = conf->getEntryFromFirstN("node_center");

    for (s32 pattern = 0; pattern < pattern_num; pattern++)
    {
        pixel_cube.setCurPattern(pattern);

        s32 parent_index = parent_ent ? parent_ent->getValue_s32(0) : pattern;

        if (parent_index >= 0 && parent_index < pattern_num && parent_index != pattern)
        {
            mdl_data.setNodeParentIndex(pattern, parent_index);
        }
        else
        {
            mdl_data.setNodeParentIndex_noParent(pattern);
        }

        fsVec center = fsVec::ZERO;

        if (center_ent && center_ent->getValueNum() >= 3)
        {
            center.set(center_ent->getValue_r32(0), center_ent->getValue_r32(1), center_ent->getValue_r32(2));
            center *= scale;

            fsMat local = fsMat::UNIT;
            local.trans = center;

            mdl_data.setNodeLocal(pattern, local);
        }
        else
        {
            mdl_data.setNodeLocal(pattern, fsMat::UNIT);
        }

        mdl_data.setNodePrimMode(pattern, fsPrim::MODE_TRIANGLES);
        mdl_data.setNodeBlendMode(pattern, fsDraw::BLEND_OFF);

        u16 start_vert_index = vert_index;

        for (DirectionType direction = DIRECTION_FRONT; direction < DIRECTION_NUM; direction = static_cast<DirectionType>(direction + 1))
        {
            for (s32 image_x = 0; image_x < side_length; image_x++)
            {
                for (s32 image_y = 0; image_y < side_length; image_y++)
                {
                    for (s32 image_z = 0; image_z < side_length; image_z++)
                    {
                        if (pixel_cube.hasFace(direction, image_x, image_y, image_z))
                        {
                            fsVec vert[6];
                            pixel_cube.calcVertPos(vert, direction, image_x, image_y, image_z);

                            fsCol col = pixel_cube.getCubeColor(direction, image_x, image_y, image_z);

                            for (s32 i = 0; i < 6; i++)
                            {
                                mdl_data.setVertPos(vert_index, vert[i] * scale - center);
                                mdl_data.setVertCol(vert_index, col);
                                mdl_data.setVertUV(vert_index, 0.0f, 0.0f);

                                vert_index++;
                            }
                        }
                    }
                }
            }
        }

        mdl_data.setNodeVertInfo(pattern, start_vert_index, vert_index - start_vert_index);

        parent_ent = parent_ent ? conf->getEntryAfterN("node_parent", parent_ent) : NULL;
        center_ent = center_ent ? conf->getEntryAfterN("node_center", center_ent) : NULL;
    }

    for (s32 i = 0; i < pattern_num; i++)
    {
        calcNodeLocalRecursively(&mdl_data, i);
    }

    fsScriptMgr::deleteConfig(conf->getID());

    mdl_data.calcNormalAsTriangles(false);
    mdl_data.registerAsResource(res_id);
}
