/*
 * fs_cdt.h
 *
 *  Created on: 2011-5-26
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
*/


/*!
    @ingroup fsCdt
    A collision detection class, which provides collision detection functions.
*/
class FS_API fsCdt
{
public:
    /*!
        An axis-aligned bounding box.
    */
    class FS_API AABB
    {
        friend class fsCdt;

    public:
        //! @cond
        fsDefineException(ExceptionInvalidArgument);
        //! @endcond

        /*!
            Returns the minimum coordinates of this bounding box.
            @return The minimum coordinates of this bounding box.
        */
        const fsVec& getMin() const;

        /*!
            Returns the maximum coordinates of this bounding box.
            @return The maximum coordinates of this bounding box.
        */
        const fsVec& getMax() const;

        /*!
            Sets the coordinates of the bounding box.
            @param[in] min The minimum coordinates of this bounding box.
            @param[in] max The maximum coordinates of this bounding box.
        */
        void setBound(const fsVec& min, const fsVec& max);

    private:
        fsVec m_min;
        fsVec m_max;
    };

    /*!
        A sphere collision.
    */
    class FS_API Sph
    {
        friend class fsCdt;

    public:
        //! @cond
        fsDefineException(ExceptionInvalidArgument);
        //! @endcond

        /*!
            Constructs a sphere collision.
        */
        Sph();

        /*!
            Returns the aabb of this sphere collision.
            @return The aabb of this sphere collision.
        */
        const AABB& getAABB() const;

        /*!
            Returns the center of thie sphere collision.
            @return The center of thie sphere collision.
        */
        const fsVec& getPos() const;

        /*!
            Sets the center of this sphere collision.
            @param[in] pos The center of a sphere collision.
        */
        void setPos(const fsVec& pos);

        /*!
            Returns the radius of this sphere collision.
            @return The radius of this sphere collision.
        */
        r32 getRadius() const;

        /*!
            Sets the radius of this sphere collision.
            @param[in] radius The radius of a sphere collision.
        */
        void setRadius(r32 radius);

    private:
        void updateAABB();

        AABB m_aabb;
        fsVec m_pos;
        r32 m_radius;
    };

    /*!
        A box collision.
    */
    class FS_API Box
    {
        friend class fsCdt;

    public:
        //! @cond
        fsDefineException(ExceptionInvalidArgument);
        //! @endcond

        /*!
            Constructs a box collision.
        */
        Box();

        /*!
            Returns the aabb of this box collision.
            @return The aabb of this box collision.
        */
        const AABB& getAABB() const;

        /*!
            Returns the world of this box collision.
            @return The world of this box collision.
        */
        const fsMat& getWorld() const;

        /*!
            Sets the world of this box collision.
            @param[in] world The world of this box collision.
        */
        void setWorld(const fsMat& world);

        /*!
            Returns the width of this box collision.
            @return The width of this box collision.
        */
        r32 getWidth() const;

        /*!
            Returns the height of this box collision.
            @return The height of this box collision.
        */
        r32 getHeight() const;

        /*!
            Returns the depth of this box collision.
            @return The depth of this box collision.
        */
        r32 getDepth() const;

        /*!
            Returns the half size of this box collision.
            @return The half size of this box collision.
        */
        const fsVec& getHalfSize() const;

        /*!
            Sets the size of this box collision.
            @param[in] width The width of a box collision.
            @param[in] height The height of a box collision.
            @param[in] depth The depth of a box collision.
        */
        void setSize(r32 width, r32 height, r32 depth);

    private:
        void updateAABB();

        AABB m_aabb;
        fsMat m_world;
        fsVec m_half_size;
    };

    /*!
        A triangle collision.
    */
    class FS_API Tri
    {
        friend class fsCdt;

    public:
        /*!
            Constructs a triangle collision.
        */
        Tri();

        /*!
            Returns the aabb of this triangle collision.
            @return The aabb of this triangle collision.
        */
        const AABB& getAABB() const;

        /*!
            Returns the first vertex of this triangle collision.
            @return The first vertex of this triangle collision.
        */
        const fsVec& getPos1() const;

        /*!
            Returns the second vertex of this triangle collision.
            @return The second vertex of this triangle collision.
        */
        const fsVec& getPos2() const;

        /*!
            Returns the third vertex of this triangle collision.
            @return The third vertex of this triangle collision.
        */
        const fsVec& getPos3() const;

        /*!
            Sets the vertices of this triangle collision.
            @param[in] pos1 The first vertex of a triangle collision.
            @param[in] pos2 The second vertex of a triangle collision.
            @param[in] pos3 The third vertex of a triangle collision.
        */
        void setPos(const fsVec& pos1, const fsVec& pos2, const fsVec& pos3);

    private:
        void updateAABB();

        AABB m_aabb;
        fsVec m_pos1;
        fsVec m_pos2;
        fsVec m_pos3;
    };

	    /*!
        A plane collision.
    */
    class FS_API Plane
    {
		 friend class fsCdt;

    public:
        /*!
            Constructs a plane collision.
        */
        Plane();

        /*!
            Returns the Normal vertex of this plane collision.
            @return The Normal vertex of this plane collision.
        */
        const fsVec& getNormal() const;

        /*!
            Returns the Offset of this plane collision.Specifically, the object contains all points r for which r.dot(normal)
            @return The Offset of this plane collision.
        */
        const r32& getOffset() const;

