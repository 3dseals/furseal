/*
 * fs_msg.h
 *
 *  Created on: 2011-5-26
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */

#ifndef FS_MSG_H_
#define FS_MSG_H_


/*!
    @ingroup fsGen
    A general purpose message with fixed number parameters.
    @tparam N The number of parameters. This value must be more than zero.
*/
template<u8 N> class fsMsg
{
public:
    //! @cond
    fsDefineException(ExceptionInvalidArgument);
    fsDefineException(ExceptionInvalidCall);
    fsDefineException(ExceptionInvalidType);
    //! @endcond

    /*!
        Constructs a fsMsg.
    */
    fsMsg()
    {
        for (s32 i = 0; i < N; i++)
        {
            m_param_size[i] = 0;
        }
    }

    /*!
        Returns the parameter of the specified index.
        @tparam T The type of the parameter.
        @param[in] index The index of a parameter.
        @return The parameter of the specified index.
    */
    template<class T> T getParam(u8 index)
    {
        if (index >= N)
        {
            fsThrow(ExceptionInvalidArgument);
        }

        if (m_param_size[index] == 0)
        {
            fsThrow(ExceptionInvalidCall);
        }

        if (sizeof(T) != m_param_size[index])
        {
            fsThrow(ExceptionInvalidType);
        }

        return *reinterpret_cast<T*>(&m_param[index]);
    }

    /*!
        Sets the parameter to the specified value.
        @tparam T The type of the parameter.
        @param[in] index The index of a parameter.
        @param[in] param The value of the parameter.
    */
    template<class T> void setParam(u8 index, T param)
    {
        if (index >= N)
        {
            fsThrow(ExceptionInvalidArgument);
        }

        if (sizeof(T) > sizeof(MsgParam))
        {
            fsThrow(ExceptionInvalidType);
        }

        *reinterpret_cast<T*>(&m_param[index]) = param;
        m_param_size[index] = sizeof(T);
    }

private:
    struct MsgParam
    {
        u8 data[8];
    };

    MsgParam m_param[N];
    u8 m_param_size[N];
};


//! @cond
template<> class fsMsg<0>
{
private:
    fsMsg() {}
};
//! @endcond

#endif /* FS_MSG_H_ */
