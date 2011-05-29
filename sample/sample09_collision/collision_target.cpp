/*
 * collision_target.cpp
 *
 *  Created on: 2011-5-29
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */


#include "fs.h"
#include "collision_target.h"


static void drawSphere(const fsCdt::Sph sph, fsCol poly_col, fsCol line_col)
{
    fsDebugMgr::drawSphere(sph.getPos(), sph.getRadius(), poly_col, line_col, fsDrawMgr::DEFAULT_3D_SCREEN_ID);
}


static void drawBox(const fsCdt::Box box, fsCol poly_col, fsCol line_col)
{
    fsDebugMgr::drawBox(box.getWorld(), box.getHalfSize() * 2.0f, poly_col, line_col, fsDrawMgr::DEFAULT_3D_SCREEN_ID);
}


static void drawTriangle(const fsCdt::Tri tri, fsCol poly_col, fsCol line_col)
{
    fsDebugMgr::drawLine(tri.getPos1(), tri.getPos2(), line_col, fsDrawMgr::DEFAULT_3D_SCREEN_ID);
    fsDebugMgr::drawLine(tri.getPos2(), tri.getPos3(), line_col, fsDrawMgr::DEFAULT_3D_SCREEN_ID);
    fsDebugMgr::drawLine(tri.getPos3(), tri.getPos1(), line_col, fsDrawMgr::DEFAULT_3D_SCREEN_ID);
    fsDebugMgr::drawPolygon(tri.getPos1(), tri.getPos2(), tri.getPos3(), poly_col, fsDrawMgr::DEFAULT_3D_SCREEN_ID);
}


static void drawRay(const fsCdt::Ray ray, fsCol poly_col, fsCol line_col)
{
    fsDebugMgr::drawLine(ray.getFrom(), ray.getTo(), line_col, fsDrawMgr::DEFAULT_3D_SCREEN_ID);
    fsDebugMgr::drawSphere(ray.getFrom(), 4.0f, poly_col, line_col, fsDrawMgr::DEFAULT_3D_SCREEN_ID);
}


static void drawAABB(const fsCdt::AABB aabb, fsCol line_col)
{
    fsMat world = fsMat::UNIT;
    world.trans = (aabb.getMin() + aabb.getMax()) / 2.0f;

    fsDebugMgr::drawBox(world, aabb.getMax() - aabb.getMin(), fsCol::ZERO, line_col, fsDrawMgr::DEFAULT_3D_SCREEN_ID);
}


bool CollisionTarget::chefsTargetHit(const Target1& tgt1, const fsCdt::Ray& hit_ray)
{
    switch (tgt1.type)
    {
    case TARGET1_SPHERE:
        return fsCdt::intersect(NULL, hit_ray, tgt1.sph);

    case TARGET1_BOX:
        return fsCdt::intersect(NULL, hit_ray, tgt1.box);

    case TARGET1_RAY:
        {
            fsMat ray_world;
            ray_world.x_axis = (tgt1.ray.getFrom() - tgt1.ray.getTo()).normalize();
            ray_world.y_axis = ((fsMath::abs(ray_world.x_axis.dot(fsVec::Y_UNIT)) < 0.7) ? fsVec::Y_UNIT : fsVec::Z_UNIT).cross(ray_world.x_axis);
            ray_world.z_axis = ray_world.x_axis.cross(ray_world.y_axis);
            ray_world.trans = (tgt1.ray.getFrom() + tgt1.ray.getTo()) / 2.0f;

            fsCdt::Box ray_box;
            ray_box.setWorld(ray_world);
            ray_box.setSize(tgt1.ray.getFrom().dist(tgt1.ray.getTo()), 20.0f, 20.0f);

            return fsCdt::intersect(NULL, hit_ray, ray_box);
        }

    default:
        return false;
    }
}


bool CollisionTarget::chefsTargetHit(const Target2& tgt2, const fsCdt::Ray& hit_ray)
{
    switch (tgt2.type)
    {
    case TARGET2_SPHERE:
        return fsCdt::intersect(NULL, hit_ray, tgt2.sph);

    case TARGET2_BOX:
        return fsCdt::intersect(NULL, hit_ray, tgt2.box);

    case TARGET2_TRIANGLE:
        return fsCdt::intersect(NULL, hit_ray, tgt2.tri);

    default:
        return false;
    }
}


void CollisionTarget::updateTarget(Target1* tgt1, const fsMat& world, const fsVec& size)
{
    tgt1->sph.setPos(world.trans);
    tgt1->sph.setRadius(size.y / 2.0f);

    tgt1->box.setWorld(world);
    tgt1->box.setSize(size.x, size.y, size.z);

    fsVec vec_y = world.y_axis * (size.y / 1.5f);

    tgt1->ray.setPos(world.trans - vec_y, world.trans + vec_y);
}


void CollisionTarget::updateTarget(Target2* tgt2, const fsMat& world, const fsVec& size)
{
    tgt2->sph.setPos(world.trans);
    tgt2->sph.setRadius(size.x / 2.0f);

    tgt2->box.setWorld(world);
    tgt2->box.setSize(size.x, size.y, size.z);

    fsVec vec_x = world.x_axis * (size.x / 1.5f);
    fsVec vec_y = world.y_axis * (size.y / 1.5f);

    tgt2->tri.setPos(world.trans + vec_y, world.trans + vec_x - vec_y, world.trans - vec_x - vec_y);
}


