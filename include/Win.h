/**
* Win.h In the openWin (https://github.com/huanhuanonly/openWin)
* 
* Created by Yang Huanhuan on November 25, 2024, 11:15:05
* 
* Email -> 3347484963@qq.com
* 
* --- This file is a part of openWin ---
* 
* @brief Encapsulates a window class (Win) to obtain window
*        information or control the window.
*/

#pragma once

#ifndef WIN_H
#define WIN_H

#include <vector>
#include <string>
#include <map>
#include <forward_list>

#include <iostream>

#include <cstdint>

#include <thread>
#include <mutex>

#include "ErrorStream.h"

#include "PathGenerator.h"
#include "pg_Direct.h"

/**
* @brief Use the WIN_FW macro to map a function name
*        with return value is a String to a function
*        name with return value is a WString.
*/
#define WIN_FW(__func) __func##_w

class [[nodiscard]] Win
{
public:

    using Handle = void*;

    using WinList = std::vector<Win>;

    Win() noexcept;

    Win(Handle __handle) noexcept;
    Win& operator=(Handle __handle) noexcept;

    Win(const Win& __win) noexcept;
    Win& operator=(const Win& __win) noexcept;

    Win(Win&& __win) noexcept;
    Win& operator=(Win&& __win) noexcept;

    ~Win() noexcept;

    void setHandle(Handle __handle) noexcept;
    [[nodiscard]] Handle handle() const noexcept;

    void setErrorStream(ErrorStream& __es);
    [[nodiscard]] ErrorStream& errorStream();
    [[nodiscard]] const ErrorStream& errorStream() const;

    [[nodiscard]] bool success() const noexcept;
    [[nodiscard]] bool failed() const noexcept;

    friend inline std::ostream& operator<<(std::ostream& __os, const Win& __win) noexcept
    {
        __os << "Win { handle = " << __win._M_handle;
        __os << ", class = " << __win.className();
        __os << ", title = " << __win.title();
        __os << " }";

        return __os;
    }

    class Point
    {
    public:

        Point() = default;

        Point(int __x, int __y) noexcept;

        bool operator==(const Point& __other) const noexcept;

        void setX(int __x) noexcept;
        void setY(int __y) noexcept;

        [[nodiscard]] int x() const noexcept;
        [[nodiscard]] int y() const noexcept;

        template<typename _Tp>
        Point operator+(_Tp __other) const noexcept
        {
            return Point(
                static_cast<int>(_M_x + __other),
                static_cast<int>(_M_y + __other));
        }

        template<typename _Tp>
        Point operator-(_Tp __other) const noexcept
        {
            return Point(
                static_cast<int>(_M_x - __other),
                static_cast<int>(_M_y - __other));
        }

        template<typename _Tp>
        Point& operator+=(_Tp __other) noexcept
        { return _M_x += __other, _M_y += __other, *this; }

        template<typename _Tp>
        Point& operator-=(_Tp __other) noexcept
        { return _M_x -= __other, _M_y -= __other, *this; }

        friend inline std::ostream& operator<<(std::ostream& __os, const Point& __point)
        { return __os << "Win::Point { x = " << __point.x() << ", y = " << __point.y() << " }"; }

        friend class Win;

    protected:

        [[nodiscard]] Point physics(float __dpi) const noexcept;
        Point& mapto(float __dpi) noexcept;

    protected:

        int _M_x = 0;
        int _M_y = 0;
    };

    class Size
    {
    public:

        Size() = default;

        Size(int __width, int __height) noexcept;

        bool operator==(const Size& __other) const noexcept;

        void setWidth(int __width) noexcept;
        void setHeight(int __height) noexcept;

        int width() const noexcept;
        int height() const noexcept;

        int w() const noexcept;
        int h() const noexcept;

        template<typename _Tp>
        Size operator*(_Tp __other) const noexcept
        {
            return Size(
                static_cast<int>(_M_width * __other),
                static_cast<int>(_M_height * __other));
        }

        template<typename _Tp>
        Size operator/(_Tp __other) const noexcept
        {
            return Size(
                static_cast<int>(_M_width / __other),
                static_cast<int>(_M_height / __other));
        }

        template<typename _Tp>
        Size& operator*=(_Tp __other) noexcept
        { return _M_width *= __other, _M_height *= __other, *this; }

        template<typename _Tp>
        Size& operator/=(_Tp __other) noexcept
        { return _M_width /= __other, _M_height /= __other, *this; }

        friend inline std::ostream& operator<<(std::ostream& __os, const Size& __size)
        { return __os << "Win::Size { width = " << __size.w() << ", height = " << __size.h() << " }"; }

        friend class Win;

    protected:

        [[nodiscard]] Size physics(float __dpi) const noexcept;
        Size& mapto(float __dpi) noexcept;

