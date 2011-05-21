/*
 * fs_draw.h
 *
 *  Created on: 2011-3-10
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */

#ifndef FS_DRAW_H_
#define FS_DRAW_H_


/*!
    @ingroup fsDraw
    绘图基类, 其他绘图类的基类.
*/
class FS_API fsDraw
{
    friend class fsScr;
    friend class fsDrawMgr;

public:
    //! @cond
    fsDefineException(ExceptionInvalidArgument);
    fsDefineException(ExceptionInvalidCall);
    fsDefineException(ExceptionNotInitialized);
    //! @endcond

    /*!
        绘图对象的类型.
    */
    enum DrawType
    {
        TYPE_NODE, //!< 节点类型.
        TYPE_PRIM, //!< 原始类型.
        TYPE_SPRT //!< 精灵类型.
    };

    /*!
        深度测试的类型.
    */
    enum DepthTest
    {
        DEPTH_TEST_ALWAYS, //!< 总是通过.
        DEPTH_TEST_LESS, //!< 如果传入的深度值小于存储值则通过.
        DEPTH_TEST_GREATER, //!< 如果传入的深度值大于存储值则通过.
        DEPTH_TEST_LEQUAL, //!< 如果传入的深度值小于或等于存储值则通过.
        DEPTH_TEST_GEQUAL //!< 如果传入的深度值大于或等于存储值则通过.
    };

    /*!
        混合模式的类型.
    */
    enum BlendMode
    {
        BLEND_OFF, //!< (Rs, Gs, Bs, As)
        BLEND_HALF, //!< (Rs, Gs, Bs, As) * As + (Rd, Gd, Bd, Ad) * (1 - As)
        BLEND_ADD, //!< (Rs, Gs, Bs, As) + (Rd, Gd, Bd, Ad)
        BLEND_DEST_ALPHA //!< (Rs, Gs, Bs, As) * Ad + (Rd, Gd, Bd, Ad) * (1 - Ad)
    };

    /*!
        指定绘图属性的标志位.
    */
    enum DrawFlag
    {
        FLAG_BOUND_CLIP, //!< 启用范围修剪.
        FLAG_SORT, //!< 按照观察点的距离对绘图对象排序.
        FLAG_WRITE_RGB, //!< 把 RGB 成员写入帧缓存.
        FLAG_WRITE_ALPHA, //!< 把透明成员写入帧缓存.
        FLAG_WRITE_DEPTH, //!< 把深度值写入深度缓存.
        FLAG_BACKFACE_CULLING, //!< 启用剔除背面.
        FLAG_BILINEAR //!< 启用双线性滤波.
    };

    /*!
        析构 fsDraw 绘图类.
    */
    virtual ~fsDraw();


    /*!
        判定 fsDraw 是否直接属于某个屏幕.
        @return fsDraw 是否直接属于某个屏幕.
    */
    bool hasScreen() const;


    /*!
        返回 fsDraw 是直接属于的屏幕 ID.
        如果 fsDraw 不属于任何屏幕则抛出异常.
        @return fsDraw 是直接属于的屏幕 ID.
    */
    fsID getScreenID() const;


    /*!
        让这个 fsDraw 属于指定的屏幕.
        @param[in] scr_id 屏幕的 ID.
    */
    void setScreenID(fsID scr_id);


    /*!
        返回这个 fsDraw 是否有父绘图对象 fsDraw.
        @return 这个 fsDraw 是否有父绘图对象 fsDraw.
    */
    bool hasParent() const;


    /*!
        返回这个 fsDraw 的父绘图对象 fsDraw.
        如果这个 fsDraw 没有父绘图对象 fsDraw 则返回 NULL.
        @return 这个 fsDraw 的父绘图对象 fsDraw.
    */
    fsDraw* getParentN() const;


    /*!
        设置这个 fsDraw 的父绘图对象 fsDraw.
        这个 fsDraw 属于指定的 fsDraw 的最后一个孩子节点.
        @param[in] parent 一个 fsDraw 父绘图对象.
    */
    void setParent(fsDraw* parent);


    /*!
        返回这个 fsDraw 的前一个绘图对象 fsDraw.
        如果这个 fsDraw 没有前一个图对象 fsDraw 则返回 NULL.
        该方法遵循整棵树的链表结构.
        @return 这个 fsDraw 的前一个绘图对象 fsDraw.
    */
    fsDraw* getPrevAllN() const;


