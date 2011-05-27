/*
 * fs_util_import_3ds_model.cpp
 *
 *  Created on: 2011-5-27
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */


#include "fs_kernel_all.h"
#include "fs_res_all.h"


void fsUtil::import3DSModel(const char* filename, fsID tex_id, bool has_normal, bool is_smoothing_normal, r32 scale)
{
    return import3DSModelAs(fsID::genID(fsUtil::getBasename(filename)), filename, tex_id, has_normal, is_smoothing_normal, scale);
}


static void setNodeInfo(fsMdlData* mdl_data, u16 node_index, const fsMat& local, fsDraw::BlendMode blend_mode, //
    u16 vert_index, u16 vert_num, const u8* vert_array, const u8* uv_array, const u8* face_array, r32 scale)
{
    fsMat scaled_local = local;
    scaled_local.trans *= scale;

    mdl_data->setNodeLocal(node_index, scaled_local);
    mdl_data->setNodePrimMode(node_index, fsPrim::MODE_TRIANGLES);
    mdl_data->setNodeBlendMode(node_index, blend_mode);
    mdl_data->setNodeVertInfo(node_index, vert_index, vert_num);

    if (!face_array)
    {
        return;
    }

    if (vert_array)
    {
        if (scale != 1.0f)
        {
            for (s32 i = 0; i < vert_num; i++)
            {
                u16 array_index = fsReadLittleEndian(face_array + (i + i / 3) * sizeof(u16), u16) * 3;
                fsVec vert_pos( //
                    fsReadLittleEndian(vert_array + sizeof(r32) * (array_index + 0), r32), //
                    fsReadLittleEndian(vert_array + sizeof(r32) * (array_index + 1), r32), //
                    fsReadLittleEndian(vert_array + sizeof(r32) * (array_index + 2), r32));

                mdl_data->setVertPos(vert_index + i, vert_pos * scale);
            }
        }
        else
        {
            for (s32 i = 0; i < vert_num; i++)
            {
                u16 array_index = fsReadLittleEndian(face_array + (i + i / 3) * sizeof(u16), u16) * 3;
                fsVec vert_pos( //
                    fsReadLittleEndian(vert_array + sizeof(r32) * (array_index + 0), r32), //
                    fsReadLittleEndian(vert_array + sizeof(r32) * (array_index + 1), r32), //
                    fsReadLittleEndian(vert_array + sizeof(r32) * (array_index + 2), r32));

                mdl_data->setVertPos(vert_index + i, vert_pos);
            }
        }
    }

    if (uv_array)
    {
        for (s32 i = 0; i < vert_num; i++)
        {
            u16 array_index = fsReadLittleEndian(face_array + (i + i / 3) * sizeof(u16), u16) * 2;
            r32 u = fsReadLittleEndian(uv_array + sizeof(r32) * (array_index + 0), r32);
            r32 v = -fsReadLittleEndian(uv_array + sizeof(r32) * (array_index + 1), r32);

            if (u > 1.0f)
            {
                u -= static_cast<s32>(u);
            }
            else if (u < 0.0f)
            {
                u += static_cast<s32>(u) + 1.0f;
            }

            if (v > 1.0f)
            {
                v -= static_cast<s32>(v);
            }
            else if (v < 0.0f)
            {
                v += static_cast<s32>(v) + 1.0f;
            }

            mdl_data->setVertUV(vert_index + i, u, v);
        }
    }
}