    protected:

        int _M_width = 0;
        int _M_height = 0;
    };

    class Rect : public Point, public Size
    {
    public:

        Rect() = default;

        Rect(int __x, int __y, int __w, int __h) noexcept;
        Rect(const Point& __point, const Size& __size) noexcept;

        Point& point() noexcept;
        Size& size() noexcept;

        const Point& point() const noexcept;
        const Size& size() const noexcept;

        bool operator==(const Rect& __other) const noexcept;

        friend inline std::ostream& operator<<(std::ostream& __os, const Rect& __rect)
        { return __os << "Win::Rect { " << __rect.point() << ", " << __rect.size() << " }"; }

        friend class Win;

    protected:

        [[nodiscard]] Rect physics(float __dpi) const noexcept;
        Rect& mapto(float __dpi) noexcept;
    };

    using String = std::string;
    using WString = std::wstring;

    using PgPoint = PathGenerator<Point, int, 2>;
    using PgSize = PathGenerator<Size, int, 2>;
    using PgRect = PathGenerator<Rect, int, 4>;

    using PgInt = PathGenerator<int, int, 1>;

    // --- Get a window ---
    
    static Win fromPoint(const Point& __point) noexcept;
    
    static inline Win fromPoint(int __x, int __y) noexcept
    { return fromPoint(Point(__x, __y)); }

    static Win fromTitle(const String& __title) noexcept;
    static Win fromTitle(const WString& __title) noexcept;

    static Win fromClassName(const String& __className) noexcept;
    static Win fromClassName(const WString& __className) noexcept;

    static Win fromTitleAndClassName(const String& __title, const String& __className) noexcept;
    static Win fromTitleAndClassName(const WString& __title, const WString& __className) noexcept;

    static Win fromFocus() noexcept;

    static Win fromForeground() noexcept;

    static Win fromConsole() noexcept;

    static Win fromShell() noexcept;

    static Win fromDesktop() noexcept;

    static Win fromActiveInCurrentThread() noexcept;

    static Win fromCaptureInCurrentThread() noexcept;

    
    // --- Swap and Compare ---

    void swap(Win& __other) & noexcept;
    void swap(Win& __other) && noexcept;
    void swap(Win&& __other) noexcept;

    [[nodiscard]]
    int compare(const Win& __other) const noexcept;

    [[nodiscard]]
    static int compare(const Win& __first, const Win& __second) noexcept;

    [[nodiscard]]
    inline bool operator<(const Win& __other) const noexcept
    { return compare(__other) < 0; }

    [[nodiscard]]
    inline bool operator<=(const Win& __other) const noexcept
    { return compare(__other) <= 0; }

    [[nodiscard]]
    inline bool operator==(const Win& __other) const noexcept
    { return compare(__other) == 0; }

    [[nodiscard]]
    inline bool operator!=(const Win& __other) const noexcept
    { return compare(__other) != 0; }

    [[nodiscard]]
    inline bool operator>(const Win& __other) const noexcept
    { return compare(__other) > 0; }

    [[nodiscard]]
    inline bool operator>=(const Win& __other) const noexcept
    { return compare(__other) >= 0; }


    [[nodiscard]]
    bool isValid() const noexcept;

    [[nodiscard]]
    static bool isValid(Handle __handle) noexcept;

    operator bool() const noexcept;

    [[nodiscard]]
    bool isEmpty() const noexcept;

    [[nodiscard]]
    static bool isEmpty(Handle __handle) noexcept;

    [[nodiscard]]
    bool isNotResponding() const noexcept;

    [[nodiscard]]
    inline bool isHunging() const noexcept
    { return isNotResponding(); }


    void setEnable(bool __enable = true) const noexcept;
    [[nodiscard]] bool isEnabled() const noexcept;

    void setActive() const noexcept;
    [[nodiscard]] bool isActived() const noexcept;

    void setNoActive(bool __enable = true) const noexcept;
    [[nodiscard]] bool isNoActived() const noexcept;

    void setForeground(bool __lock = false) const noexcept;
    [[nodiscard]] bool isForeground() const noexcept;

    static void lockSetForeground() noexcept;
    static void unlockSetForeground() noexcept;

    void setFocus() const noexcept;
    [[nodiscard]] bool hasFocus() const noexcept;

    void setCapture(bool __enable = true) const noexcept;

    enum CharSetCode : bool
    {
        ANSI,
        Unicode
    };

    [[nodiscard]]
    CharSetCode charSetCode() const noexcept;

    [[nodiscard]]
    float dpi() const noexcept;

    
    // --- Parent and Child ---

    void setParent(const Win& __win) const noexcept;
    void setParent(std::nullptr_t) const noexcept;