void CollisionTarget::drawTarget(const Target1& tgt1, fsCol poly_col, fsCol line_col, fsCol aabb_col)
{
    switch (tgt1.type)
    {
    case TARGET1_SPHERE:
        drawSphere(tgt1.sph, poly_col, line_col);
        drawAABB(tgt1.sph.getAABB(), aabb_col);
        break;

    case TARGET1_BOX:
        drawBox(tgt1.box, poly_col, line_col);
        drawAABB(tgt1.box.getAABB(), aabb_col);
        break;

    case TARGET1_RAY:
        drawRay(tgt1.ray, poly_col, line_col);
        drawAABB(tgt1.ray.getAABB(), aabb_col);
        break;

    default:
        break;
    }
}


void CollisionTarget::drawTarget(const Target2& tgt2, fsCol poly_col, fsCol line_col, fsCol aabb_col)
{
    switch (tgt2.type)
    {
    case TARGET2_SPHERE:
        drawSphere(tgt2.sph, poly_col, line_col);
        drawAABB(tgt2.sph.getAABB(), aabb_col);
        break;

    case TARGET2_BOX:
        drawBox(tgt2.box, poly_col, line_col);
        drawAABB(tgt2.box.getAABB(), aabb_col);
        break;

    case TARGET2_TRIANGLE:
        drawTriangle(tgt2.tri, poly_col, line_col);
        drawAABB(tgt2.tri.getAABB(), aabb_col);
        break;

    default:
        break;
    }
}


bool CollisionTarget::collide(fsCdt::CdtInfo* cdt_info, const Target1& tgt1, const Target2& tgt2)
{
    if (tgt1.type == TARGET1_SPHERE)
    {
        switch (tgt2.type)
        {
        case TARGET2_SPHERE:
            return fsCdt::collide(cdt_info, tgt1.sph, tgt2.sph);

        case TARGET2_BOX:
            return fsCdt::collide(cdt_info, tgt1.sph, tgt2.box);

        case TARGET2_TRIANGLE:
            return fsCdt::collide(cdt_info, tgt1.sph, tgt2.tri);

        default:
            return false;
        }
    }
    else if (tgt1.type == TARGET1_BOX)
    {
        switch (tgt2.type)
        {
        case TARGET2_SPHERE:
            return fsCdt::collide(cdt_info, tgt1.box, tgt2.sph);

        case TARGET2_BOX:
            return fsCdt::collide(cdt_info, tgt1.box, tgt2.box);

        case TARGET2_TRIANGLE:
            return fsCdt::collide(cdt_info, tgt1.box, tgt2.tri);

        default:
            return false;
        }
    }

    return false;
}


bool CollisionTarget::intersect(fsVec* pos, const Target1& tgt1, const Target2& tgt2)
{
    if (tgt1.type == TARGET1_RAY)
    {
        switch (tgt2.type)
        {
        case TARGET2_SPHERE:
            return fsCdt::intersect(pos, tgt1.ray, tgt2.sph);

        case TARGET2_BOX:
            return fsCdt::intersect(pos, tgt1.ray, tgt2.box);

        case TARGET2_TRIANGLE:
            return fsCdt::intersect(pos, tgt1.ray, tgt2.tri);

        default:
            return false;
        }
    }

    return false;
}


void CollisionTarget::drawPos(const fsVec& pos, fsCol col)
{
    fsScr* scr = fsDrawMgr::getScreen(fsDrawMgr::DEFAULT_3D_SCREEN_ID);

    r32 rel_z = pos.toLocalOf(scr->view()).z;
    fsVec vec_x = scr->view().x_axis * (8.0f * -rel_z / scr->getFocusDist());
    fsVec vec_y = scr->view().y_axis * (8.0f * -rel_z / scr->getFocusDist());

    fsDebugMgr::drawLine(pos + vec_x + vec_y, pos - vec_x - vec_y, col, fsDrawMgr::DEFAULT_3D_SCREEN_ID);
    fsDebugMgr::drawLine(pos - vec_x + vec_y, pos + vec_x - vec_y, col, fsDrawMgr::DEFAULT_3D_SCREEN_ID);
}


void CollisionTarget::drawAxis(fsCol col)
{
    const r32 AXIS_LENGTH = 10000.0f;

    fsDebugMgr::drawLine(fsVec::X_UNIT * AXIS_LENGTH, fsVec::X_UNIT * -AXIS_LENGTH, col, fsDrawMgr::DEFAULT_3D_SCREEN_ID);
    fsDebugMgr::drawLine(fsVec::Y_UNIT * AXIS_LENGTH, fsVec::Y_UNIT * -AXIS_LENGTH, col, fsDrawMgr::DEFAULT_3D_SCREEN_ID);
    fsDebugMgr::drawLine(fsVec::Z_UNIT * AXIS_LENGTH, fsVec::Z_UNIT * -AXIS_LENGTH, col, fsDrawMgr::DEFAULT_3D_SCREEN_ID);
}
