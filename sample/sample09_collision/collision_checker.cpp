/*
 * collision_checker.cpp
 *
 *  Created on: 2011-5-29
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */


#include "fs.h"
#include "collision_target.h"


class CollisionChefser : public fsTask
{
public:
    CollisionChefser();

private:
    enum MouseState
    {
        STATE_NORMAL, //
        STATE_DRAG_L, //
        STATE_DRAG_M, //
        STATE_DRAG_R
    };

    void resetPos();
    virtual void onUpdate();

    fsScr* m_scr;
    fsMat m_view_rot;
    r32 m_view_dist;

    r32 m_mouse_x;
    r32 m_mouse_y;
    MouseState m_mouse_state;
    u8 m_drag_no;

    fsMat m_tgt_world[2];
    fsVec m_tgt_size[2];
    CollisionTarget::Target1 m_tgt1;
    CollisionTarget::Target2 m_tgt2;

    bool m_is_push_bafs;
};


void newCollisionChefser()
{
    fsNewTask(CollisionChefser);
}


CollisionChefser::CollisionChefser() : fsTask(ORDER_ZERO)
{
    m_scr = fsDrawMgr::getScreen(fsDrawMgr::DEFAULT_3D_SCREEN_ID);

    m_tgt1.type = CollisionTarget::TARGET1_SPHERE;
    m_tgt2.type = CollisionTarget::TARGET2_BOX;

    m_is_push_bafs = false;

    resetPos();
}


void CollisionChefser::resetPos()
{
    m_view_rot = fsMat::UNIT;
    m_view_dist = m_scr->getFocusDist();

    m_mouse_state = STATE_NORMAL;

    m_tgt_world[0] = fsMat::UNIT.translate(-180.0f, 0.0f, 0.0f);
    m_tgt_world[1] = fsMat::UNIT.translate(180.0f, 0.0f, 0.0f);

    m_tgt_size[0].set(200.0f, 200.0f, 200.0f);
    m_tgt_size[1].set(200.0f, 200.0f, 200.0f);
}