    /**
    * @param __onlyParent If true, returns owner of the
    *                     current window when the current
    *                     is a child window.
    */
    Win parent(bool __onlyParent = false) const noexcept;
    [[nodiscard]] bool hasParent() const noexcept;

    WinList children() const noexcept;
    [[nodiscard]] bool hasChild() const noexcept;

    /**
    * @brief If the function succeeds, the return value is a
    *        handle to the child window at the top of the Z
    *        order. If the specified window has no child
    *        windows, the return value is empty.
    */
    Win topChild() const noexcept;

    [[nodiscard]] bool isParent() const noexcept;
    [[nodiscard]] bool isChild() const noexcept;

    [[nodiscard]] bool isChildOf(const Win& __parent) const noexcept;

    enum Orientation
    {
        Vertical   = 0x01,
        Horizontal = 0x02
    };

    int setTile(Orientation __orientation) const noexcept;
    int setTile(Orientation __orientation, const Rect& __clientRect) const noexcept;

    /**
    * @returns all top-level windows on the screen
    */
    [[nodiscard]]
    static WinList list() noexcept;

    [[nodiscard]]
    WinList listInSameThread() const noexcept;

    void forward() const noexcept;
    void backward() const noexcept;
    
    void setTopmost(bool __enable = true) const noexcept;
    [[nodiscard]] bool isTopmost() const noexcept;

    Win firstLayer() const noexcept;
    Win lastLayer() const noexcept;
    
    Win nextLayer() const noexcept;
    Win prevLayer() const noexcept;

    Win owner() const noexcept;
    Win root() const noexcept;


    void becomePopup(bool __enable) const noexcept;
    [[nodiscard]] bool isPopup() const noexcept;

    void becomeTool(bool __enable) const noexcept;
    [[nodiscard]] bool isTool() const noexcept;

    [[nodiscard]] bool isMdiChild() const noexcept;

    
    void show() const noexcept;
    void hide() const noexcept;

    [[nodiscard]] bool isVisible() const noexcept;
    
    /**
    * @brief Shows or hides all pop-up windows owned by the
    *        current window.
    */
    void showPopups() const noexcept;
    void hidePopups() const noexcept;

    void maximize() const noexcept;
    void minimize() const noexcept;
    void restore() const noexcept;

    [[nodiscard]] bool isMaximized() const noexcept;
    [[nodiscard]] bool isMinimized() const noexcept;
    [[nodiscard]] bool isRestored() const noexcept;
    [[nodiscard]] bool isArranged() const noexcept;


    void setTitle(const String& __title) const noexcept;
    void setTitle(const WString& __title) const noexcept;

    [[nodiscard]] String title() const noexcept;
    [[nodiscard]] WString WIN_FW(title)() const noexcept;

    [[nodiscard]] String className() const noexcept;
    [[nodiscard]] WString WIN_FW(className)() const noexcept;

    
    void setRect(const Rect& __rect) const noexcept;
    [[nodiscard]] Rect rect() const noexcept;

    [[nodiscard]] Rect clientRect() const noexcept;

    void setPos(
        const Point& __point,
        PgPoint* __pg = pg::Direct<Point, int, 2>::global()) const noexcept;

    inline void setPos(
        int __x, int __y,
        PgPoint* __pg = pg::Direct<Point, int, 2>::global()) const noexcept
    { setPos(Point(__x, __y), __pg); }

    enum PosFlag : int
    {
        TopLeftCorner, TopRightCorner,

        Center,  // Ignore the parameter __reserve

        BottomLeftCorner, BottomRightCorner
    };

    void setPos(
        PosFlag __flag,
        int __reserve = 0,
        PgPoint* __pg = pg::Direct<Point, int, 2>::global()) const noexcept;

    [[nodiscard]] Point pos() const noexcept;

    void move(
        int __addX, int __addY,
        PgPoint* __pg = pg::Direct<Point, int, 2>::global()) const noexcept;

    inline void moveTo(
        const Point& __point,
        PgPoint* __pg = pg::Direct<Point, int, 2>::global()) const noexcept
    { setPos(__point, __pg); }

    inline void moveTo(
        int __x, int __y,
        PgPoint* __pg = pg::Direct<Point, int, 2>::global()) const noexcept
    { setPos(Point(__x, __y), __pg); }

    inline void moveTo(
        PosFlag __flag,
        int __reserve = 0,
        PgPoint* __pg = pg::Direct<Point, int, 2>::global()) const noexcept
    { setPos(__flag, __reserve, __pg); }


    void setSize(
        const Size& __size,
        PgSize* __pg = pg::Direct<Size, int, 2>::global()) const noexcept;

    inline void setSize(
        int __x, int __y,
        PgSize* __pg = pg::Direct<Size, int, 2>::global()) const noexcept
    { setSize(Size(__x, __y), __pg); }

