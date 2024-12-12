/**
* ErrorStream.h In the openWin (https://github.com/huanhuanonly/openWin)
*
* Created by Yang Huanhuan on December 10, 2024, 10:37:33
*
* Email -> 3347484963@qq.com
*
* --- This file is a part of openWin ---
*
* @brief Encapsulates an error stream to handle errors in
*        layered work, which will output error information
*        to std::cerr.
*/

#pragma once

#ifndef ERROR_STREAM_H
#define ERROR_STREAM_H

#include <iostream>

#include <forward_list>
#include <vector>

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
    * @returns the text corresponding to the error code, or
    *          returns null if there is no corresponding text.
    */
    virtual const char* codeToText(std::uint32_t __code) const noexcept;

    /**
    * @return false if an error is found.
    */
    bool check() noexcept;

    void setFail();

    bool success() const noexcept;
    bool failed() const noexcept;

    /**
    * @returns the last error text if there is one, otherwise
    *          return nullptr.
    * 
    * @warning the return value may be freed later, or it may be
    *          static.
    */
    const char* last() const noexcept;

    /**
    * @brief Remove last error.
    */
    void remove() noexcept;

    void clear();

    ErrorStream& operator<<(std::uint32_t __code) noexcept;
    ErrorStream& operator<<(const std::string& __text) noexcept;

    static ErrorStream* global() noexcept;

private:

    std::forward_list<Item> _M_queue;

    std::vector<const char*> _M_workpath;
    std::uint32_t _M_failed;
};

class ErrorStreamGuard final
{
public:

    explicit ErrorStreamGuard(
        ErrorStream& __ref,
        const char* __work)
        : _M_ref(__ref)
    {
        _M_ref.begin(__work);
    }

    ~ErrorStreamGuard()
    {
        _M_ref.check();
        _M_ref.end();
    }

    ErrorStreamGuard(const ErrorStreamGuard&) = delete;
    ErrorStreamGuard& operator=(const ErrorStreamGuard&) = delete;

private:

    ErrorStream& _M_ref;
};

#endif  // ERROR_STREAM_H
