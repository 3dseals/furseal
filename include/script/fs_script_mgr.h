/*
 * fs_script_mgr.h
 *
 *  Created on: 2011-5-16
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */

#ifndef FS_SCRIPT_MGR_H_
#define FS_SCRIPT_MGR_H_


/*!
    @ingroup fsScript
    The configuration manager singleton.
*/
class FS_API fsScriptMgr
{
    friend class fsScript;

public:
    //! @cond
    fsDefineException(ExceptionInvalidArgument);
    fsDefineException(ExceptionNotFound);
    fsDefineException(ExceptionNotInitialized);
    fsDefineException(ExceptionSameIDExists);
    //! @endcond

    /*!
        Returns whether fsScriptMgr singleton is created.
        @return Whether fsScriptMgr singleton is created.
    */
    static bool isCreated();

    /*!
        Creates the fsScriptMgr singleton.
    */
    static void createAfterRes();

    /*!
        Destroys the fsScriptMgr singleton.
    */
    static void destroyBeforeRes();

    /*!
        Returns whether the specified configuration exists.
        @param[in] id The ID of a configuration.
        @return Whether the specified configuration exists.
    */
    static bool hasConfig(fsID id);

    /*!
        Returns the specified configuration.
        @param[in] id The ID of a configuration.
        @return The specified configuration.
    */
    static fsScript* getConfig(fsID id);

    /*!
        Creates a configuration.
        @param[in] id The ID of a configuration.
        @param[in] data The data of configuration.
        @param[in] data_size The size of data.
        @return A configuration.
    */
    static fsScript* newConfig(fsID id, const void* data, u32 data_size);

    /*!
        Deletes the specified configuration.
        @param[in] id The ID of a configuration.
    */
    static void deleteConfig(fsID id);

    /*!
        Returns the first configuration. If the first configuration doesn't exist, return NULL.
        @return The first configuration.
    */
    static fsScript* getFirstConfigN();

    /*!
        Returns the last configuration. If the last configuration doesn't exist, return NULL.
        @return The last configuration.
    */
    static fsScript* getLastConfigN();

private:
    static const u32 CONFIG_HASH_SIZE = 10;

    fsScriptMgr();
    ~fsScriptMgr();
    void operator=(const fsScriptMgr&);

    static fsScriptMgr* instance();

    static void configInitializer(fsID id, fsStr<char, 3> ext, const void* data, u32 data_size, void** exinfo);
    static void configFinalizer(fsID id, fsStr<char, 3> ext, const void* data, u32 data_size, void* exinfo);

    fsMap<fsID, fsScript*> m_conf_map;

    static fsScriptMgr* m_instance;
};

#endif /* FS_SCRIPT_MGR_H_ */