void fsUtil::import3DSModelAs(fsID res_id, const char* filename, fsID tex_id, bool has_normal, bool is_smoothing_normal, r32 scale)
{
    if ((!has_normal && is_smoothing_normal) || scale <= 0.0f)
    {
        fsThrow(ExceptionInvalidArgument);
    }

    fsID temp_res_id = fsID::genID();

    fsResMgr::loadResourceAs(temp_res_id, filename, false);

    fsRes temp_res = fsResMgr::getResource(temp_res_id);

    /*
        count up the number of objects and vertices in 3ds file
    */
    const u8* data = temp_res.getData<u8>();
    const u8* data_end = data + temp_res.getDataSize();

    u32 node_num = 0;
    u32 vert_num = 0;

    while (data < data_end)
    {
        u16 chunk_id = fsReadLittleEndian(data, u16);
        data += 2;

        u32 chunk_size = fsReadLittleEndian(data, u32);
        data += 4;

        switch (chunk_id)
        {
        case 0x4d4d: // MAIN CHUNK
            break;

        case 0x3d3d: // 3D EDITOR CHUNK
            break;

        case 0x4000: // OBJECT BLOCK
            node_num++;
            data += fsUtil::strlen(reinterpret_cast<const char*>(data)) + 1;
            break;

        case 0x4100: // TRIANGULAR MESH
            break;

        case 0x4120: // FACES DESCRIPTION
            vert_num += fsReadLittleEndian(data, u16) * 3;
            data += chunk_size - 6;
            break;

        default:
            data += chunk_size - 6;
            break;
        }
    }

    if (node_num == 0 || node_num > 0xffff || vert_num == 0 || vert_num > 0xffff)
    {
        fsResMgr::removeResource(temp_res_id);
        fsThrow(ExceptionInvalidData);
    }

    /*
        make fsMdlData from 3ds file
    */
    fsMdlData mdl_data;
    mdl_data.initAsWriter(node_num, vert_num, tex_id, has_normal);

    bool is_first_node = true;
    u16 node_vert_num = 0;
    fsMat node_local = fsMat::UNIT;
    fsDraw::BlendMode node_blend_mode = fsDraw::BLEND_OFF;
    u16 node_index = 0;
    u16 vert_index = 0;
    u16 tree_node_index = 0;
    const u8* vert_array = NULL;
    const u8* uv_array = NULL;
    const u8* face_array = NULL;

    data = temp_res.getData<u8>();

    while (data < data_end)
    {
        u16 chunk_id = fsReadLittleEndian(data, u16);
        data += 2;

        u32 chunk_size = fsReadLittleEndian(data, u32);
        data += 4;

        switch (chunk_id)
        {
        case 0x4d4d: // MAIN CHUNK
            break;

        case 0x3d3d: // 3D EDITOR CHUNK
            break;

        case 0x4000: // OBJECT BLOCK
            if (is_first_node)
            {
                is_first_node = false;
            }
            else
            {
                setNodeInfo(&mdl_data, node_index, node_local, node_blend_mode, //
                    vert_index, node_vert_num, vert_array, uv_array, face_array, scale);

                node_index++;
                vert_index += node_vert_num;

                node_local = fsMat::UNIT;
                node_vert_num = 0;
                vert_array = NULL;
                uv_array = NULL;
                face_array = NULL;
            }

            switch (*reinterpret_cast<const char*>(data))
            {
            case '*':
                node_blend_mode = fsDraw::BLEND_HALF;
                break;

            case '+':
                node_blend_mode = fsDraw::BLEND_ADD;
                break;

            default:
                node_blend_mode = fsDraw::BLEND_OFF;
                break;
            }

            data += fsUtil::strlen(reinterpret_cast<const char*>(data)) + 1;
            break;

        case 0x4100: // TRIANGULAR MESH
            break;

        case 0x4110: // VERTICES LIST
            vert_array = data + 2;
            data += chunk_size - 6;
            break;

        case 0x4120: // FACES DESCRIPTION
            node_vert_num = fsReadLittleEndian(data, u16) * 3;
            face_array = data + 2;
            data += chunk_size - 6;
            break;

        case 0x4140: // MAPPING COORDINATES LIST
            uv_array = data + 2;
            data += chunk_size - 6;
            break;

        case 0x4160: // LOCAL COORDINATES SYSTEM
            node_local = *reinterpret_cast<const fsMat*>(data);
            data += chunk_size - 6;
            break;

        case 0xb000: // KEYFRAMER CHUNK
            break;

        case 0xb002: // MESH INFORMATION BLOCK
            break;

        case 0xb010: // OBJECT NAME
            {
                const u8* parent_index_data = data + fsUtil::strlen(reinterpret_cast<const char*>(data)) + 5;
                s16 parent_index = fsReadLittleEndian(parent_index_data, s16);

                if (parent_index >= 0 && parent_index < static_cast<u16>(node_num))
                {
                    mdl_data.setNodeParentIndex(tree_node_index, parent_index);
                }
                else
                {
                    mdl_data.setNodeParentIndex_noParent(tree_node_index);
                }

                tree_node_index++;
            }

            data += chunk_size - 6;
            break;

        default:
            data += chunk_size - 6;
            break;
        }
    }

    if (!is_first_node)
    {
        setNodeInfo(&mdl_data, node_index, node_local, node_blend_mode, vert_index, //
            node_vert_num, vert_array, uv_array, face_array, scale);
    }

    if (has_normal)
    {
        mdl_data.calcNormalAsTriangles(is_smoothing_normal);
    }

    mdl_data.registerAsResource(res_id);

    fsResMgr::removeResource(temp_res_id);
}
