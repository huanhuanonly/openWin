/**
* Copyright (c) 2024-2025 Yang Huanhuan (3347484963@qq.com).
* 
* This software is provided "as is", without warranty of any kind, express or implied.
*/

/**
* ErrorStream.cpp In the openWin (https://github.com/huanhuanonly/openWin)
* 
* Created by Yang Huanhuan on December 10, 2024, 10:37:35
* 
* --- This file is a part of openWin ---
* 
* @brief Implement ErrorStream.h
*/

#include <openWin/ErrorStream.h>

#include "Built-in/_Windows.h"

#include <mutex>

using namespace win;

static std::mutex _S_cerrMutex;

ErrorStream::ErrorStream() noexcept
    : _M_failed(false)
{ }

void ErrorStream::begin(const char* __work)
{
    if (_M_workpath.empty())
    {
        clear();
        SetLastError(ERROR_SUCCESS);
    }

    _M_workpath.append(__work);
}

void ErrorStream::end()
{
    if (_M_failed && _M_queue.empty())
    {
        onFailed("Failed in this work!");
    }

    _M_workpath.removeLast();
}

void ErrorStream::onFailed(std::uint32_t __code)
{
    std::lock_guard<std::mutex> _L_guard(_S_cerrMutex);

    if (not _M_workpath.empty())
    {
        std::cerr << "In ";
        
        for (auto work : _M_workpath)
        {
            std::cerr << work << '/';
        }

        std::cerr << ": ";
    }

    std::cerr << "( " << __code << " ) ";

    const char* text = codeToText(__code);

    if (text)
    {
        std::cerr << text;
    }

    std::cerr.put('\n');
}

void ErrorStream::onFailed(const std::string& __text)
{
    std::lock_guard<std::mutex> _L_guard(_S_cerrMutex);

    if (not _M_workpath.empty())
    {
        std::cerr << "In ";

        for (auto work : _M_workpath)
        {
            std::cerr << work << '/';
        }

        std::cerr << ": ";
    }

    std::cerr << __text << '\n';
}

const char* ErrorStream::codeToText(std::uint32_t __code) const noexcept
{
    char* buffer = nullptr;
    FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr,
        __code,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        reinterpret_cast<LPSTR>(&buffer),
        0,
        nullptr
    );
    return buffer;
}

bool ErrorStream::check() noexcept
{
    std::uint32_t code = GetLastError();

    if (code)
    {
        *this << code;
        return false;
    }

    return true;
}

void ErrorStream::setFail()
{
    _M_failed = true;
}

bool ErrorStream::success() const noexcept
{
    return !_M_failed && _M_queue.empty();
}

bool ErrorStream::failed() const noexcept
{
    return _M_failed || !_M_queue.empty();
}

const char* ErrorStream::last() const noexcept
{
    if (!_M_queue.empty())
    {
        if (_M_queue.front().code)
        {
            return codeToText(_M_queue.front().code);
        }
        else
        {
            return _M_queue.front().text.c_str();
        }
    }

    return nullptr;
}

void ErrorStream::remove() noexcept
{
    if (not _M_queue.empty())
    {
        _M_queue.remove();
    }
}

void ErrorStream::clear()
{
    _M_queue.clear();
    // _M_workpath.clear();
    _M_failed = false;
}

ErrorStream& ErrorStream::operator<<(std::uint32_t __code) noexcept
{
    if (__code == 0)
    {
        return *this;
    }

    _M_queue.insert({ __code, std::string() });

    onFailed(__code);
    return *this;
}

ErrorStream& ErrorStream::operator<<(const std::string& __text) noexcept
{
    if (__text.empty())
    {
        return *this;
    }

    _M_queue.insert({ 0, __text });

    onFailed(__text);
    return *this;
}

ErrorStream* ErrorStream::global() noexcept
{
    thread_local ErrorStream _S_errorStream;
    return &_S_errorStream;
}