    [[nodiscard]] Size size() const noexcept;

    [[nodiscard]] static Size screenSize() noexcept;

    void setWidth(
        int __width,
        PgInt* __pg = pg::Direct<int, int, 1>::global()) const noexcept;

    void setHeight(
        int __height,
        PgInt* __pg = pg::Direct<int, int, 1>::global()) const noexcept;

    [[nodiscard]] int width() const noexcept;
    [[nodiscard]] int height() const noexcept;

    [[nodiscard]] static int screenWidth() noexcept;
    [[nodiscard]] static int screenHeight() noexcept;

    void setZoom(
        int __addWidth, int __addHeight,
        PgSize* __pg = pg::Direct<Size, int, 2>::global()) const noexcept;

    void setZoom(
        double __scaleX, double __scaleY,
        PgSize* __pg = pg::Direct<Size, int, 2>::global()) const noexcept;

    
    /**
    * @param __value [0, 255] If it is 0, the current
    *                window is completely transparent.
    */
    void setOpacity(
        int __value,
        PgInt* __pg = pg::Direct<int, int, 1>::global()) const noexcept;

    [[nodiscard]] int opacity() const noexcept;

    class Color
    {
    public:

        using Channel = std::uint8_t;

        Color() = default;

        Color(Channel __r, Channel __g, Channel __b) noexcept;

        void setR(Channel __r) noexcept;
        void setG(Channel __g) noexcept;
        void setB(Channel __b) noexcept;

        Channel r() const noexcept;
        Channel g() const noexcept;
        Channel b() const noexcept;

        bool operator==(const Color& __other) const noexcept;

        friend class Win;

    protected:

        std::uint32_t _M_data = 0;
    };

    void setTransparencyColor(const Color& __color) const noexcept;
    [[nodiscard]] Color transparencyColor() const noexcept;

protected:

    void _M_addStyle(std::int32_t __style) const noexcept;
    void _M_delStyle(std::int32_t __style) const noexcept;

    [[nodiscard]]
    bool _M_hasStyle(std::int32_t __style) const noexcept;

    void _M_addExtendStyle(std::int32_t __style) const noexcept;
    void _M_delExtendStyle(std::int32_t __style) const noexcept;

    [[nodiscard]]
    bool _M_hasExtendStyle(std::int32_t __style) const noexcept;

public:

    // --- System window frame ---

    void setBorder(bool __enable = true) const noexcept;
    [[nodiscard]] bool hasBorder() const noexcept;

    void setSunkenEdge(bool __enable = true) const noexcept;
    [[nodiscard]] bool hasSunkenEdge() const noexcept;

    void setRaisedEdge(bool __enable = true) const noexcept;
    [[nodiscard]] bool hasRaisedEdge() const noexcept;

    void setStaticEdge(bool __enable = true) const noexcept;
    [[nodiscard]] bool hasStaticEdge() const noexcept;

    void setTitlebar(bool __enable = true) const noexcept;
    [[nodiscard]] bool hasTitlebar() const noexcept;

    void setMenubar(bool __enable = true) const noexcept;
    [[nodiscard]] bool hasMenubar() const noexcept;

    void setScroll(Orientation __orientation, bool __enable = true) const noexcept;
    [[nodiscard]] bool hasScroll(Orientation __orientation) const noexcept;

    void setSizebox(bool __enable = true) const noexcept;
    [[nodiscard]] bool hasSizebox() const noexcept;

    enum TitlebarButtons : std::uint32_t
    {
        MaximizeButton = 0x01,
        MinimizeButton = 0x02,
        CloseButton    = 0x04,

        HelpButton     = 0x08
    };

    friend inline TitlebarButtons operator|(
        TitlebarButtons __first,
        TitlebarButtons __second) noexcept
    { return static_cast<TitlebarButtons>(__first | __second); }

    void setTitlebarButtons(
        TitlebarButtons __titlebarButtons,
        bool __enable = true) const noexcept;

    [[nodiscard]]
    bool hasTitlebarButtons(
        TitlebarButtons __titlebarButtons) const noexcept;

    // --- Window properties ---
    
    void setTaskbarIcon(bool __enable = true) const noexcept;
    [[nodiscard]] bool hasTaskbarIcon() const noexcept;

    
    void setDoubleBufferDrawing(bool __enable = true) const noexcept;
    [[nodiscard]] bool isDoubleBufferDrawing() const noexcept;

    void setAcceptFiles(bool __enable = true) const noexcept;
    [[nodiscard]] bool isAcceptFiles() const noexcept;

    using Timeout = std::uint32_t;

    enum TimeoutFlag : Timeout
    {
        DefaultTimeout = 5000U,
        InfiniteTimeout = 0xFFFFFFFFU
    };

