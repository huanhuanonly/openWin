/**
* Copyright (c) 2024-2025 Yang Huanhuan (3347484963@qq.com).
* 
* This software is provided "as is", without warranty of any kind, express or implied.
*/

/**
* Key.h In the openWin (https://github.com/huanhuanonly/openWin)
* 
* Created by Yang Huanhuan on January 13, 2025, 22:30:39
* 
* --- This file is a part of openWin ---
* 
* @brief Define key-related types and provide global shortcut registration methods and managers.
*/

#pragma once

#ifndef OPENWIN_HEADER_KEY_H
#define OPENWIN_HEADER_KEY_H

#include <cstdint>

#include <map>
#include <forward_list>
#include <tuple>
#include <string>

#include <memory>

#include <thread>
#include <mutex>

namespace win
{

/// @see also https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
enum Key : std::uint32_t
{
    Key_LeftButton = 0x01,
    Key_RightButton,
    Key_Cancel,
    Key_MiddleButton,
    Key_SideButton1,
    Key_SideButton2,

    Key_BackSpace = 0x08,
    Key_Tab,
    Key_Clear = 0x0C,
    Key_Return,
    Key_Shift = 0x10,
    Key_Ctrl,
    Key_Alt,
    Key_Pause,
    Key_CapsLock,
    Key_Escape = 0x1B,
    Key_Space = 0x20,
    Key_PageUp,
    Key_PageDown,
    Key_End,
    Key_Home,
    Key_LeftArrow,
    Key_UpArrow,
    Key_RightArrow,
    Key_DownArrow,
    Key_Select,
    Key_Print,
    Key_Execute,
    Key_PrintScreen,
    Key_Insert,
    Key_Delete,
    Key_Help,
    Key_0,
    Key_1,
    Key_2,
    Key_3,
    Key_4,
    Key_5,
    Key_6,
    Key_7,
    Key_8,
    Key_9,
    Key_A = 0x41,
    Key_B,
    Key_C,
    Key_D,
    Key_E,
    Key_F,
    Key_G,
    Key_H,
    Key_I,
    Key_J,
    Key_K,
    Key_L,
    Key_M,
    Key_N,
    Key_O,
    Key_P,
    Key_Q,
    Key_R,
    Key_S,
    Key_T,
    Key_U,
    Key_V,
    Key_W,
    Key_X,
    Key_Y,
    Key_Z,
    Key_LWin,
    Key_RWin,
    Key_Apps,
    Key_Sleep = 0x5F,
    Key_Numpad_0,
    Key_Numpad_1,
    Key_Numpad_2,
    Key_Numpad_3,
    Key_Numpad_4,
    Key_Numpad_5,
    Key_Numpad_6,
    Key_Numpad_7,
    Key_Numpad_8,
    Key_Numpad_9,
    Key_Multiply,
    Key_Add,
    Key_Separator,
    Key_Subtract,
    Key_Decimal,
    Key_Divide,
    Key_F1,
    Key_F2,
    Key_F3,
    Key_F4,
    Key_F5,
    Key_F6,
    Key_F7,
    Key_F8,
    Key_F9,
    Key_F10,
    Key_F11,
    Key_F12,
    Key_F13,
    Key_F14,
    Key_F15,
    Key_F16,
    Key_F17,
    Key_F18,
    Key_F19,
    Key_F20,
    Key_F21,
    Key_F22,
    Key_F23,
    Key_F24,
    Key_NumLock = 0x90,
    Key_ScrollLock,
    Key_LShift = 0xA0,
    Key_RShift,
    Key_LCtrl,
    Key_RCtrl,
    Key_LAlt,
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
    Key_Launch_App2,
    Key_Oem_1 = 0xBA,  // ';:' for US
    Key_Oem_Plus,      // '+' any country
    Key_Oem_Comma,     // ',' any country
    Key_Oem_Minus,     // '-' any country
    Key_Oem_Period,    // '.' any country
    Key_Oem_2,         // '/?' for US
    Key_Oem_3,         // '`~' for US
    Key_Oem_4 = 0xDB,  //  '[{' for US
    Key_Oem_5,         //  '\|' for US
    Key_Oem_6,         //  ']}' for US
    Key_Oem_7,         //  ''"' for US
    Key_Oem_8,
    Key_Oem_Ax = 0xE1, //  'AX' key on Japanese AX kbd
    Key_Oem_102,       //  "<>" or "\|" on RT 102-key kbd.
    Key_Attn = 0xF6,
    Key_CrSel,
    Key_ExSel,
    Key_EraseEOF,
    Key_Play,
    Key_Zoom,
    Key_Pa1 = 0xFD,
    Key_Oem_Clear
};

enum Modifiers : std::uint32_t
{
    Alt   = 0x01 | 0x4000,
    Ctrl  = 0x02 | 0x4000,
    Shift = 0x04 | 0x4000,
    WIN   = 0x08 | 0x4000
};

namespace keys
{

[[nodiscard]] std::string get_name(Modifiers __modifiers) noexcept;
[[nodiscard]] std::string get_name(Key __key) noexcept;

[[nodiscard]] bool is_extended_key(Key __key) noexcept;

}  // namespace key

/**
* Use `Ctrl + Alt + Key_A` or similar to create a Shortcut.
*/
struct Shortcut
{
    Modifiers modifiers;
    Key key;

