/*
 * fs_script_parse.cpp
 *
 *  Created on: 2011-5-16
 *      Author: Lional King
 *
 *  Copyright (c) 2011 netsurfers
 */


#include "fs_script_all.h"

#include "fs_gen_all.h"
#include "fs_base_all.h"


struct Value
{
    Value* next;
    fsScriptEntry::ValueType type;

    union
    {
        s32 val_s32;
        r32 val_r32;
        char* val_str;
    };
};


class Parser
{
public:
    Parser(const void* src, u32 src_size)
    {
        m_cursor = static_cast<const char*>(src);
        m_remain_size = src_size;
        m_line_no = 1;
        m_is_err = false;

        skipSpace();
    }

    bool isEnd() const
    {
        return (m_remain_size == 0 || m_is_err);
    }

    bool isError() const
    {
        return m_is_err;
    }

    u16 getErrorLineNo()
    {
        return isError() ? m_line_no : 0;
    }

    void parseChar(char c)
    {
        if (isError())
        {
            return;
        }

        skipSpace();

        if (getNext() == c)
        {
            moveForward();
        }
        else
        {
            m_is_err = true;
        }
    }

    bool tryToParseChar(char c)
    {
        if (isError())
        {
            return false;
        }

        skipSpace();

        if (getNext() == c)
        {
            moveForward();

            return true;
        }
        else
        {
            return false;
        }
    }

    void parseIdentifier(fsStr<char, fsScriptEntry::MAX_NAME_LENGTH>* name)
    {
        if (isError())
        {
            return;
        }

        skipSpace();

        *name = "";

        while (!isEnd())
        {
            char c = getNext();

            if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_')
            {
                if (name->getLength() >= fsScriptEntry::MAX_NAME_LENGTH)
                {
                    m_is_err = true;
                }

                *name += c;

                moveForward();
            }
            else
            {
                return;
            }
        }
    }

    void parseNumber(Value* val)
    {
        val->type = fsScriptEntry::TYPE_S32;
        val->val_s32 = 0;

        if (isError())
        {
            return;
        }

        skipSpace();

        bool is_minus = false;
        char c = getNext();

        if (c == '-')
        {
            is_minus = true;

            moveForward();

            c = getNext();
        }

        if (c >= '0' && c <= '9')
        {
            val->val_s32 = c - '0';

            moveForward();

            for (c = getNext(); c >= '0' && c <= '9'; c = getNext())
            {
                val->val_s32 *= 10;
                val->val_s32 += c - '0';

                moveForward();
            }

            if (c == '.')
            {
                val->type = fsScriptEntry::TYPE_R32;
                val->val_r32 = static_cast<r32>(val->val_s32);

                moveForward();

                r32 decimal = 0.1f;

                for (c = getNext(); c >= '0' && c <= '9'; c = getNext())
                {
                    val->val_r32 += (c - '0') * decimal;
                    decimal *= 0.1f;

                    moveForward();
                }

                if (is_minus)
                {
                    val->val_r32 = -val->val_r32;
                }
            }
            else
            {
                if (is_minus)
                {
                    val->val_s32 = -val->val_s32;
                }
            }
        }
    }

    u16 parseString1()
    {
        if (isError())
        {
            return 0;
        }

        u16 len = 0;
        const char* cursor = m_cursor;

        for (u16 remain_size = m_remain_size; remain_size > 0; remain_size--)
        {
            if (*cursor == '"')
            {
                return len;
            }
            else if (*cursor == '\n' || *cursor == '\r')
            {
                break;
            }
            else if (*cursor == '\\')
            {
                if (remain_size == 0)
                {
                    break;
                }

                cursor++;
                remain_size--;
            }

            len++;
            cursor++;
        }

        m_is_err = true;
        return 0;
    }

    void parseString2(Value* val, char* str_buf, u16 str_buf_size)
    {
        val->type = fsScriptEntry::TYPE_STRING;
        val->val_str = str_buf;

        if (isError())
        {
            return;
        }

        while (!isEnd())
        {
            char c = getNext();
            moveForward();

            if (c == '"')
            {
                if (str_buf_size == 0)
                {
                    break;
                }

                *str_buf = '\0';

                return;
            }
            else if (c == '\n' || c == '\r')
            {
                break;
            }
            else if (c == '\\')
            {
                if (isEnd())
                {
                    break;
                }

                c = getNext();
                moveForward();

                if (c == 'n')
                {
                    c = '\n';
                }
                else if (c == 't')
                {
                    c = '\t';
                }
            }

            if (str_buf_size > 0)
            {
                *str_buf = c;

                str_buf++;
                str_buf_size--;
            }
            else
            {
                break;
            }
        }

        m_is_err = true;
    }