void CollisionChefser::onUpdate()
{
    if (fsInputMgr::isPressed(fsInputMgr::KEY_Q))
    {
        fsStopFurseal();
    }

    if (fsInputMgr::isPressed(fsInputMgr::KEY_A))
    {
        m_tgt1.type = static_cast<CollisionTarget::Target1Type>((m_tgt1.type + 1) % CollisionTarget::TARGET1_NUM);
    }

    if (fsInputMgr::isPressed(fsInputMgr::KEY_B))
    {
        m_tgt2.type = static_cast<CollisionTarget::Target2Type>((m_tgt2.type + 1) % CollisionTarget::TARGET2_NUM);
    }

    if (fsInputMgr::isPressed(fsInputMgr::KEY_P))
    {
        m_is_push_bafs = !m_is_push_bafs;
    }

    if (fsInputMgr::isPressed(fsInputMgr::KEY_R))
    {
        resetPos();
    }

    if (fsMgr::isFramebufferSizeChanged())
    {
        m_scr->setAreaInFramebuffer(0, 0, fsMgr::getFramebufferWidth(), fsMgr::getFramebufferHeight());
        m_scr->setViewSize(fsMgr::getFramebufferWidth(), fsMgr::getFramebufferHeight());
    }

    r32 last_mouse_x = m_mouse_x;
    r32 last_mouse_y = m_mouse_y;

    m_mouse_x = m_scr->framebufferXToScreenX(fsInputMgr::getMouseX());
    m_mouse_y = m_scr->framebufferYToScreenY(fsInputMgr::getMouseY());

    /*
        check hit
    */
    fsVec hit_to(m_mouse_x, m_mouse_y, -m_scr->getFocusDist());
    hit_to *= 10.0f;
    hit_to = hit_to.toGlobalFrom(m_scr->view());

    fsCdt::Ray hit_ray;
    hit_ray.setPos(m_scr->view().trans, hit_to);

    u8 hit_no = CollisionTarget::checkTargetHit(m_tgt1, hit_ray) ? 1 : (CollisionTarget::checkTargetHit(m_tgt2, hit_ray) ? 2 : 0);

    /*
        drag object/camera
    */
    bool is_l_pressed = fsInputMgr::isPressed(fsInputMgr::KEY_LBUTTON);
    bool is_m_pressed = fsInputMgr::isPressed(fsInputMgr::KEY_MBUTTON);
    bool is_r_pressed = fsInputMgr::isPressed(fsInputMgr::KEY_RBUTTON);

    if (is_l_pressed || is_m_pressed || is_r_pressed)
    {
        last_mouse_x = m_mouse_x;
        last_mouse_y = m_mouse_y;

        m_mouse_state = is_l_pressed ? STATE_DRAG_L : (is_m_pressed ? STATE_DRAG_M : STATE_DRAG_R);
        m_drag_no = hit_no;
    }

    if (m_mouse_state != STATE_NORMAL)
    {
        r32 drag_x = m_mouse_x - last_mouse_x;
        r32 drag_y = m_mouse_y - last_mouse_y;

        fsMat* tgt_world = (m_drag_no > 0) ? &m_tgt_world[m_drag_no - 1] : NULL;
        fsVec* tgt_size = (m_drag_no > 0) ? &m_tgt_size[m_drag_no - 1] : NULL;

        if (m_mouse_state == STATE_DRAG_L)
        {
            if (fsInputMgr::isOn(fsInputMgr::KEY_LBUTTON))
            {
                if (m_drag_no > 0)
                {
                    r32 rel_z = tgt_world->trans.toLocalOf(m_scr->view()).z;

                    tgt_world->trans += (m_view_rot.x_axis * drag_x + m_view_rot.y_axis * drag_y) * -rel_z / m_scr->getFocusDist();
                }
                else
                {
                    fsVec vec = (m_view_rot.x_axis * drag_x + m_view_rot.y_axis * drag_y) * m_view_dist / m_scr->getFocusDist();

                    m_tgt_world[0].trans += vec;
                    m_tgt_world[1].trans += vec;
                }
            }
            else
            {
                m_mouse_state = STATE_NORMAL;
            }
        }
        else if (m_mouse_state == STATE_DRAG_M)
        {
            if (fsInputMgr::isOn(fsInputMgr::KEY_MBUTTON))
            {
                if (m_drag_no > 0)
                {
                    tgt_size->x = fsMath::clamp(tgt_size->x + drag_x, 10.0f, 1000.0f);
                    tgt_size->y = fsMath::clamp(tgt_size->y + drag_y, 10.0f, 1000.0f);
                    tgt_size->z = fsMath::clamp(tgt_size->z + drag_y / 2.0f, 10.0f, 1000.0f);
                }
                else
                {
                    m_view_dist = fsMath::max(m_view_dist + drag_y * -5.0f, 500.0f);
                }
            }
            else
            {
                m_mouse_state = STATE_NORMAL;
            }
        }
        else if (m_mouse_state == STATE_DRAG_R)
        {
            if (fsInputMgr::isOn(fsInputMgr::KEY_RBUTTON))
            {
                if (m_drag_no > 0)
                {
                    fsVec trans = tgt_world->trans;
                    *tgt_world = tgt_world->toLocalOf_noTrans(m_view_rot);
                    *tgt_world = tgt_world->toGlobalFrom_noTrans(m_view_rot.rotateX_r32(-drag_y).rotateY_r32(drag_x));
                    tgt_world->trans = trans;
                }
                else
                {
                    m_view_rot = m_view_rot.rotateX_r32(drag_y).rotateY_r32(-drag_x);
                }
            }
            else
            {
                m_mouse_state = STATE_NORMAL;
            }
        }
    }

    m_view_dist = fsMath::max(m_view_dist - fsInputMgr::getMouseWheel() * 100.0f, 500.0f);

    m_scr->view() = m_view_rot;
    m_scr->view().trans = m_view_rot.z_axis * m_view_dist;

    CollisionTarget::updateTarget(&m_tgt1, m_tgt_world[0], m_tgt_size[0]);
    CollisionTarget::updateTarget(&m_tgt2, m_tgt_world[1], m_tgt_size[1]);

    /*
        check collision
    */
    if (m_tgt1.type == CollisionTarget::TARGET1_RAY)
    {
        fsVec pos;

        if (CollisionTarget::intersect(&pos, m_tgt1, m_tgt2))
        {
            CollisionTarget::drawPos(pos, fsCol(0, 255, 255));

            if (m_is_push_bafs)
            {
                m_tgt_world[0].trans += pos - m_tgt1.ray.getTo();
                CollisionTarget::updateTarget(&m_tgt1, m_tgt_world[0], m_tgt_size[0]);
            }
        }
    }
    else
    {
        fsCdt::CdtInfo cdt_info;

        if (CollisionTarget::collide(&cdt_info, m_tgt1, m_tgt2))
        {
            CollisionTarget::drawPos(cdt_info.pos, fsCol(0, 255, 255));
            fsDebugMgr::drawLine(cdt_info.pos, cdt_info.pos + cdt_info.bafs_dir * cdt_info.bafs_dist, fsCol(255, 0, 255), fsDrawMgr::DEFAULT_3D_SCREEN_ID);

            if (m_is_push_bafs)
            {
                m_tgt_world[0].trans += cdt_info.bafs_dir * cdt_info.bafs_dist;
                CollisionTarget::updateTarget(&m_tgt1, m_tgt_world[0], m_tgt_size[0]);
            }
        }
    }

    /*
        draw objects
    */
    fsCol tgt1_col = ((m_mouse_state == STATE_NORMAL && hit_no == 1) || (m_mouse_state != STATE_NORMAL && m_drag_no == 1)) ? fsCol(192, 64, 0, 128) : fsCol(0, 255, 128, 64);
    fsCol tgt2_col = ((m_mouse_state == STATE_NORMAL && hit_no == 2) || (m_mouse_state != STATE_NORMAL && m_drag_no == 2)) ? fsCol(255, 64, 0, 128) : fsCol(128, 128, 255, 64);
    fsCol line_col = m_is_push_bafs ? fsCol(255, 192, 64, 255) : fsCol(255, 255, 255, 128);
    fsCol aabb_col(0, 255, 0, 48);

    CollisionTarget::drawTarget(m_tgt1, tgt1_col, line_col, aabb_col);
    CollisionTarget::drawTarget(m_tgt2, tgt2_col, line_col, aabb_col);
    CollisionTarget::drawAxis(fsCol(255, 128, 128, 64));
}