    enum FlashFlag { UntilIsForeground };

    void flash(bool __enable = true) const noexcept;
    void flash(int __count, Timeout __timeout = 0U, bool __caption = false) const noexcept;
    void flash(FlashFlag, Timeout __timeout = 0U, bool __caption = false) const noexcept;

    void update(bool __eraseBackground = true) const noexcept;
    void update(bool __eraseBackground, const Rect& __clientRect) const noexcept;

    void lockUpdate() const noexcept;
    static void unlockUpdate() noexcept;

    using ThreadId = std::uint32_t;
    using ProcessId = std::uint32_t;

    [[nodiscard]] ThreadId threadId() const noexcept;
    [[nodiscard]] ProcessId processId() const noexcept;

    [[nodiscard]] bool isInCurrentThread() const noexcept;
    [[nodiscard]] bool isInCurrentProcess() const noexcept;


    void close() const noexcept;
    void destroy() const noexcept;
    
    [[deprecated("Using Win::killThread does not allow proper thread clean up.")]]
    void killThread(int __exitCode = -1) const noexcept;

    void killProcess(int __exitCode = -1) const noexcept;

    [[nodiscard]] String path() const noexcept;
    [[nodiscard]] WString WIN_FW(path)() const noexcept;

    [[nodiscard]] static bool hasPopupInScreen() noexcept;
    
    // --- Send or post a message ---

    struct Message
    {
        using msg_type = std::uint32_t;
        using wparam_type = std::uint64_t;
        using lparam_type = std::int64_t;

        msg_type    msg = 0;
        wparam_type wParam = 0;
        lparam_type lParam = 0;

        Message() = default;

        Message(msg_type __msg, wparam_type __wParam, lparam_type __lParam)
            : msg(__msg), wParam(__wParam), lParam(__lParam)
        { }
    };


    /**
    * https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
    */
    enum Key : std::uint32_t
    {
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
        Alt = 0x01,
        Ctrl = 0x02,
        Shift = 0x04,
        WIN = 0x08
    };

    struct Shortcut
    {
        Modifiers modifiers;
        Key key;

        Shortcut(Modifiers __modifiers, Key __key) noexcept
            : modifiers(__modifiers), key(__key) 
        { }

        friend inline Modifiers operator+(Modifiers __first, Modifiers __second) noexcept
        { return static_cast<Modifiers>(__first | __second); }

        friend inline Shortcut operator+(Modifiers __modifiers, Key __key) noexcept
        { return Shortcut(__modifiers, __key); }

        [[nodiscard]] int getId() const noexcept;
        [[nodiscard]] static Shortcut fromId(int __id) noexcept;

        inline bool operator<(Shortcut __other) const noexcept
        { return getId() < __other.getId(); }
    };

protected:

    void _M_sendMessageA(
        Message::msg_type __msg,
        Message::wparam_type __wParam,
        Message::lparam_type __lParam,
        Timeout __timeout) const noexcept;

    void _M_sendMessageW(
        Message::msg_type __msg,
        Message::wparam_type __wParam,
        Message::lparam_type __lParam,
        Timeout __timeout) const noexcept;

    union WM_CHAR_LPARAM
    {
        struct _Struct_t
        {
            /**
            * The repeat count for the current message. The value
            * is the number of times the keystroke is autorepeated
            * as a result of the user holding down the key. If the
            * keystroke is held long enough, multiple messages are
            * sent. However, the repeat count is not cumulative.
            */
            std::uint32_t repeatCount      : 16;

            /**
            * The scan code. The value depends on the OEM.
            */
            std::uint32_t scanCode         : 8;

            /**
            * Indicates whether the key is an extended key, such as
            * the right-hand ALT and CTRL keys that appear on an
            * enhanced 101- or 102-key keyboard. The value is 1 if
            * it is an extended key; otherwise, it is 0.
            */
            std::uint32_t isExtendedKey    : 1;

            /**
            * Reserved; do not use.
            */
            std::uint32_t                  : 4;

            /**
            * The context code. The value is 1 if the ALT key is
            * held down while the key is pressed; otherwise, the
            * value is 0.
            */
            std::uint32_t contextCode      : 1;

            /**
            * The previous key state. The value is 1 if the key is
            * down before the message is sent, or it is 0 if the
            * key is up.
            */
            std::uint32_t previousKeyState : 1;

            /**
            * The transition state. The value is 1 if the key is
            * being released, or it is 0 if the key is being pressed.
            */
            std::uint32_t transitionState  : 1;

        } _struct_v;

        std::uint32_t _uint_v;
    };

public:

    /**
    * - send(): return after processing the message;
    * 
    * - post(): posts the message to the message queue of
    *           the current window thread and returns
    *           immediately;
    */