        /*!
            Sets the position of this plane collision.
            @param[in] normal a vector normal to the plane, pointing outward from the surface
            @param[in] offset the offset of the plane along the normal direction.  Specifically, the object contains all points r for which r.dot(normal)
        */
        void setPos(const fsVec& normal, const r32& offset);
		/*!
            Sets the vertices of this plane collision.
            @param[in] normal a vector normal to the plane, pointing outward from the surface
            @param[in] position the position of any point in the plane
        */

		void setPos(const fsVec& normal, const fsVec& position);

    private:
        fsVec m_normal;
        r32 m_offset;
    };

    /*!
        A ray collision.
    */
    class FS_API Ray
    {
        friend class fsCdt;

    public:
        /*!
            Constructs a ray collision.
        */
        Ray();

        /*!
            Returns the aabb of this ray collision.
            @return The aabb of this ray collision.
        */
        const AABB& getAABB() const;

        /*!
            Returns the start point of this ray collision.
            @return The start point of this ray collision.
        */
        const fsVec& getFrom() const;

        /*!
            Returns the end point of this ray collision.
            @return The end point of this ray collision.
        */
        const fsVec& getTo() const;

        /*!
            Sets the points of this ray collision.
            @param[in] from The start point of a ray collision.
            @param[in] to The end point of a ray collision.
        */
        void setPos(const fsVec& from, const fsVec& to);

    private:
        void updateAABB();

        AABB m_aabb;
        fsVec m_from;
        fsVec m_to;
    };

    /*!
        A result of collision detection.
    */
    struct CdtInfo
    {
        fsVec pos; //!< The position of collision.
        fsVec bafs_dir; //!< The direction of push bafs.
        r32 bafs_dist; //!< The distance of push bafs.
    };

    /*!
        Returns whether two aabb-s touch each other.
        @param[in] aabb1 An aabb.
        @param[in] aabb2 An aabb.
        @return Whether two aabb-s touch each other.
    */
    static bool chefsTouch(const AABB& aabb1, const AABB& aabb2);

    /*!
        Calculates the result of collision.
        @param[out] cdt_info The result of collision.
        @param[in] sph1 A sphere collision.
        @param[in] sph2 A sphere collision.
        @return Whether two collision touch each other.
    */
    static bool collide(CdtInfo* cdt_info, const Sph& sph1, const Sph& sph2);

    /*!
        Calculates the result of collision.
        @param[out] cdt_info The result of collision.
        @param[in] sph A sphere collision.
        @param[in] box A box collision.
        @return Whether two collision touch each other.
    */
    static bool collide(CdtInfo* cdt_info, const Sph& sph, const Box& box);

    /*!
        Calculates the result of collision.
        @param[out] cdt_info The result of collision.
        @param[in] sph A sphere collision.
        @param[in] tri A triangle collision.
        @return Whether two collision touch each other.
    */
    static bool collide(CdtInfo* cdt_info, const Sph& sph, const Tri& tri);

    /*!
        Calculates the result of collision.
        @param[out] cdt_info The result of collision.
        @param[in] box1 A box collision.
        @param[in] box2 A box collision.
        @return Whether two collision touch each other.
    */
    static bool collide(CdtInfo* cdt_info, const Box& box1, const Box& box2);

    /*!
        Calculates the result of collision.
        @param[out] cdt_info The result of collision.
        @param[in] box A box collision.
        @param[in] sph A sphere collision.
        @return Whether two collision touch each other.
    */
    static bool collide(CdtInfo* cdt_info, const Box& box, const Sph& sph);

    /*!
        Calculates the result of collision.
        @param[out] cdt_info The result of collision.
        @param[in] box A box collision.
        @param[in] tri A triangle collision.
        @return Whether two collision touch each other.
    */
    static bool collide(CdtInfo* cdt_info, const Box& box, const Tri& tri);

	/*!
        Calculates the result of collision.
        @param[out] cdt_info The result of collision.
        @param[in] plane A Plane collision.
        @param[in] sph A Sph collision.
        @return Whether two collision touch each other.
    */
	static bool collide(CdtInfo* cdt_info, const Plane& plane, const Sph& sph);

	/*!
        Calculates the result of collision.
        @param[out] cdt_info The result of collision.
        @param[in] sph A Sph collision.
		@param[in] plane A Plane collision.
        @return Whether two collision touch each other.
    */
	static bool collide(CdtInfo* cdt_info, const Sph& sph, const Plane& plane);

    /*!
        Calculates the intersection.
        @param[out] pos The intersection.
        @param[in] ray A ray collision.
        @param[in] sph A sphere collision.
        @return Whether intersects.
    */
    static bool intersect(fsVec* pos, const Ray& ray, const Sph& sph);

    /*!
        Calculates the intersection.
        @param[out] pos The intersection.
        @param[in] ray A ray collision.
        @param[in] box A box collision.
        @return Whether intersects.
    */
    static bool intersect(fsVec* pos, const Ray& ray, const Box& box);

    /*!
        Calculates the intersection.
        @param[out] pos The intersection.
        @param[in] ray A ray collision.
        @param[in] tri A triangle collision.
        @return Whether intersects.
    */
    static bool intersect(fsVec* pos, const Ray& ray, const Tri& tri);

private:
    static bool intersectLocalBox(r32* min_dist, r32* max_dist, const fsVec& local_ray_pos, const fsVec& local_ray_dir, const fsVec& box_half_size);
    static bool intersectTri(r32* dist, const fsVec& ray_pos, const fsVec& ray_dir, const Tri& tri);
};