    /*!
        返回这个 fsDraw 的下一个绘图对象 fsDraw.
        如果这个 fsDraw 没有下一个图对象 fsDraw 则返回 NULL.
        该方法遵循整棵树的链表结构.
        @return 这个 fsDraw 的下一个绘图对象 fsDraw.
    */
    fsDraw* getNextAllN() const;


    /*!
        返回这个 fsDraw 的前一个兄弟绘图对象 fsDraw.
        如果这个 fsDraw 没有前一个兄弟图对象 fsDraw 则返回 NULL.
        @return 这个 fsDraw 的前一个兄弟绘图对象 fsDraw.
    */
    fsDraw* getPrevSiblingN() const;


    /*!
        返回这个 fsDraw 的下一个兄弟绘图对象 fsDraw.
        如果这个 fsDraw 没有下一个兄弟图对象 fsDraw 则返回 NULL.
        @return 这个 fsDraw 的下一个兄弟绘图对象 fsDraw.
    */
    fsDraw* getNextSiblingN() const;


    /*!
        返回这个 fsDraw 的最后子孙绘图对象 fsDraw.
        如果这个 fsDraw 没有最后子孙图对象 fsDraw 则返回 NULL.
        @return 这个 fsDraw 的最后子孙绘图对象 fsDraw.
    */
    fsDraw* getLastDescendant() const;


    /*!
        判断这个 fsDraw 是否有子绘图对象 fsDraw.
        @return 这个 fsDraw 是否有子绘图对象 fsDraw.
    */
    bool hasChild() const;


    /*!
        返回这个 fsDraw 的第一个子绘图对象 fsDraw.
        如果这个 fsDraw 没有第一个子图对象 fsDraw 则返回 NULL.
        @return 这个 fsDraw 的第一个子绘图对象 fsDraw.
    */
    fsDraw* getFirstChildN() const;


    /*!
        返回这个 fsDraw 的最后一个子绘图对象 fsDraw.
        如果这个 fsDraw 没有最后一个子图对象 fsDraw 则返回 NULL.
        @return 这个 fsDraw 的最后一个子绘图对象 fsDraw.
    */
    fsDraw* getLastChildN() const;


    /*!
        改变这个 fsDraw 在兄弟绘图对象中的顺序为第一个.
        这个 fsDraw 必须有个屏幕或者是一个父绘图对象的孩子.
    */
    void moveFirst();


    /*!
        改变这个 fsDraw 在兄弟绘图对象中的顺序为最后一个.
        这个 fsDraw 必须有个屏幕或者是一个父绘图对象的孩子.
    */
    void moveLast();


    /*!
        改变这个 fsDraw 的顺序为指定的 fsDraw 绘图对象的前一个.
        这个 fsDraw 必须有个屏幕或者是一个父绘图对象的孩子.
        @param[in] draw 指定的 fsDraw.
    */
    void moveBefore(fsDraw* draw);


    /*!
        改变这个 fsDraw 的顺序为指定的 fsDraw 绘图对象的下一个.
        这个 fsDraw 必须有个屏幕或者是一个父绘图对象的孩子.
        @param[in] draw 指定的 fsDraw.
    */
    void moveAfter(fsDraw* draw);


    /*!
        返回这个 fsDraw 绘图对象的类型.
        @return 这个 fsDraw 绘图对象的类型.
    */
    DrawType getType() const;


    /*!
        返回这个 fsDraw 是否可见.
        @return 这个 fsDraw 是否可见.
    */
    bool isVisible() const;


    /*!
        设置这个 fsDraw 是否可见.
        @param[in] is_visible 这个 fsDraw 是否可见.
    */
    void setVisible(bool is_visible);


    /*!
        返回这个 fsDraw 滤波器的颜色.
        @return 这个 fsDraw 滤波器的颜色.
    */
    fsCol getColor() const;

    /*!
        设置这个 fsDraw 滤波器的颜色.
        @param[in] col 滤波器的颜色.
    */
    void setColor(fsCol col);


    /*!
        返回深度测试的类型.
        @return 深度测试的类型.
    */
    DepthTest getDepthTest() const;


    /*!
        设置深度测试的类型.
        @param[in] depth_test 深度测试的类型.
    */
    void setDepthTest(DepthTest depth_test);


    /*!
        返回混合模式的类型.
        @return 混合模式的类型.
    */
    BlendMode getBlendMode() const;