    /**
    * @param __timeout      Maximum waiting time for each
    *                       message, the function returns
    *                       without waiting for the
    *                       time-out period to elapse if
    *                       the receiving thread appears
    *                       to not respond or "hangs".
    * 
    *                       Call isHanging() to determine
    *                       whether the window is hanging.
    * 
    * @param __linebreadKey if true, call send(Key_Return)
    *                       when __text[i] is '\n' and
    *                       ignore '\r'.
    */
    void send(
        const String& __text,
        Timeout __timeout = DefaultTimeout,
        bool __linebreadKey = true) const noexcept;

    void post(
        const String& __text,
        bool __linebreadKey = true) const noexcept;

    void send(
        const WString& __text,
        Timeout __timeout = DefaultTimeout,
        bool __linebreadKey = true) const noexcept;

    void post(
        const WString& __text,
        bool __linebreadKey = true) const noexcept;

    void send(
        char __word,
        Timeout __timeout = DefaultTimeout) const noexcept;

    void post(char __word) const noexcept;

    void send(
        wchar_t __word,
        Timeout __timeout = DefaultTimeout) const noexcept;

    void post(wchar_t __word) const noexcept;

    void send(
        Key __key,
        Timeout __timeout = DefaultTimeout) const noexcept;
    
    void post(Key __key) const noexcept;

    enum KeyAction
    {
        OnlyPress,
        OnlyRelease
    };

    void send(
        Key __key,
        KeyAction __action,
        Timeout __timeout = DefaultTimeout) const noexcept;

    void post(Key __key, KeyAction __action) const noexcept;


    /**
    * @warning The current window must be created by the current
    *          thread.
    */
    Message waitMsg() const noexcept;
    Message waitMsg(Message::msg_type __msg) const noexcept;

    static Message waitCurrentThreadMsg() noexcept;
    static Message waitCurrentThreadMsg(Message::msg_type __msg) noexcept;

    /**
    * @brief Send a WM_CLEAR message.
    */
    void clear(Timeout __timeout = DefaultTimeout) const noexcept;
    
    /**
    * @brief Send a WM_COPY message.
    */
    void copy(Timeout __timeout = DefaultTimeout) const noexcept;
    
    /**
    * @brief Send a WM_CUT message.
    */
    void cut(Timeout __timeout = DefaultTimeout) const noexcept;
    
    /**
    * @brief Send a WM_PASTE message.
    */
    void paste(Timeout __timeout = DefaultTimeout) const noexcept;
    
    /**
    * @brief Send a WM_UNDO message.
    */
    void undo(Timeout __timeout = DefaultTimeout) const noexcept;


    static void wait(Timeout __ms) noexcept;

    // --- Global Shortcut ---

    /**
    * @brief Register a global shortcut, when the shortcut
    *        key is pressed, a WM_HOTKEY message
    *        [with wParam = __shortcut.getId()] will be
    *        sent to the current window.
    */
    void setShortcut(
        Shortcut __shortcut,
        bool __enable = true) const noexcept;

    using ShortcutFunctionParam = void*;
    using ShortcutFunction = void (*)(ShortcutFunctionParam);

protected:

    struct BindShortcutToFunction_Data
    {
        std::map<
            Shortcut,
            std::pair<
                ShortcutFunction,
                ShortcutFunctionParam>> _map;

        std::thread _thread;

        mutable std::mutex _mutex;

        std::forward_list<
            std::tuple<
                bool,
                Shortcut,
                ShortcutFunction,
                ShortcutFunctionParam>> _toBeDone;

        template<typename _Fn, typename... _Args>
        explicit BindShortcutToFunction_Data(_Fn&& __fn, _Args&&... __args) noexcept
            : _thread(std::forward<_Fn>(__fn), std::forward<_Args>(__args)...)
        { }

        ~BindShortcutToFunction_Data() noexcept;

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
    };

private:

    static void _S_bindShortcutToFunction_Proc() noexcept;

    static inline BindShortcutToFunction_Data* _S_bindShortcutToFunction_Data;

public:

    /**
    * @brief Register a global shortcut and create a new
    *        thread to receive messages when it is called
    *        for the first time. When the shortcut key is
    *        pressed, the bound function is called.
    */
    static void bindShortcutToFunction(
        Shortcut __shortcut,
        ShortcutFunction __function,
        ShortcutFunctionParam __param,
        bool __enable = true) noexcept;

    [[nodiscard]]
    static ShortcutFunction functionFromBoundShortcut(
        Shortcut __shortcut,
        ShortcutFunctionParam* __param = nullptr) noexcept;


    // --- Window Painter ---

    enum HorizontalAlignment : std::uint32_t
    {
        LeftAlign    = 0x00,
        HCenterAlign = 0x01,
        RightAlign   = 0x02
    };

