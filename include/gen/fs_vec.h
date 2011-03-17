/*
 * fs_vec.h
 *
 *  Created on: 2011-3-10
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */

#ifndef FS_VEC_H_
#define FS_VEC_H_


struct fsMat;


/*!
    @ingroup fsGen
    一个三元向量,用来表示x轴y轴z轴坐标.
*/
struct FS_API fsVec
{
    static const fsVec ZERO; //!< 零向量.
    static const fsVec X_UNIT; //!< x轴的单位向量.
    static const fsVec Y_UNIT; //!< y轴的单位向量.
    static const fsVec Z_UNIT; //!< z轴的单位向量.

    r32 x; //!< x轴坐标.
    r32 y; //!< y轴坐标.
    r32 z; //!< z轴坐标.

    /*!
        构建fsVec.
    */
    fsVec();

    /*!
        构建fsVec并根据x轴y轴z轴坐标初始化.
        @param[in] x_ x轴坐标.
        @param[in] y_ y轴坐标.
        @param[in] z_ z轴坐标. 默认值0.0f.
    */
    fsVec(r32 x_, r32 y_, r32 z_ = 0.0f);

    /*!
        设置fsVecx轴y轴z轴坐标.
        @param[in] x_ x轴坐标.
        @param[in] y_ y轴坐标.
        @param[in] z_ z轴坐标. 默认值0.0f.
    */
    void set(r32 x_, r32 y_, r32 z_ = 0.0f);

    /*!
        重载+运算符,返回左右两边的和.
        @param[in] vec 右边的fsVec.
        @return 左右两边fsVec的和.
    */
    fsVec operator+(const fsVec& vec) const;

    /*!
        重载+=运算符,让左值加上右值.
        @param[in] vec 右边的fsVec.
    */
    void operator+=(const fsVec& vec);

    /*!
        重载-运算符,返回负值.
        @return 负fsVec值.
    */
    fsVec operator-() const;

    /*!
        重载-运算符,返回左右两边的差.
        @param[in] vec 右边的fsVec.
        @return 左右两边fsVec的差.
    */
    fsVec operator-(const fsVec& vec) const;

    /*!
        重载-=运算符,让左值减去右值.
        @param[in] vec 右边的fsVec.
    */
    void operator-=(const fsVec& vec);

    /*!
        重载*运算符,返回左边x轴y轴z轴分别乘上右边实数的积的fsVec.
        @param[in] s 右边的实数.
        @return 左右两边积的fsVec.
    */
    fsVec operator*(r32 s) const;

    /*!
        重载*运算符,返回左边实数分别乘上右边fsVec的x轴y轴z轴积的fsVec.
        @param[in] s 左边的实数.
        @param[in] vec 右边的fsVec.
        @return 左右两边积的fsVec.
    */
    FS_API friend fsVec operator*(r32 s, const fsVec& vec);

    /*!
         重载*=运算符,让左值x轴y轴z轴分别乘以右值.
        @param[in] s 右边的实数.
    */
    void operator*=(r32 s);

    /*!
        重载/运算符,返回左边x轴y轴z轴分别除以右边实数的商的fsVec.
        @param[in] s 左边的实数.
        @return 左右两边商的fsVec.
    */
    fsVec operator/(r32 s) const;

    /*!
        重载/=运算符,让左值x轴y轴z轴分别除以右值.
        @param[in] s 右边的实数.
    */
    void operator/=(r32 s);

    /*!
        返回fsVec的标量长度.
        @return 标量长度.
    */
    r32 length() const;

    /*!
        返回fsVec的标量长度的平方.
        @return 标量长度的平方.
    */
    r32 sqLength() const;

    /*!
        返回两个fsVec的距离.
        @param[in] vec 另一个fsVec.
        @return 两个fsVec的距离.
    */
    r32 dist(const fsVec& vec) const;

    /*!
        返回两个fsVec的距离的平方.
        @param[in] vec 另一个fsVec.
        @return 两个fsVec的距离的平方.
    */
    r32 sqDist(const fsVec& vec) const;

    /*!
        返回两个fsVec每个轴的积的fsVec.
        @param[in] vec 另一个fsVec.
        @return 两个fsVec每个轴的积的fsVec.
    */
    r32 dot(const fsVec& vec) const;

