/*
 * fs_script_entry.h
 *
 *  Created on: 2011-5-16
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */

#ifndef FS_SCRIPT_ENRTY_H_
#define FS_SCRIPT_ENRTY_H_


class fsScript;


/*!
    @ingroup fsScript
    An entry of a configuration script.
*/
class FS_API fsScriptEntry
{
    friend class fsScript;

public:
    /*!
        The maximum length of the name.
    */
    static const u32 MAX_NAME_LENGTH = 20;

    /*!
        The value types of an entry.
    */
    enum ValueType
    {
        TYPE_S32, //!< 32-bit singed number.
        TYPE_R32, //!< 32-bit floating point number.
        TYPE_STRING //!< 8-bit string.
    };

    /*!
        Returns the previous entry. If the previous entry doesn't exit, returns NULL.
        @return The previous entry.
    */
    fsScriptEntry* getPrevN() const;

    /*!
        Returns the next entry. If the next entry doesn't exit, returns NULL.
        @return The next entry.
    */
    fsScriptEntry* getNextN() const;

    /*!
        Returns the name of this entry.
        @return The name of this entry.
    */
    const fsStr<char, MAX_NAME_LENGTH>& getName() const;

    /*!
        Returns the number of the values.
        @return The number of the values.
    */
    u16 getValueNum() const;

    /*!
        Returns the type of the specified value.
        @param[in] index The index of a value.
        @return The type of the specified value.
    */
    ValueType getValueType(u16 index) const;

    /*!
        Returns the specified value as 32-bit integer number.
        @param[in] index The index of a value.
        @return The specified value.
    */
    s32 getValue_s32(u16 index) const;

    /*!
        Returns the specified value as 32-bit floating point number.
        @param[in] index The index of a value.
        @return The specified value.
    */
    r32 getValue_r32(u16 index) const;

    /*!
        Returns the specified value as string.
        @param[in] index The index of a value.
        @return The specified value.
    */
    const char* getValue_string(u16 index) const;

private:
    union Value
    {
        s32 val_s32;
        r32 val_r32;
        const char* val_str;
    };

    fsScriptEntry(const char* name, u16 val_num, fsType<u8, ValueType>* val_type, Value* val);
    void operator=(const fsScriptEntry&);

    fsList<fsScriptEntry>::Item m_item;
    fsStr<char, MAX_NAME_LENGTH> m_name;
    fsID m_name_id;
    u16 m_val_num;
    fsType<u8, ValueType>* m_val_type;
    Value* m_val;
};

#endif /* FS_SCRIPT_ENRTY_H_ */
