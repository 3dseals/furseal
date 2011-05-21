/*
 * fs_col.h
 *
 *  Created on: 2011-3-10
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */

#ifndef FS_COL_H_
#define FS_COL_H_


/*!
    @ingroup fsDraw
    颜色结构类, 由红色, 绿色, 蓝色和透明度组成.
*/
struct FS_API fsCol
{
    static const fsCol ZERO; //!< 所有成员值为 0s 的颜色.
    static const fsCol FULL; //!< 所有成员值为 255s 的颜色.

    u8 r; //!< 红色成员.
    u8 g; //!< 绿色成员.
    u8 b; //!< 蓝色成员.
    u8 a; //!< 透明成员.

    /*!
        构造一个 fsCol 对象.
    */
    fsCol();

    /*!
        从指定的 fsCol 构造一个 fsCol 对象.
        @param[in] col 一个 fsCol 对象.
    */
    fsCol(const fsCol& col);

    /*!
        从指定的成分种构造一个 fsCol 对象.
        @param[in] r_ 红色成员.
        @param[in] g_ 绿色成员.
        @param[in] b_ 蓝色成员.
        @param[in] a_ 透明成员. 默认值为 255 不透明.
    */
    fsCol(u8 r_, u8 g_, u8 b_, u8 a_ = 255);

    /*!
        从指定的成分中设置所有的值.
        @param[in] r_ 红色成员.
        @param[in] g_ 绿色成员.
        @param[in] b_ 蓝色成员.
        @param[in] a_ 透明成员. 默认值为 255 不透明.
    */
    void set(u8 r_, u8 g_, u8 b_, u8 a_ = 255);

    /*!
        重载 == 运算,判断右边的颜色是否相等.
        @param[in] col 右边的颜色 fsCol.
        @return 右边的颜色是否相等.
    */
    bool operator==(fsCol col) const;

    /*!
        重载 != 运算,判断右边的颜色是否不等.
        @param[in] col 右边的颜色 fsCol.
        @return 右边的颜色是否不等.
    */
    bool operator!=(fsCol col) const;

    /*!
        重载 + 运算,返回当前颜色 fsCol 加上操作符右边的颜色 fsCol.
        所有成员的值在 0 到 255 之间.
        @param[in] col 右边的颜色 fsCol.
        @return 这两个颜色 fsCols 的和.
    */
    fsCol operator+(fsCol col) const;

    /*!
        重载 += 运算,使得当前颜色 fsCol 加上右边的颜色 fsCol.
        所有成员的值在 0 到 255 之间.
        @param[in] col 右边的颜色 fsCol.
    */
    void operator+=(fsCol col);

    /*!
        重载-运算,返回当前颜色 fsCol 减去操作符右边的颜色 fsCol.
        所有成员的值在 0 到 255 之间.
        @param[in] col 右边的颜色 fsCol.
        @return 这两个颜色 fsCols 的差.
    */
    fsCol operator-(fsCol col) const;

    /*!
        重载 -= 运算,使得当前颜色 fsCol 减去右边的颜色 fsCol.
        所有成员的值在 0 到 255 之间.
        @param[in] col 右边的颜色 fsCol.
    */
    void operator-=(fsCol col);

    /*!
        重载 * 运算,返回当前颜色 fsCol 乘以操作符右边的颜色 fsCol.
        所有成员的值在 0 到 255 之间.
        @param[in] col 右边的颜色 fsCol.
        @return 这两个颜色 fsCols 的积.
    */
    fsCol operator*(fsCol col) const;

    /*!
        重载 *= 运算,使得当前颜色 fsCol 乘以右边的颜色 fsCol.
        所有成员的值在 0 到 255 之间.
        @param[in] col 右边的颜色 fsCol.
    */
    void operator*=(fsCol col);

    /*!
        重载 * 运算,返回当前颜色 fsCol 分别乘以操作符右边的数值.
        所有成员的值在 0 到 255 之间.
        @param[in] s 右边的实数.
        @return 当前颜色 fsCol 分别乘以操作符右边的数值.
    */
    fsCol operator*(r32 s) const;

    /*!
        重载 * 运算,返回颜色参数 fsCol 分别乘以实数参数的数值.
        所有成员的值在 0 到 255 之间.
        @param[in] s 实数.
        @param[in] col 颜色 fsCol.
        @return 颜色参数 fsCol 分别乘以实数参数的数值.
    */
    FS_API friend fsCol operator*(r32 s, fsCol col);

    /*!
        重载 *= 运算,使得当前颜色 fsCol 分别乘以操作符右边的数值.
        所有成员的值在 0 到 255 之间.
        @param[in] s 右边的实数.
    */
    void operator*=(r32 s);

    /*!
        重载 / 运算,返回当前颜色 fsCol 分别除以操作符右边的数值.
        所有成员的值在 0 到 255 之间.
        @param[in] s 右边的实数.
        @return 当前颜色 fsCol 分别除以操作符右边的数值.
    */
    fsCol operator/(r32 s) const;

    /*!
        重载 /= 运算,使得当前颜色 fsCol 分别除以操作符右边的数值.
        所有成员的值在 0 到 255 之间.
        @param[in] s 右边的实数.
    */
    void operator/=(r32 s);

    /*!
        返回当前颜色 fsCol 到另一个颜色 fsCol 按照比率 ratio 的线性插值.
        @param[in] to 另一个颜色 fsCol.
        @param[in] ratio 两个 fsCols 之间插值的比率. 比率值在 0.0f 到 1.0f 之间.
        @return 这两个 fsCols 的相性插值的颜色 fsCol.
    */
    fsCol interp(fsCol to, r32 ratio) const;
};


#endif /* FS_COL_H_ */
