/**
* Copyright (c) 2024-2025 Yang Huanhuan (3347484963@qq.com).
* 
* This software is provided "as is", without warranty of any kind, express or implied.
*/

/**
* ErrorStream.h In the openWin (https://github.com/huanhuanonly/openWin)
* 
* Created by Yang Huanhuan on December 10, 2024, 10:37:33
* 
* --- This file is a part of openWin ---
* 
* @brief Encapsulates an error stream to handle errors in layered work, which will output error
*        information to std::cerr.
*/

#pragma once

#ifndef OPENWIN_HEADER_ERRORSTREAM_H
#define OPENWIN_HEADER_ERRORSTREAM_H

#include <iostream>

#include <forward_list>
#include <vector>

#include "ds/ForwardList.h"
#include "ds/LinkedList.h"

namespace win
{

class ErrorStream
{
public:

    struct Item
    {
        std::uint32_t code;
        std::string   text;
    };

    ErrorStream() noexcept;

    void begin(const char* __work);
    void end();

    virtual void onFailed(std::uint32_t __code);
    virtual void onFailed(const std::string& __text);

    /**
    * @return The text corresponding to the error code, or returns null if
    *         there is no corresponding text.
    */
    [[nodiscard]] virtual const char* codeToText(std::uint32_t __code) const noexcept;

    /**
    * @brief  Check if there are any new errors.
    * 
    * @return false if an new error is found.
    */
    bool check() noexcept;

    void setFail();

    [[nodiscard]] bool success() const noexcept;
    [[nodiscard]] bool failed() const noexcept;

    /**
    * @return  The last error text if there is one, otherwise return nullptr.
    * 
    * @warning The return value may be freed later, or it may be static.
    */
    [[nodiscard]] const char* last() const noexcept;

    /**
    * @brief Removes the last error.
    */
    void remove() noexcept;

    /**
     * @brief Clears all errors.
     */
    void clear();

    /**
     * @brief Adds an new error.
     */
    ErrorStream& operator<<(std::uint32_t __code) noexcept;
    ErrorStream& operator<<(const std::string& __text) noexcept;

    [[nodiscard]] static ErrorStream* global() noexcept;

private:

    ds::ForwardList<Item> _M_queue;
    ds::LinkedList<const char*> _M_workpath;

    std::uint32_t _M_failed;
};

class ErrorStreamGuard
{
public:

    ErrorStreamGuard(
        ErrorStream& __ref,
        const char* __work,
        bool __checkAtEnd = true)
        : _M_ref(__ref)
        , _M_checkAtEnd(__checkAtEnd)
    {
        _M_ref.begin(__work);
    }

    ~ErrorStreamGuard()
    {
        if (_M_checkAtEnd)
        {
            _M_ref.check();
        }

        _M_ref.end();
    }

    /*
     * Copying and moving are not allowed.
     */

    ErrorStreamGuard(const ErrorStreamGuard&) = delete;
    ErrorStreamGuard& operator=(const ErrorStreamGuard&) = delete;

    ErrorStreamGuard(ErrorStreamGuard&&) = delete;
    ErrorStreamGuard& operator=(ErrorStreamGuard&&) = delete;

    inline void skipCheck() noexcept
    { _M_checkAtEnd = false; }

private:

    ErrorStream& _M_ref;
    bool _M_checkAtEnd;
};

}  // namespace win

#endif  // OPENWIN_HEADER_ERRORSTREAM_H