    Shortcut(Modifiers __modifiers, Key __key) noexcept
        : modifiers(__modifiers), key(__key) 
    { }

    /**
     * @brief Changes the hotkey behavior so that the keyboard auto-repeat does
     *        not yield multiple hotkey notifications.
     * 
     * @return Self.
     */
    Shortcut& setRepeatable(bool __enable = true) noexcept;
    [[nodiscard]] bool isRepeatable() const noexcept;

    [[nodiscard]] bool contains(Modifiers __modifiers) const noexcept;
    [[nodiscard]] bool contains(Key __key) const noexcept;

    [[nodiscard]] int getId() const noexcept;
    [[nodiscard]] static Shortcut fromId(int __id) noexcept;

    [[nodiscard]] bool operator==(Shortcut __other) const noexcept
    { return getId() == __other.getId(); }
    [[nodiscard]] bool operator!=(Shortcut __other) const noexcept
    { return getId() != __other.getId(); }
    [[nodiscard]] bool operator>(Shortcut __other) const noexcept
    { return getId() > __other.getId(); }
    [[nodiscard]] bool operator>=(Shortcut __other) const noexcept
    { return getId() >= __other.getId(); }
    [[nodiscard]] bool operator<(Shortcut __other) const noexcept
    { return getId() < __other.getId(); }
    [[nodiscard]] bool operator<=(Shortcut __other) const noexcept
    { return getId() <= __other.getId(); }

    [[nodiscard]] std::string name() const noexcept;
};


inline Modifiers operator+(Modifiers __first, Modifiers __second) noexcept
{ return static_cast<Modifiers>(__first | __second); }

inline Shortcut operator+(Modifiers __modifiers, Key __key) noexcept
{ return Shortcut(__modifiers, __key); }


namespace keys
{

[[nodiscard]] inline std::string get_name(Shortcut __shortcut) noexcept
{ return __shortcut.name(); }

}


class GlobalShortcutManager
{
private:

    GlobalShortcutManager();

    GlobalShortcutManager(const GlobalShortcutManager&) = delete;
    GlobalShortcutManager(GlobalShortcutManager&&) = delete;

    GlobalShortcutManager& operator=(const GlobalShortcutManager&) = delete;
    GlobalShortcutManager& operator=(GlobalShortcutManager&&) = delete;

public:

    using ShortcutFunctionParam = void*;
    using ShortcutFunction = void (*)(ShortcutFunctionParam);

    static GlobalShortcutManager* global() noexcept;

    [[nodiscard]]
    static bool isRegistered(Shortcut __shortcut) noexcept;

    /**
    * @brief Registers a global shortcut When the shortcut key is pressed, the
    *        bound function is called.
    * 
    * @param __enable If false, unregisters and unbinds the global shortcut.
    */
    void bindShortcutToFunction(
        Shortcut __shortcut,
        ShortcutFunction __function,
        ShortcutFunctionParam __param,
        bool __enable = true) noexcept;

    /**
    * @return nullptr if the __shortcut is not bound.
    */
    [[nodiscard]]
    ShortcutFunction functionFromBoundShortcut(
        Shortcut __shortcut,
        ShortcutFunctionParam* __param = nullptr) noexcept;


private:

    void _M_manager() noexcept;

    class Impl;

    std::unique_ptr<Impl> _M_impl;
};


class GlobalShortcutManager::Impl
{
public:
    
    template<typename _Fn, typename... _Args>
    explicit Impl(_Fn&& __fn, _Args&&... __args) noexcept
        : _M_thread(std::forward<_Fn>(__fn), std::forward<_Args>(__args)...)
    { }

    ~Impl() noexcept;

    enum : std::uint32_t { WmApplied = 0x0400U };

    void _M_applyRegister(
        Shortcut __shortcut,
        ShortcutFunction __function,
        ShortcutFunctionParam __param) noexcept;

    void _M_applyUnregister(
        Shortcut __shortcut) noexcept;

    void _M_register(
        Shortcut __shortcut,
        ShortcutFunction __function,
        ShortcutFunctionParam __param) noexcept;

    void _M_unregister(
        Shortcut __shortcut) noexcept;

    void _M_tryTodo() noexcept;

    std::pair<ShortcutFunction, ShortcutFunctionParam>
        _M_find(Shortcut __shortcut) const noexcept;

private:

    std::map<
        Shortcut,
        std::pair<
            ShortcutFunction,
            ShortcutFunctionParam>> _M_shortcutBindings;

    std::thread _M_thread;

    mutable std::mutex _M_mutex;

    std::forward_list<
        std::tuple<
            bool,
            Shortcut,
            ShortcutFunction,
            ShortcutFunctionParam>> _M_taskQueue;
};

}  // namespace win

#endif  // OPENWIN_HEADER_KEY_H