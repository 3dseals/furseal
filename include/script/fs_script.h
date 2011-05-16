/*
 * fs_script.h
 *
 *  Created on: 2011-5-16
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */

#ifndef FS_SCRIPT_H_
#define FS_SCRIPT_H_


class fsScriptMgr;


/*!
    @ingroup fsScript
    A configuration script.
*/
class FS_API fsScript
{
    friend class fsScriptMgr;

public:
    //! @cond
    fsDefineException(ExceptionCannotOpenFile);
    fsDefineException(ExceptionCannotReadFile);
    fsDefineException(ExceptionInvalidArgument);
    fsDefineException(ExceptionInvalidCall);
    //! @endcond

    /*!
        Returns the previous configuration. If the previous configuration doesn't exist, returns NULL.
        @return The previous configuration.
    */
    fsScript* getPrevN() const;

    /*!
        Returns the next configuration. If the next configuration doesn't exist, returns NULL.
        @return The next configuration.
    */
    fsScript* getNextN() const;

    /*!
        Returns the ID of this configuration.
        @return The ID of this configuration.
    */
    fsID getID() const;

    /*!
        Returns whether this configuration is valid.
        @return Whether this configuration is valid.
    */
    bool isValid() const;

    /*!
        Returns the line number of this configuration.
        @return The line number of this configuration.
    */
    u16 getErrorLineNo() const;

    /*!
        Returns the number of the entries.
        @param[in] ent_name The name of an entry.
        @return The number of the entries.
    */
    u16 getEntryNum(const char* ent_name) const;

    /*!
        Returns the specified entry. The entry is searched from the first. If the entry doesn't exist, returns NULL.
        @param[in] ent_name The name of an entry.     
        @return The specified entry.
    */
    fsScriptEntry* getEntryFromFirstN(const char* ent_name) const;

    /*!
        Returns the specified entry. The entry is searched from the last. If the entry doesn't exist, returns NULL.
        @param[in] ent_name The name of an entry.     
        @return The specified entry.
    */
    fsScriptEntry* getEntryFromLastN(const char* ent_name) const;

    /*!
        Returns the specified entry. The entry is searched from the previous of the some entry. If the entry doesn't exist, returns NULL.
        @param[in] ent_name The name of an entry.     
        @param[in] ent An entry.
        @return The specified entry.
    */
    fsScriptEntry* getEntryBeforeN(const char* ent_name, fsScriptEntry* ent) const;

    /*!
        Returns the specified entry. The entry is searched from the next of the some entry. If the entry doesn't exist, returns NULL.
        @param[in] ent_name The name of an entry.     
        @param[in] ent An entry.
        @return The specified entry.
    */
    fsScriptEntry* getEntryAfterN(const char* ent_name, fsScriptEntry* ent) const;

    /*!
        Returns the first entry. If the first entry doesn't exist, return NULL.
        @return The first entry.
    */
    fsScriptEntry* getFirstEntryN() const;

    /*!
        Returns the last entry. If the last entry doesn't exist, return NULL.
        @return The last entry.
    */
    fsScriptEntry* getLastEntryN() const;

    /*!
        Creates an configuration from the specified file before the system initialization.
        @param[in] filename The name of a file.
        @return An entry.
    */
    static fsScript* newPriorConfigBeforeInitialization(const char* filename);

    /*!
        Deletes the specified configuration created before the system initialization.
        @param[in] conf A configuration.
    */
    static void deletePriorConfig(fsScript* conf);

private:
    fsScript(fsID id, const void* data, u32 data_size);
    fsScript(fsID id, const void* data, u32 data_size, void* dummy);
    ~fsScript();
    void operator=(const fsScript&);

    fsScriptEntry* newEntry(const char* ent_name, u16 val_num);
    void deleteEntry(fsScriptEntry* ent);

    void parse(const void* data, u32 data_size);

    fsID m_id;
    u16 m_err_line_no;
    fsList<fsScriptEntry> m_ent_list;
    void* (*m_malloc)(u32 size);
    void (*m_free)(void* ptr);
};

#endif /* FS_SCRIPT_H_ */
