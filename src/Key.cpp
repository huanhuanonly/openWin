/**
* Copyright (c) 2024-2025 Yang Huanhuan (3347484963@qq.com).
* 
* This software is provided "as is", without warranty of any kind, express or implied.
*/

/**
* Key.cpp In the openWin (https://github.com/huanhuanonly/openWin)
* 
* Created by Yang Huanhuan on January 13, 2025, 22:35:31
* 
* --- This file is a part of openWin ---
* 
* @brief Implement Key.h
*/

#include <openWin/Key.h>
#include <openWin/ErrorStream.h>
#include <openWin/Tools.h>

#include "Built-in/_Windows.h"

using namespace win;

void Shortcut::setRepeatable(bool __enable) noexcept
{
    if (__enable)
        tools::assign_as(modifiers, modifiers & ~MOD_NOREPEAT);
    else
        tools::assign_as(modifiers, modifiers | MOD_NOREPEAT);
}

int Shortcut::getId() const noexcept
{
    return static_cast<int>(modifiers | (key << 4));
}

Shortcut Shortcut::fromId(int __id) noexcept
{
    return Shortcut(
        static_cast<Modifiers>(__id & 0x0F),
        static_cast<Key>((__id >> 4) & 0xFF));
}

GlobalShortcutManager::GlobalShortcutManager()
    : _M_impl(new GlobalShortcutManager::Impl(&GlobalShortcutManager::_M_manager, this))
{ }

GlobalShortcutManager* GlobalShortcutManager::global() noexcept
{
    static GlobalShortcutManager manager;
    return &manager;
}

bool GlobalShortcutManager::isRegistered(Shortcut __shortcut) noexcept
{
    if (RegisterHotKey(nullptr, 0, __shortcut.modifiers, __shortcut.key))
    {
        UnregisterHotKey(nullptr, 0);
        return false;
    }
    else
    {
        return true;
    }
}

void GlobalShortcutManager::bindShortcutToFunction(
    Shortcut __shortcut,
    GlobalShortcutManager::ShortcutFunction __function,
    GlobalShortcutManager::ShortcutFunctionParam __param,
    bool __enable) noexcept
{
    if (__enable)
    {
        _M_impl->_M_applyRegister(
            __shortcut,
            __function,
            __param);
    }
    else
    {
        _M_impl->_M_applyUnregister(__shortcut);
    }
}

GlobalShortcutManager::ShortcutFunction GlobalShortcutManager::functionFromBoundShortcut(
    Shortcut __shortcut,
    GlobalShortcutManager::ShortcutFunctionParam* __param) noexcept
{
    auto [func, param] = _M_impl->_M_find(__shortcut);

    if (__param != nullptr)
    {
        *__param = param;
    }

    return func;
}


void GlobalShortcutManager::_M_manager() noexcept
{
    /// @note ErrorStream::global() is defined as thread_local.
    ErrorStreamGuard guard(*ErrorStream::global(), __func__);

    MSG msg;

    int ret;

    while ((ret = GetMessage(&msg, NULL, 0, 0)) != 0)
    {
        if (ret == -1)
        {
            ErrorStream::global()->check();
            ErrorStream::global()->setFail();
            break;
        }

        switch (msg.message)
        {
            case Impl::WmApplied:
            {
                _M_impl->_M_tryTodo();
                break;
            }

            case WM_HOTKEY:
            {
                auto [func, param] = _M_impl->_M_find(
                    Shortcut::fromId(static_cast<int>(msg.wParam)));

                (*func)(param);

                break;
            }
        }
    }
}


GlobalShortcutManager::Impl::~Impl() noexcept
{
    for (auto it = _M_shortcutBindings.begin(); it != _M_shortcutBindings.end(); ++it)
    {
        _M_unregister(it->first);
    }
}

void GlobalShortcutManager::Impl::_M_applyRegister(
    Shortcut __shortcut,
    GlobalShortcutManager::ShortcutFunction __function,
    GlobalShortcutManager::ShortcutFunctionParam __param) noexcept
{
    _M_mutex.lock();
    _M_taskQueue.push_front(std::make_tuple(true, __shortcut, __function, __param));
    _M_mutex.unlock();

    PostThreadMessage(
        tools::force_cast<DWORD>(_M_thread.get_id()),
        WmApplied,
        0,
        0);
}

void GlobalShortcutManager::Impl::_M_applyUnregister(
    Shortcut __shortcut) noexcept
{
    _M_mutex.lock();
    _M_taskQueue.push_front(std::make_tuple(false, __shortcut, nullptr, nullptr));
    _M_mutex.unlock();

    PostThreadMessage(
        tools::force_cast<DWORD>(_M_thread.get_id()),
        WmApplied,
        0,
        0);
}

void GlobalShortcutManager::Impl::_M_register(
    Shortcut __shortcut,
    GlobalShortcutManager::ShortcutFunction __function,
    GlobalShortcutManager::ShortcutFunctionParam __param) noexcept
{
    ErrorStreamGuard guard(*ErrorStream::global(), __func__);

    if (RegisterHotKey(
        nullptr,
        __shortcut.getId(),
        __shortcut.modifiers,
        __shortcut.key))
    {
        _M_shortcutBindings[__shortcut] = std::make_pair(__function, __param);
    }
}

void GlobalShortcutManager::Impl::_M_unregister(
    Shortcut __shortcut) noexcept
{
    ErrorStreamGuard guard(*ErrorStream::global(), __func__);

    if (UnregisterHotKey(nullptr, __shortcut.getId()))
    {
        if (auto it = _M_shortcutBindings.find(__shortcut); it != _M_shortcutBindings.end())
        {
            _M_shortcutBindings.erase(it);
        }
    }
}

void GlobalShortcutManager::Impl::_M_tryTodo() noexcept
{
    _M_mutex.lock();

    while (_M_taskQueue.empty())
    {
        auto& [flag, sc, sf, sfp] = _M_taskQueue.front();

        if (flag)
            _M_register(sc, sf, sfp);
        else
            _M_unregister(sc);

        _M_taskQueue.pop_front();
    }

    _M_mutex.unlock();
}

std::pair<GlobalShortcutManager::ShortcutFunction, GlobalShortcutManager::ShortcutFunctionParam>
    GlobalShortcutManager::Impl::_M_find(Shortcut __shortcut) const noexcept
{
    std::lock_guard<std::mutex> lg(_M_mutex);

    auto fit = _M_shortcutBindings.find(__shortcut);

    if (fit == _M_shortcutBindings.end())
    {
        return { nullptr, nullptr };
    }
    
    return fit->second;
}