    /*!
        设置混合模式的类型.
        @param[in] blend_mode 混合模式的类型.
        @param[in] is_preset_setting 如果设置为 true, 则自动设置绘图标志的深度模式类型.
    */
    void setBlendMode(BlendMode blend_mode, bool is_preset_setting);


    /*!
        返回指定的绘图属性标志位是否开启.
        @param[in] draw_flag 绘图属性标志位.
        @return 指定的绘图属性标志位是否开启.
    */
    bool isDrawFlag(DrawFlag draw_flag) const;


    /*!
        设置指定的绘图属性标志位是否开启.
        @param[in] draw_flag 绘图属性标志位.
        @param[in] is_on 指定的绘图属性标志位是否开启.
    */
    void setDrawFlag(DrawFlag draw_flag, bool is_on);


    /*!
        设置所有的绘图属性标志位关闭.
    */
    void clearDrawFlag();


    /*!
        从指定的 fsDraw 复制绘图属性标志位.
        @param[in] src 指定的 fsDraw 绘图对象.
    */
    void copyDrawFlag(const fsDraw* src);


    /*!
        返回这个 fsDraw 的最小范围修剪值.
        @return 这个 fsDraw 的最小范围修剪值.
    */
    const fsVec& getClipBoundMin() const;


    /*!
        返回这个 fsDraw 的最大范围修剪值.
        @return 这个 fsDraw 的最大范围修剪值.
    */
    const fsVec& getClipBoundMax() const;


    /*!
        设置这个 fsDraw 的范围修剪值.
        @param[in] bound_min 最小的范围修剪值.
        @param[in] bound_max 最大的范围修剪值.
    */
    void setClipBound(const fsVec& bound_min, const fsVec& bound_max);


    /*!
        返回 fsDraw 序列的中间位置.
        @return fsDraw 序列的中间位置.
    */
    const fsVec& getSortCenter() const;


    /*!
        设置 fsDraw 序列的中间位置.
        @param[in] sort_center fsDraw 序列的中间位置.
    */
    void setSortCenter(const fsVec& sort_center);


    /*!
        返回序列的偏移量.
        @return 序列的偏移量.
    */
    r32 getSortOffset() const;


    /*!
        设置序列的偏移量.
        @param sort_offset 序列的偏移量.
    */
    void setSortOffset(r32 sort_offset);


    /*!
        返回这个 fsDraw 使用的纹理 ID.
        如果不存在则返回 fsID::ZERO.
        @return 这个 fsDraw 使用的纹理 ID.
    */
    fsID getTextureID() const;


    /*!
        设置这个 fsDraw 使用的指定的纹理 ID.
        如果设置 fsID::ZERO 未指定值则不指定纹理.
        @param tex_id 指定的纹理 ID.
    */
    void setTextureID(fsID tex_id);


    /*!
        返回当前屏幕的参照矩阵.
        @return 当前的参照矩阵.
    */
    fsMat& local();


    /*!
        计算并返回目前的世界参照矩阵.
        @return 目前的世界参照矩阵.
    */
    fsMat calcWorld() const;


    /*!
        计算并返回目前的最终绘图颜色.
        @return 目前的最终绘图颜色.
    */
    fsCol calcFinalColor() const;


protected:
    //! @cond
    enum PrivateFlag
    {
        FLAG_INITIALIZED, //
        FLAG_VISIBLE
    };

    fsDraw();

    void setupWorld();
    void setupFinalColor();
    void setupSortValue(const fsMat& view);
    void setupDrawState();

    virtual void render(const fsMat& view);

    fsTree<fsDraw> m_tree;
    fsType<u8, DrawType> m_type;
    fsType<u8, DepthTest> m_depth_test;
    fsType<u8, BlendMode> m_blend_mode;
    fsFlag<u8, PrivateFlag> m_private_flag;
    fsFlag<u16, DrawFlag> m_draw_flag;
    fsID m_scr_id;
    fsTex* m_tex;
    fsMat m_local;
    fsMat m_world;
    fsCol m_local_col;
    fsCol m_final_col;
    fsVec m_bound_min;
    fsVec m_bound_max;
    fsVec m_sort_center;
    r32 m_sort_offset;
    r32 m_sort_value;
    fsDraw* m_next_sort;
    //! @endcond

private:
    fsDraw(const fsDraw&);
    void operator=(const fsDraw&);
};


#endif /* FS_DRAW_H_ */
