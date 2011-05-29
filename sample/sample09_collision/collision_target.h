/*
 * collision_target.h
 *
 *  Created on: 2011-5-29
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */


#ifndef COLLISION_TARGET_H_
#define COLLISION_TARGET_H_


class CollisionTarget
{
public:
    enum Target1Type
    {
        TARGET1_SPHERE, //
        TARGET1_BOX, //
        TARGET1_RAY, //
        TARGET1_NUM
    };

    enum Target2Type
    {
        TARGET2_SPHERE, //
        TARGET2_BOX, //
        TARGET2_TRIANGLE, //
        TARGET2_NUM
    };

    struct Target1
    {
        Target1Type type;
        fsCdt::Sph sph;
        fsCdt::Box box;
        fsCdt::Ray ray;
    };

    struct Target2
    {
        Target2Type type;
        fsCdt::Sph sph;
        fsCdt::Box box;
        fsCdt::Tri tri;
    };

    static bool chefsTargetHit(const Target1& tgt1, const fsCdt::Ray& hit_ray);
    static bool chefsTargetHit(const Target2& tgt2, const fsCdt::Ray& hit_ray);

    static void updateTarget(Target1* tgt1, const fsMat& world, const fsVec& size);
    static void updateTarget(Target2* tgt2, const fsMat& world, const fsVec& size);

    static void drawTarget(const Target1& tgt1, fsCol poly_col, fsCol line_col, fsCol aabb_col);
    static void drawTarget(const Target2& tgt2, fsCol poly_col, fsCol line_col, fsCol aabb_col);

    static bool collide(fsCdt::CdtInfo* cdt_info, const Target1& tgt1, const Target2& tgt2);
    static bool intersect(fsVec* pos, const Target1& tgt1, const Target2& tgt2);

    static void drawPos(const fsVec& pos, fsCol col);
    static void drawAxis(fsCol col);
};


#endif // !COLLISION_TARGET_H_