    /*!
        返回两个fsVec的矢量积.
        @param[in] vec 另一个fsVec.
        @return 两个fsVec的矢量积.
    */
    fsVec cross(const fsVec& vec) const;

    /*!
        返回fsVec绕x轴旋转的值.
        @param[in] deg 实数类型的旋转角度.
        @return fsVec绕x轴旋转的值.
    */
    fsVec rotateX_r32(r32 deg) const;

    /*!
        返回fsVec绕y轴旋转的值.
        @param[in] deg 实数类型的旋转角度.
        @return fsVec绕y轴旋转的值.
    */
    fsVec rotateY_r32(r32 deg) const;

    /*!
        返回fsVec绕z轴旋转的值.
        @param[in] deg 实数类型的旋转角度.
        @return fsVec绕z轴旋转的值.
    */
    fsVec rotateZ_r32(r32 deg) const;

    /*!
        返回fsVec绕x轴旋转的值.
        @param[in] deg 整数类型的旋转角度.
        @return fsVec绕x轴旋转的值.
    */
    fsVec rotateX_s32(s32 deg) const;

    /*!
        返回fsVec绕y轴旋转的值.
        @param[in] deg 整数类型的旋转角度.
        @return fsVec绕y轴旋转的值.
    */
    fsVec rotateY_s32(s32 deg) const;

    /*!
        返回fsVec绕z轴旋转的值.
        @param[in] deg 整数类型的旋转角度.
        @return fsVec绕z轴旋转的值.
    */
    fsVec rotateZ_s32(s32 deg) const;

    /*!
        返回fsVec的法向量.
        @return fsVec的法向量.
    */
    fsVec normalize() const;

    /*!
        返回fsVec到另一个fsVec的线性差值.
        @param[in] to 另一个fsVec.
        @param[in] ratio 线性差值在0.0f到1.0f之间.
        @return 到另一个fsVec的线性差值.
    */
    fsVec interp(const fsVec& to, r32 ratio) const;

    /*!
        返回fsVec到另一个fsVec的球形差值.
        @param[in] to 另一个fsVec.
        @param[in] ratio 球形差值在0.0f到1.0f之间.
        @return 到另一个fsVec的球形差值.
    */
    fsVec slerp(const fsVec& to, r32 ratio) const;

    /*!
        返回局部坐标的长度向量.
        @param[in] mat 局部坐标的位置矩阵.
        @return 局部坐标的长度向量.
    */
    fsVec toLocalOf(const fsMat& mat) const;

    /*!
        返回世界坐标的长度向量.
        @param[in] mat 局部坐标的位置矩阵.
        @return 世界坐标的长度向量.
    */
    fsVec toGlobalFrom(const fsMat& mat) const;

    /*!
        返回局部坐标的长度向量,忽略移动变化.
        @param[in] mat 局部坐标的位置矩阵.
        @return 局部坐标的长度向量.
    */
    fsVec toLocalOf_noTrans(const fsMat& mat) const;

    /*!
        返回世界坐标的长度向量,忽略移动变化.
        @param[in] mat 局部坐标的位置矩阵.
        @return 世界坐标的长度向量.
    */
    fsVec toGlobalFrom_noTrans(const fsMat& mat) const;

    /*!
        转化为四元矩阵.
        @param[out] r32x4 目的四元矩阵.
        @param[in] w 第四元素.
    */
    void toR32x4(r32* r32x4, r32 w) const;

    /*!
        从四元矩阵返回向量fsVec.
        @param[in] r32x4 源四元矩阵.
        @return 从四元矩阵返回的向量fsVec.
    */
    static fsVec fromR32x4(const r32* r32x4);

    /*!
        十六元数组乘以四元矩阵.
        @param[out] res_r32x4 目的四元矩阵.
        @param[in] lhs_r32x16 左边十六元数组.
        @param[in] rhs_r32x4 右边的四元矩阵.
    */
    static void mulR32x4(r32* res_r32x4, const r32* lhs_r32x16, const r32* rhs_r32x4);
};


#endif /* FS_VEC_H_ */
