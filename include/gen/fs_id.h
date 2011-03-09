/*
 * fs_id.h
 *
 *  Created on: 2011-3-9
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */

#ifndef FS_ID_H_
#define FS_ID_H_


/*!
    @ingroup fsGen
    一个唯一的值,用来保存对象的ID号.
*/
class FS_API fsID
{
public:
    //! @cond
    fsDefineException(ExceptionInvalidArgument);
    fsDefineException(ExceptionOutOfID);
    fsDefineException(ExceptionTooLongString);
    //! @endcond


    static const fsID ZERO; //!< The initial id.


    /*!
        Constructs and initializes a fsID to fsID::ZERO.
    */
    fsID();


    /*!
        Returns whether this fsID is equal to the right hand side fsID.
        @param[in] id The right hand side fsID.
        @return Whether this fsID is equal to the right hand side fsID.
    */
    bool operator==(fsID id) const;


    /*!
        Returns whether this fsID is unequal to the right hand side fsID.
        @param[in] id The right hand side fsID.
        @return Whether this fsID is unequal to the right hand side fsID.
    */
    bool operator!=(fsID id) const;


    /*!
        Returns the remainder of the value of this fsID devided by the specified value.
        This method is used when fsID is used as the key type of ckMap.
        @param[in] n A value to devide the value of fsID.
        @return The remainder of the value of this fsID devided by the specified value.
    */
    u32 operator%(u32 n) const;


    /*!
        Returns the value of this fsID.
        @return The value of this fsID.
    */
    u32 getValue() const;

    /*!
        Generates a unique fsID.
        @return A unique fsID.
    */
    static fsID genID();


    /*!
        Generates a fsID whose value is generated from the specified string.
        There may be rare occations when the same value is generated from different strings.
        @param[in] str The seed of a unique value.
        @return A fsID.
    */
    static fsID genID(const char* str);


    /*!
        Generates a fsID from the specified value. This method is only for system.
        @param[in] value The value of the fsID to be generated.
        @return A fsID.
    */
    static fsID genIDForEngine(u32 value);


    /*!
        Sets the value, which is used to generate the next fsID, to the specified value. This method is only for system.
        @param[in] value A value which is used to generate the next fsID.
    */
    static void setCurIDForEngine(u32 value);


    /*!
        Throws an exception to notify too-long-string is specified.
    */
    static s32 ThrowTooLongStringExceptionForEngine();


private:
    static const u32 BIT_NUM = 32;
    static const u32 MAX_ID = (static_cast<u64>(1) << (BIT_NUM - 1)) - 1;

    u32 m_id;

    static u32 m_cur_id;
};


/*!
    Generates a fsID whose value is generated from the specified string literal.
    There may be rare occations when the same value is generated from different strings.
    @param[in] str The seed of a unique value.
    @return A fsID.
*/
#define fsID_(str) fsID::genIDForEngine(FS_ID_00(str, sizeof("" str)))


#define FS_ID_00(str, len) ((len <= 1) ? 0 : (FS_ID_01(str, len - 1) | 0x80000000))
#define FS_ID_01(str, len) ((len <= 1) ? str[0] : FS_ID_02(str, len - 1) * 37 + str[len - 1])
#define FS_ID_02(str, len) ((len <= 1) ? str[0] : FS_ID_03(str, len - 1) * 37 + str[len - 1])
#define FS_ID_03(str, len) ((len <= 1) ? str[0] : FS_ID_04(str, len - 1) * 37 + str[len - 1])
#define FS_ID_04(str, len) ((len <= 1) ? str[0] : FS_ID_05(str, len - 1) * 37 + str[len - 1])
#define FS_ID_05(str, len) ((len <= 1) ? str[0] : FS_ID_06(str, len - 1) * 37 + str[len - 1])
#define FS_ID_06(str, len) ((len <= 1) ? str[0] : FS_ID_07(str, len - 1) * 37 + str[len - 1])
#define FS_ID_07(str, len) ((len <= 1) ? str[0] : FS_ID_08(str, len - 1) * 37 + str[len - 1])
#define FS_ID_08(str, len) ((len <= 1) ? str[0] : FS_ID_09(str, len - 1) * 37 + str[len - 1])
#define FS_ID_09(str, len) ((len <= 1) ? str[0] : FS_ID_10(str, len - 1) * 37 + str[len - 1])
#define FS_ID_10(str, len) ((len <= 1) ? str[0] : FS_ID_11(str, len - 1) * 37 + str[len - 1])
#define FS_ID_11(str, len) ((len <= 1) ? str[0] : FS_ID_12(str, len - 1) * 37 + str[len - 1])
#define FS_ID_12(str, len) ((len <= 1) ? str[0] : FS_ID_13(str, len - 1) * 37 + str[len - 1])
#define FS_ID_13(str, len) ((len <= 1) ? str[0] : FS_ID_14(str, len - 1) * 37 + str[len - 1])
#define FS_ID_14(str, len) ((len <= 1) ? str[0] : FS_ID_15(str, len - 1) * 37 + str[len - 1])
#define FS_ID_15(str, len) ((len <= 1) ? str[0] : FS_ID_16(str, len - 1) * 37 + str[len - 1])
#define FS_ID_16(str, len) ((len <= 1) ? str[0] : FS_ID_17(str, len - 1) * 37 + str[len - 1])
#define FS_ID_17(str, len) ((len <= 1) ? str[0] : FS_ID_18(str, len - 1) * 37 + str[len - 1])
#define FS_ID_18(str, len) ((len <= 1) ? str[0] : FS_ID_19(str, len - 1) * 37 + str[len - 1])
#define FS_ID_19(str, len) ((len <= 1) ? str[0] : FS_ID_20(str, len - 1) * 37 + str[len - 1])
#define FS_ID_20(str, len) ((len <= 1) ? str[0] : FS_ID_21(str, len - 1) * 37 + str[len - 1])
#define FS_ID_21(str, len) ((len <= 1) ? str[0] : FS_ID_22(str, len - 1) * 37 + str[len - 1])
#define FS_ID_22(str, len) ((len <= 1) ? str[0] : FS_ID_23(str, len - 1) * 37 + str[len - 1])
#define FS_ID_23(str, len) ((len <= 1) ? str[0] : FS_ID_24(str, len - 1) * 37 + str[len - 1])
#define FS_ID_24(str, len) ((len <= 1) ? str[0] : FS_ID_25(str, len - 1) * 37 + str[len - 1])
#define FS_ID_25(str, len) ((len <= 1) ? str[0] : FS_ID_26(str, len - 1) * 37 + str[len - 1])
#define FS_ID_26(str, len) ((len <= 1) ? str[0] : FS_ID_27(str, len - 1) * 37 + str[len - 1])
#define FS_ID_27(str, len) ((len <= 1) ? str[0] : FS_ID_28(str, len - 1) * 37 + str[len - 1])
#define FS_ID_28(str, len) ((len <= 1) ? str[0] : FS_ID_29(str, len - 1) * 37 + str[len - 1])
#define FS_ID_29(str, len) ((len <= 1) ? str[0] : FS_ID_30(str, len - 1) * 37 + str[len - 1])
#define FS_ID_30(str, len) ((len <= 1) ? str[0] : FS_ID_31(str, len - 1) * 37 + str[len - 1])
#define FS_ID_31(str, len) ((len <= 1) ? str[0] : FS_ID_32(str, len - 1) * 37 + str[len - 1])
#define FS_ID_32(str, len) ((len <= 1) ? str[0] : fsID::ThrowTooLongStringExceptionForEngine())


#endif /* FS_ID_H_ */
