/*
 * fs_debug_mgr_dump.cpp
 *
 *  Created on: 2011-5-26
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */



#include "fs_debug_all.h"

#include "fs_kernel_all.h"
#include "fs_task_all.h"
#include "fs_res_all.h"
#include "fs_script_all.h"


void fsDebugMgr::dumpVector(const fsVec& vec, const char* name)
{
    if (!m_instance)
    {
        return;
    }

    if (name)
    {
        trace("%s = ", name);
    }

    trace("(%.3f, %.3f, %.3f)\n", vec.x, vec.y, vec.z);
}


void fsDebugMgr::dumpMatrix(const fsMat& mat, const char* name)
{
    if (!m_instance)
    {
        return;
    }

    if (name)
    {
        trace("%s = ", name);
    }

    trace("((%.3f, %.3f, %.3f), ", mat.x_axis.x, mat.x_axis.y, mat.x_axis.z);
    trace("(%.3f, %.3f, %.3f), ", mat.y_axis.x, mat.y_axis.y, mat.y_axis.z);
    trace("(%.3f, %.3f, %.3f), ", mat.z_axis.x, mat.z_axis.y, mat.z_axis.z);
    trace("(%.3f, %.3f, %.3f))\n", mat.trans.x, mat.trans.y, mat.trans.z);
}


static bool isEqual(const char* str1, const char* str2)
{
    for ( ; ; str1++, str2++)
    {
        if (*str1 != *str2)
        {
            return false;
        }

        if (*str1 == '\0')
        {
            return true;
        }
    }
}


void fsDebugMgr::dumpMemory()
{
    if (!m_instance)
    {
        return;
    }

    trace("` DumpMemory `\n");
    trace("+--------------------+-----+--------+\n");
    trace("|        NAME        |BLOCK|  SIZE  |\n");
    trace("+--------------------+-----+--------+\n");

    for (const void* mem1 = fsMemHelper::getFirstMemoryBlockN(); mem1; mem1 = fsMemHelper::getNextMemoryBlockN(mem1))
    {
        const char* name = fsMemHelper::getMemoryBlockName(mem1);

        bool is_continue = false;

        for (const void* mem2 = fsMemHelper::getFirstMemoryBlockN(); mem2 != mem1; mem2 = fsMemHelper::getNextMemoryBlockN(mem2))
        {
            if (isEqual(name, fsMemHelper::getMemoryBlockName(mem2)))
            {
                is_continue = true;
                break;
            }
        }

        if (is_continue)
        {
            continue;
        }

        u32 count = 1;
        u32 size = fsMemHelper::getMemoryBlockSize(mem1);

        for (const void* mem2 = fsMemHelper::getNextMemoryBlockN(mem1); mem2; mem2 = fsMemHelper::getNextMemoryBlockN(mem2))
        {
            if (isEqual(name, fsMemHelper::getMemoryBlockName(mem2)))
            {
                count++;
                size += fsMemHelper::getMemoryBlockSize(mem2);
            }
        }

        trace("|%-20s|%5d|%8d|\n", fsUtil::getBasename(name), count, size);
    }

    trace("+--------------------+-----+--------+\n");
    trace("|TOTAL SIZE  %18d byte|\n", fsMemHelper::getCurUsedMemorySize());
    trace("|MAX SIZE    %18d byte|\n", fsMemHelper::getMaxUsedMemorySize());
    trace("+-----------------------------------+\n");
}


void fsDebugMgr::dumpTask()
{
    if (!m_instance)
    {
        return;
    }

    trace("` DumpTask `\n");
    trace("+--------------------+-----+------+\n");
    trace("|        NAME        |ORDER| TIME |\n");
    trace("+--------------------+-----+------+\n");

    for (u32 order = fsTask::ORDER_MINUS_8_FOR_SYSTEM; order <= fsTask::ORDER_PLUS_8_FOR_SYSTEM; order++)
    {
        for (fsTask* task = fsTaskMgr::getFirstTaskN(static_cast<fsTask::TaskOrder>(order)); task; task = task->getNextAllN())
        {
            static char buf1[8];

            if (task->hasOrder())
            {
                fsMgr::sprintf(buf1, 8, "%5d", task->getOrder() - 8);
            }
            else if (task->getParentN() == task->getPrevAllN())
            {
                fsMgr::sprintf(buf1, 8, "CHILD");
            }
            else
            {
                fsMgr::sprintf(buf1, 8, "child");
            }

            static char buf2[8];

            if (task->isActive())
            {
                fsMgr::sprintf(buf2, 8, "%6d", task->getExecuteUsecTime());
            }
            else
            {
                fsMgr::sprintf(buf2, 8, "PAUSED");
            }

            trace("|%-20s|%5s|%6s|\n", fsUtil::getBasename(task->getName()), buf1, buf2);
        }
    }

    trace("|%-20s|%5d|%6d|\n", "RENDER", 9, fsTaskMgr::getRenderUsecTime());

    trace("+--------------------+-----+------+\n");
    trace("|TOTAL TIME   %15d usec|\n", fsTaskMgr::getExecuteUsecTime());
    trace("+---------------------------------+\n");
}