    void checkEnd()
    {
        skipSpace();
    }

private:
    char getNext() const
    {
        if (isError())
        {
            return '\0';
        }

        return (m_remain_size > 0) ? *m_cursor : '\0';
    }

    void skipSpace()
    {
        if (isError())
        {
            return;
        }

        bool is_end_of_line = false;
        char end_of_line_char = '\0';

        while (!isEnd())
        {
            char c = getNext();

            if (c == '#')
            {
                moveForward();

                while (!isEnd())
                {
                    c = getNext();

                    if (c == '\n' || c == '\r')
                    {
                        break;
                    }

                    moveForward();
                }

                continue;
            }

            if (c == '\n' || c == '\r')
            {
                if (!is_end_of_line)
                {
                    is_end_of_line = true;
                    end_of_line_char = c;

                    m_line_no++;
                }
                else if (end_of_line_char != c)
                {
                    is_end_of_line = false;
                }

                moveForward();
            }
            else
            {
                is_end_of_line = false;

                if (c == ' ' || c == '\t')
                {
                    moveForward();
                }
                else
                {
                    return;
                }
            }
        }
    }

    void moveForward()
    {
        if (isError())
        {
            return;
        }

        if (m_remain_size > 0)
        {
            m_cursor++;
            m_remain_size--;
        }
    }

    const char* m_cursor;
    u32 m_remain_size;
    u16 m_line_no;
    bool m_is_err;
};


void fsScript::parse(const void* data, u32 data_size)
{
    Parser pars(data, data_size);

    while (!pars.isEnd())
    {
        fsStr<char, fsScriptEntry::MAX_NAME_LENGTH> ent_name;
        pars.parseIdentifier(&ent_name);

        pars.parseChar('=');

        if (pars.tryToParseChar('('))
        {
            u16 val_num = 0;
            Value* rev_val_list = NULL;

            while (!pars.isEnd())
            {
                Value* val = static_cast<Value*>(m_malloc(sizeof(Value)));

                if (pars.tryToParseChar('"'))
                {
                    u16 str_buf_size = pars.parseString1() + 1;
                    char* str_buf = static_cast<char*>(m_malloc(str_buf_size));

                    pars.parseString2(val, str_buf, str_buf_size);
                }
                else
                {
                    pars.parseNumber(val);
                }

                val->next = rev_val_list;
                rev_val_list = val;

                val_num++;

                if (pars.tryToParseChar(')'))
                {
                    break;
                }

                pars.parseChar(',');
            }

            Value* val_list = NULL;
            Value* val_next;

            for (Value* val = rev_val_list; val; val = val_next)
            {
                val_next = val->next;

                val->next = val_list;
                val_list = val;
            }

            fsScriptEntry* ent = newEntry(ent_name.getString(), val_num);
            u16 val_index = 0;

            for (Value* val = val_list; val; val = val->next)
            {
                ent->m_val_type[val_index] = val->type;

                switch (val->type)
                {
                case fsScriptEntry::TYPE_S32:
                    ent->m_val[val_index].val_s32 = val->val_s32;
                    break;

                case fsScriptEntry::TYPE_R32:
                    ent->m_val[val_index].val_r32 = val->val_r32;
                    break;

                case fsScriptEntry::TYPE_STRING:
                    ent->m_val[val_index].val_str = val->val_str;
                    break;
                }

                val_index++;
            }

            for (Value* val = val_list; val; val = val_next)
            {
                val_next = val->next;

                m_free(val);
            }
        }
        else
        {
            Value val;

            if (pars.tryToParseChar('"'))
            {
                u16 str_buf_size = pars.parseString1() + 1;
                char* str_buf = static_cast<char*>(m_malloc(str_buf_size));

                pars.parseString2(&val, str_buf, str_buf_size);
            }
            else
            {
                pars.parseNumber(&val);
            }

            fsScriptEntry* ent = newEntry(ent_name.getString(), 1);

            ent->m_val_type[0] = val.type;

            switch (val.type)
            {
            case fsScriptEntry::TYPE_S32:
                ent->m_val[0].val_s32 = val.val_s32;
                break;

            case fsScriptEntry::TYPE_R32:
                ent->m_val[0].val_r32 = val.val_r32;
                break;

            case fsScriptEntry::TYPE_STRING:
                ent->m_val[0].val_str = val.val_str;
                break;
            }
        }

        pars.checkEnd();
    }

    if (pars.isError() && isValid())
    {
        m_err_line_no = pars.getErrorLineNo();

        fsLowLevelAPI::printf("*** config file compile error ***\n");
    }
}
