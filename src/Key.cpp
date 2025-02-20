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

#include <algorithm>

#include <openWin/Key.h>
#include <openWin/ErrorStream.h>
#include <openWin/Tools.h>

#include "Built-in/_MacrosForErrorHandling.h"
#include "Built-in/_Windows.h"

using namespace win;

Shortcut& Shortcut::setRepeatable(bool __enable) noexcept
{
    if (__enable)
        tools::assign_as(modifiers, modifiers & ~MOD_NOREPEAT);
    else
        tools::assign_as(modifiers, modifiers | MOD_NOREPEAT);

    return *this;
}

bool Shortcut::isRepeatable() const noexcept
{
    return (modifiers & MOD_NOREPEAT) != MOD_NOREPEAT;
}

bool Shortcut::contains(Modifiers __modifiers) const noexcept
{
    return (modifiers & __modifiers & 0b1111) == (__modifiers & 0b1111);
}

bool Shortcut::contains(Key __key) const noexcept
{
    return key == __key;
}

int Shortcut::getId() const noexcept
{
    return static_cast<int>(modifiers & 0b1111 | (key << 4));
}

Shortcut Shortcut::fromId(int __id) noexcept
{
    return Shortcut(
        static_cast<Modifiers>(__id & 0b1111 | MOD_NOREPEAT),
        static_cast<Key>((__id >> 4) & 0xff));
}

std::string keys::get_name(Modifiers __modifiers) noexcept
{
    std::string res;

    if (__modifiers & WIN & 0b1111)
    {
        res += "Win";
    }

    if (__modifiers & Ctrl & 0b1111)
    {
        if (not res.empty())
            res += '+';

        res += "Ctrl";
    }

    if (__modifiers & Shift & 0b1111)
    {
        if (not res.empty())
            res += '+';

        res += "Shift";
    }

    if (__modifiers & Alt & 0b1111)
    {
        if (not res.empty())
            res += '+';

        res += "Alt";
    }

    return res;
}

std::string keys::get_name(Key __key) noexcept
{
    _Win_Static_Begin_

    std::string res(20, '\0');

    int size = GetKeyNameTextA(
        (MapVirtualKeyA(__key, MAPVK_VK_TO_VSC) << 16) | (is_extended_key(__key) ? 0x01000000 : 0),
        const_cast<char*>(res.data()),
        static_cast<int>(res.size()));

    res.resize(size);
    return res;
}

std::string Shortcut::name() const noexcept
{
    std::string modname = keys::get_name(modifiers);
    std::string keyname = keys::get_name(key);

    if (not modname.empty() && not keyname.empty())
        return modname + '+' + keyname;
    else if (not modname.empty())
        return modname;
    else
        return keyname;
}

bool keys::is_extended_key(Key __key) noexcept
{
    static constexpr Key extended_key_list[] = {
        Key_Cancel,
        Key_Clear,
        Key_Pause,
        Key_PageUp,
        Key_PageDown,
        Key_End,
        Key_Home,
        Key_LeftArrow,
        Key_UpArrow,
        Key_RightArrow,
        Key_DownArrow,
        Key_PrintScreen,
        Key_Insert,
        Key_Delete,
        Key_LWin,
        Key_RWin,
        Key_Apps,
        Key_Multiply,
        Key_Add,
        Key_Subtract,
        Key_Divide,
        Key_NumLock,
        Key_RCtrl,
        Key_RAlt,
        Key_Browser_Back,
        Key_Browser_Forward,
        Key_Browser_Refresh,
        Key_Browser_Stop,
        Key_Browser_Search,
        Key_Browser_Favorites,
        Key_Browser_Home,
        Key_Volume_Mute,
        Key_Volume_Down,
        Key_Volume_Up,
        Key_Media_NextTrack,
        Key_Media_PrevTrack,
        Key_Media_Stop,
        Key_Media_PlayOrPause,
        Key_Launch_Mail,
        Key_Launch_MediaSelect,
        Key_Launch_App1,
        Key_Launch_App2
    };

#if true
    static_assert(
        std::is_sorted(std::begin(extended_key_list), std::end(extended_key_list)),
        "Ensure that the array is sorted before calling binary_search.");
#endif

    return std::binary_search(std::begin(extended_key_list), std::end(extended_key_list), __key);
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
    _Win_Static_Begin_

    MSG msg;

    int ret;

    while ((ret = GetMessage(&msg, nullptr, 0, 0)) != 0)
    {
        if (ret == -1)
        {
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
    _Win_Static_Begin_

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
    _Win_Static_Begin_

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
    _Win_Static_Begin_

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
    _Win_Static_Begin_

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
    _Win_Static_Begin_

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