void fsDebugMgr::dumpResource()
{
    if (!m_instance)
    {
        return;
    }

    trace("` DumpResource `\n");
    trace("+--------+---+--------+----+\n");
    trace("|   ID   |EXT|  SIZE  |AUTO|\n");
    trace("+--------+---+--------+----+\n");

    for (const fsRes* res = fsResMgr::getFirstResourceN(); res; res = fsResMgr::getNextResourceN(res->getID()))
    {
        trace("|%08X|%3s|%8d|%4s|\n", res->getID().getValue(), res->getExtension().getString(), //
            res->getDataSize(), res->isAutoFree() ? "ON" : "OFF");
    }

    trace("+--------+---+--------+----+\n");
    trace("TYPE = { ");

    for (const fsStr<char, 3>* str = fsResMgr::getFirstTypeN(); str; str = fsResMgr::getNextTypeN(*str))
    {
        if (str != fsResMgr::getFirstTypeN())
        {
            trace(", ");
        }

        trace("\"%s\"", str->getString());
    }

    trace(" }\n");
}


void fsDebugMgr::dumpConfig()
{
    if (!m_instance)
    {
        return;
    }

    trace("` DumpConfig `\n");
    trace("+--------+-------+-----+\n");
    trace("|   ID   | STATE |ERROR|\n");
    trace("+--------+-------+-----+\n");

    for (fsScript* conf = fsScriptMgr::getFirstConfigN(); conf; conf = conf->getNextN())
    {
        trace("|%08X|%7s|%5d|\n", conf->getID().getValue(), conf->isValid() ? "VALID" : "INVALID", conf->isValid() ? 0 : conf->getErrorLineNo());
    }

    trace("+--------+-------+-----+\n");
}


void fsDebugMgr::dumpScreen()
{
    if (!m_instance)
    {
        return;
    }

    trace("` DumpScreen `\n");
    trace("+--------+---------+---------+----+\n");
    trace("|   ID   |   POS   |   SIZE  |FLAG|\n");
    trace("+--------+---------+---------+----+\n");

    for (fsScr* scr = fsDrawMgr::getFirstScreenN(); scr; scr = scr->getNextN())
    {
        trace("|%08X|%4d,%4d|%4dx%4d|%c%c%c%c|\n", scr->getID().getValue(), //
            scr->getLeftInFramebuffer(), scr->getTopInFramebuffer(), scr->getWidthInFramebuffer(), scr->getHeightInFramebuffer(), //
            scr->isActive() ? 'A' : '_', scr->isClearColor() ? 'C' : '_', scr->isClearDepth() ? 'D' : '_', scr->isPerspective() ? 'P' : '_');
    }

    trace("+--------+---------+---------+----+\n");
}


void fsDebugMgr::dumpTexture()
{
    if (!m_instance)
    {
        return;
    }

    static const char* s_format_name[] =
    {
        "RGB", "RGBA", "ALPHA", "PNG_RGB", "PNG_RGBA", "PNG_ALPHA"
    };

    static const char* s_mode_name[] =
    {
        "READ_ONLY", "READ_WRITE", "FRAMEBUFFER", "VOLATILE"
    };

    trace("` DumpTexture `\n");
    trace("+--------+---------+---------+-----------+\n");
    trace("|   ID   |   SIZE  |  FORMAT |    MODE   |\n");
    trace("+--------+---------+---------+-----------+\n");

    for (fsTex* tex = fsDrawMgr::getFirstTextureN(); tex; tex = tex->getNextN())
    {
        trace("|%08X|%4dx%4d|%9s|%11s|\n", tex->getID().getValue(), tex->getWidth(), tex->getHeight(), //
            s_format_name[tex->getFormat()], s_mode_name[tex->getMode()]);
    }

    trace("+--------+---------+---------+-----------+\n");
}


void fsDebugMgr::dumpShader()
{
    if (!m_instance)
    {
        return;
    }

    trace("` DumpShader `\n");
    trace("+--------+-------------------+\n");
    trace("| SHADER: %-11s        |\n", fsDrawMgr::isShaderAvailable() ? "AVAILABLE" : "UNAVAILABLE");
    trace("+--------+---+---+---+-------+\n");
    trace("|   ID   |UNI|ATT|TEX| STATE |\n");
    trace("+--------+---+---+---+-------+\n");

    for (fsShd* shd = fsDrawMgr::getFirstShaderN(); shd; shd = shd->getNextN())
    {
        trace("|%08X|%3d|%3d|%3d|%7s|\n", shd->getID().getValue(), shd->getUniformNum(), shd->getAttribNum(), //
            shd->getTextureNum(), shd->isValid() ? "VALID" : "INVALID");
    }

    trace("+--------+---+---+---+-------+\n");
}