    enum VerticalAlignment : std::int32_t
    {
        TopAlign     = 0x00,
        VCenterAlign = 0x04,
        BottomAlign  = 0x08
    };

    struct Alignment
    {
        HorizontalAlignment h;
        VerticalAlignment   v;

        Alignment(HorizontalAlignment __h, VerticalAlignment __v)
            : h(__h), v(__v)
        { }

        Alignment(VerticalAlignment __v, HorizontalAlignment __h)
            : h(__h), v(__v)
        { }

        friend inline Alignment operator+(HorizontalAlignment __h, VerticalAlignment __v)
        { return Alignment(__h, __v); }

        friend inline Alignment operator+(VerticalAlignment __v, HorizontalAlignment __h)
        { return Alignment(__h, __v); }

        friend inline Alignment operator|(HorizontalAlignment __h, VerticalAlignment __v)
        { return Alignment(__h, __v); }

        friend inline Alignment operator|(VerticalAlignment __v, HorizontalAlignment __h)
        { return Alignment(__h, __v); }
    };

    struct BezierVertex
    {
        Point forwardControl;
        Point vertex;
        Point backwardControl;
    };

    class Painter final
    {
    public:

        using Handle = void*;

        Painter() noexcept;

        explicit Painter(const Win& __win) noexcept;
        explicit Painter(Win::Handle __winHandle) noexcept;

        Painter(std::nullptr_t) noexcept;

        Painter(const Painter& __other) noexcept;
        Painter(Painter&& __other) noexcept;

        ~Painter() noexcept;

        Painter& operator=(const Painter& __other) noexcept;
        Painter& operator=(Painter&& __other) noexcept;

        [[nodiscard]] Win window() const noexcept;

        void setColorUnder(
            const Point& __point,
            const Color& __color) const noexcept;

        [[nodiscard]] Color colorUnder(const Point& __point) const noexcept;

        void setForegroundColor(const Color& __color) const noexcept;
        void setBackgroundColor(const Color& __color) const noexcept;

        void setBackgroundTransparent(bool __enable = true) const noexcept;

        [[nodiscard]] Color foregroundColor() const noexcept;
        [[nodiscard]] Color backgroundColor() const noexcept;

        [[nodiscard]] bool backgroundTransparent() const noexcept;


        /**
        * +------------------------------+
        * | Operand |       Meaning      |
        * +---------+--------------------+
        * | P       | Selected pen       |
        * | D       | Destination bitmap |
        * +---------+--------------------+
        *
        * +------------------------------+
        * | Operator |       Meaning     |
        * +----------+-------------------+
        * | a        | Bitwise  (&)  AND |
        * | n        | Bitwise  (~)  NOT |
        * | o        | Bitwise  (|)   OR |
        * | x        | Bitwise  (^)  XOR |
        * +----------+-------------------+
        *
        * reverse Polish notation
        */
        enum class BlendingModes : std::uint32_t
        {
            ZERO = 1,  // 0
            DPon,      // NOT(D OR P)
            DPna,      // D AND NOT(P)
            Pn,        // NOT(P)
            PDna,      // P AND NOT(D)
            Dn,        // NOT(D)
            DPx,       // D XOR P
            DPan,      // D AND NOT(P)
            DPa,       // D AND P
            DPxn,      // D XOR NOT(P)
            D,         // D
            DPno,      // D OR NOT(P)
            P,         // P
            PDno,      // D OR NOT(D)
            DPo,       // D OR P
            ONE        // 1
        };

        void setBlendingMode(BlendingModes __bm) const noexcept;
        [[nodiscard]] BlendingModes blendingMode() const noexcept;

        void drawText(
            const String& __text,
            const Point& __point) const noexcept;

        void drawText(
            const WString& __text,
            const Point& __point) const noexcept;

        void drawText(
            const String& __text,
            const Rect& __rect,
            Alignment __align,
            bool __singleLine = false) const noexcept;

        void drawText(
            const WString& __text,
            const Rect& __rect,
            Alignment __align,
            bool __singleLine = false) const noexcept;

    protected:

        void _M_setCurrentPosition(const Point& __point) const noexcept;
        [[nodiscard]] Point _M_currentPosition() const noexcept;

        void _M_drawLineTo(const Point& __point) const noexcept;

    public:

        void drawLine(
            const Point& __start,
            const Point& __end) const noexcept;

        void drawLines(const Point* __points, int __size) const noexcept;

        template<int _Size>
        inline void drawLines(const Point (&__points)[_Size]) const noexcept
        { drawLines(__points, _Size); }

        template<typename _Container>
        void drawLines(
            const std::enable_if_t<
                std::is_same_v<
                    std::remove_const_t<
                        std::remove_reference_t<
                            decltype(std::declval<_Container>().data())>>,
                Point>, _Container>& __c) const noexcept
        { drawLines(__c.data(), static_cast<int>(__c.size())); }

