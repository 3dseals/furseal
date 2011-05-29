/*
 * mass_font.cpp
 *
 *  Created on: 2011-5-29
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */


#include "fs.h"


class MassFont : public fsTask
{
public:
    MassFont();

private:
    static const u16 BASE_FONT_SIZE = 16;
    static const u16 GRID_SIZE = 12;

    void initMassFont();
    virtual void onUpdate();

    fsFont m_mass_font;
    fsPrim m_mass_prim;
};


void newMassFont()
{
    fsNewTask(MassFont);
}


MassFont::MassFont() : fsTask(ORDER_ZERO)
{
    initMassFont();
}


void MassFont::initMassFont()
{
    const char str[2] =
    {
        static_cast<char>(fsMath::rand('A', 'Z')), '\0'
    };

    fsDrawMgr::setFont(fsID_("stonsans.ttf"), 0);
    fsDrawMgr::setFontSize(BASE_FONT_SIZE);

    fsFont master_font;
    master_font.init(BASE_FONT_SIZE, BASE_FONT_SIZE);

    u16 base_font_width = master_font.drawString(0, 0, str);
    u16 base_font_height = master_font.getHeight();

    fsTex* tex = fsDrawMgr::getTexture(master_font.getTextureID());
    const u8* image = static_cast<const u8*>(tex->getImage());

    m_mass_font.init(base_font_width * GRID_SIZE, base_font_height * GRID_SIZE);

    for (s32 i = 0; i < base_font_height; i++)
    {
        for (s32 j = 0; j < base_font_width; j++)
        {
            if (*(image + i * BASE_FONT_SIZE + j) > 128)
            {
                m_mass_font.drawString(j * GRID_SIZE, i * GRID_SIZE, str);
            }
        }
    }

    m_mass_prim.init(fsPrim::MODE_TRIANGLE_FAN, 4, fsDrawMgr::DEFAULT_3D_SCREEN_ID);
    m_mass_prim.setTextureID(m_mass_font.getTextureID());
    m_mass_prim.setBlendMode(fsDraw::BLEND_ADD, true);

    r32 mass_prim_width = static_cast<r32>(base_font_width * GRID_SIZE);
    r32 mass_prim_height = static_cast<r32>(base_font_height * GRID_SIZE);

    m_mass_prim.setDataRect(0, fsVec::ZERO, mass_prim_width, mass_prim_height, //
        fsCol(fsMath::rand(64, 255), fsMath::rand(64, 255), fsMath::rand(64, 255)), 0.0f, 0.0f, 1.0f, 1.0f);

    m_mass_prim.local() = m_mass_prim.local().rotateY_s32(fsMath::rand(0, 360));
    m_mass_prim.local().trans.set( //
        fsMath::rand(-200.0f, 200.0f, 20.0f), fsMath::rand(-150.0f, 150.0f, 20.0f), fsMath::rand(-2000.0f, 1000.0f, 100.0f));

    m_mass_prim.setColor(fsCol(255, 255, 255, 0));
}


void MassFont::onUpdate()
{
    m_mass_prim.local() = m_mass_prim.local().rotateY_s32(1);
    m_mass_prim.local().trans.z += 2.0f;

    fsCol col = m_mass_prim.getColor();

    if (col.a < 160)
    {
        col.a += 4;
        m_mass_prim.setColor(col);
    }

    if (m_mass_prim.local().trans.z > 1000.0f)
    {
        initMassFont();
    }
}