        template<typename _Container>
        void drawLines(
            const std::enable_if_t<
                std::is_same_v<
                    void,
                    std::void_t<
                        decltype(std::declval<_Container>().begin()),
                        decltype(std::declval<_Container>().end())>>,
                _Container>& __c) const noexcept
        { drawLines(__c.begin(), __c.end()); }

        template<
            typename _ForwardIterator,
            typename = std::enable_if_t<
                std::is_same_v<
                    std::remove_const_t<
                        std::remove_reference_t<
                            decltype(*std::declval<_ForwardIterator>())>>,
                    Point>>>
        inline void drawLines(_ForwardIterator __first, _ForwardIterator __last) const noexcept
        {
            if (__first == __last)
            {
                return;
            }

            _M_setCurrentPosition(*__first);

            for (++__first; __first != __last; ++__first)
            {
                _M_drawLineTo(*__first);
            }
        }

        void drawPolygon(const Point* __vertexes, int __size) const noexcept;

        void drawRect(const Rect& __rect) const noexcept;

        void drawRect(const Rect& __rect, const Size& __round) const noexcept;

        void drawCircle(
            const Point& __center,
            int __radius,
            float __startAngle = 0,
            float __sweepAngle = 360) const noexcept;

        void drawEllipse(const Rect& __rect) const noexcept;

        void drawArc(
            const Rect& __rect,
            const Point& __start,
            const Point& __end) const noexcept;

        void drawChord(
            const Rect& __rect,
            const Point& __start,
            const Point& __end) const noexcept;

        void drawPie(
            const Rect& __rect,
            const Point& __start,
            const Point& __end) const noexcept;

    protected:

        void _M_drawPolyBezier(
            const Point* __points,
            int __size) const noexcept;

    public:


        /**
        * @brief Draw a Cubic Bezier curve.
        * 
        * See also https://en.wikipedia.org/wiki/B%C3%A9zier_curve
        * for more detail.
        */
        template<
            typename _ForwardIterator,
            typename = std::enable_if_t<
                std::is_same_v<
                    std::remove_const_t<
                        std::remove_reference_t<
                            decltype(*std::declval<_ForwardIterator>())>>,
                    BezierVertex>>>
        inline void drawPolyBezier(
            _ForwardIterator __first,
            _ForwardIterator __last,
            std::size_t __size = 0) const noexcept
        {
            std::vector<Point> v;

            if (__size > 0)
            {
                v.reserve(__size * 3);
            }

            if (__first == __last)
            {
                return;
            }

            v.push_back(__first->vertex);
            v.push_back(__first->backwardControl);

            for (++__first; __first != __last; ++__first)
            {
                v.push_back(__first->forwardControl);
                v.push_back(__first->vertex);
                v.push_back(__first->backwardControl);
            }

            v.pop_back();
            _M_drawPolyBezier(v.data(), v.size());
        }

        template<
            typename _Container,
            typename = std::enable_if_t<
                std::is_same_v<typename _Container::value_type, BezierVertex>,
                std::void_t<
                    decltype(std::declval<_Container>().begin()),
                    decltype(std::declval<_Container>().end()),
                    decltype(std::declval<_Container>().size())>>>
        inline void drawPolyBezier(const _Container& __points) const noexcept
        { drawPolyBezier(__points.begin(), __points.end(), __points.size()); }


        void invert(const Rect& __rect) const noexcept;

    private:

        Handle _M_dc;
        float _M_dpi;
    };

    [[nodiscard]]
    std::unique_ptr<Painter> painter() const noexcept;

    [[nodiscard]]
    static Painter* screenPainter() noexcept;

    [[nodiscard]]
    Color colorUnder(const Point& __point) const noexcept;

    // --- Create a Window ---

    enum class ModalDialogButtonsId : std::uint32_t
    {
        AbortButton = 3,
        CancelButton = 2,
        ContinueButton = 11,
        IgnoreButton = 5,
        NoButton = 7,
        OkButton = 1,
        Retry = 4,
        Tryagain = 10,
        YesButton = 6
    };

    enum MessageLeval : std::int32_t
    {
        Information,  // (Ok)
        Question,     // (Yes), No, Cancel
        Warning,      // Cancel, (Try Again), Continue
        Error         // Abort, (Retry), Ignore
    };

    ModalDialogButtonsId createModalDialogBox(
        const String& __text,
        MessageLeval __leval) const noexcept;

    ModalDialogButtonsId createModalDialogBox(
        const WString& __text,
        MessageLeval __leval) const noexcept;

private:

    Handle _M_handle;
    ErrorStream* _M_errorStream;
};

#endif  // WIN_H