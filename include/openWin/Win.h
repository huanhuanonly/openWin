/**
* Copyright (c) 2024-2025 Yang Huanhuan (3347484963@qq.com).
* 
* This software is provided "as is", without warranty of any kind, express or implied.
*/

/**
* Win.h In the openWin (https://github.com/huanhuanonly/openWin)
* 
* Created by Yang Huanhuan on November 25, 2024, 11:15:05
* 
* --- This file is a part of openWin ---
* 
* @brief Encapsulates a window class (Win) to obtain window information or control the window.
*/

#pragma once

#ifndef OPENWIN_HEADER_WIN_H
#define OPENWIN_HEADER_WIN_H

#include <vector>
#include <string>

#include <iostream>

#include <cstdint>

#include "Geometry.h"
#include "ErrorStream.h"
#include "Wins.h"
#include "Key.h"

#include "pg/PathGenerator.h"
#include "pg/Direct.h"

/**
* @brief Use the WIN_FW macro to map a function name
*        with return value is a String to a function
*        name with return value is a WString.
*/
#define WIN_FW(__func) __func##_w

namespace win
{

class Painter;

class [[nodiscard]] Win
{
public:

    using Handle = void*;

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

    void setErrorStream(const ErrorStream& __es);
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

    using String = std::string;
    using WString = std::wstring;

    using PgPoint = pg::PathGenerator<Point, int, 2>;
    using PgPointDefault = pg::Direct<Point, int, 2>;

    using PgSize = pg::PathGenerator<Size, int, 2>;
    using PgSizeDefault = pg::Direct<Size, int, 2>;

    using PgRect = pg::PathGenerator<Rect, int, 4>;
    using PgRectDefault = pg::Direct<Rect, int, 4>;

    using PgInt = pg::PathGenerator<int, int, 1>;
    using PgIntDefault = pg::Direct<int, int, 1>;

    // --- Get a window ---
    
    static Win fromPoint(const Point& __point) noexcept;
    
    static inline Win fromPoint(int __x, int __y) noexcept
    { return fromPoint(Point(__x, __y)); }

    static Win fromTitle(const String& __title) noexcept;
    static Win fromTitle(const WString& __title) noexcept;

    static Win fromClassName(const String& __className) noexcept;
    static Win fromClassName(const WString& __className) noexcept;

    static Win fromTitleAndClassName(
        const String& __title,
        const String& __className) noexcept;

    static Win fromTitleAndClassName(
        const WString& __title,
        const WString& __className) noexcept;

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
        PgPoint* __pg = PgPointDefault::global()) const noexcept;

    inline void setPos(
        int __x, int __y,
        PgPoint* __pg = PgPointDefault::global()) const noexcept
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
        PgPoint* __pg = PgPointDefault::global()) const noexcept;

    [[nodiscard]] Point pos() const noexcept;

    void move(
        int __addX, int __addY,
        PgPoint* __pg = PgPointDefault::global()) const noexcept;

    inline void moveTo(
        const Point& __point,
        PgPoint* __pg = PgPointDefault::global()) const noexcept
    { setPos(__point, __pg); }

    inline void moveTo(
        int __x, int __y,
        PgPoint* __pg = PgPointDefault::global()) const noexcept
    { setPos(Point(__x, __y), __pg); }

    inline void moveTo(
        PosFlag __flag,
        int __reserve = 0,
        PgPoint* __pg = PgPointDefault::global()) const noexcept
    { setPos(__flag, __reserve, __pg); }


    void setSize(
        const Size& __size,
        PgSize* __pg = PgSizeDefault::global()) const noexcept;

    inline void setSize(
        int __x, int __y,
        PgSize* __pg = PgSizeDefault::global()) const noexcept
    { setSize(Size(__x, __y), __pg); }

    [[nodiscard]] Size size() const noexcept;

    [[nodiscard]] static Size screenSize() noexcept;

    void setWidth(
        int __width,
        PgInt* __pg = PgIntDefault::global()) const noexcept;

    void setHeight(
        int __height,
        PgInt* __pg = PgIntDefault::global()) const noexcept;

    [[nodiscard]] int width() const noexcept;
    [[nodiscard]] int height() const noexcept;

    [[nodiscard]] static int screenWidth() noexcept;
    [[nodiscard]] static int screenHeight() noexcept;

    void setZoom(
        int __addWidth, int __addHeight,
        PgSize* __pg = PgSizeDefault::global()) const noexcept;

    void setZoom(
        double __scaleX, double __scaleY,
        PgSize* __pg = PgSizeDefault::global()) const noexcept;

    
    /**
    * @param __value [0, 255] If it is 0, the current
    *                window is completely transparent.
    */
    void setOpacity(
        int __value,
        PgInt* __pg = PgIntDefault::global()) const noexcept;

    [[nodiscard]] int opacity() const noexcept;


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

        HelpButton = 0x08  /// @warning The HelpButton cannot be
                           ///          used with MaximizeButton
                           ///          or MinimizeButton.
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
    void sendClearMsg(Timeout __timeout = DefaultTimeout) const noexcept;
    
    /**
    * @brief Send a WM_COPY message.
    */
    void sendCopyMsg(Timeout __timeout = DefaultTimeout) const noexcept;
    
    /**
    * @brief Send a WM_CUT message.
    */
    void sendCutMsg(Timeout __timeout = DefaultTimeout) const noexcept;
    
    /**
    * @brief Send a WM_PASTE message.
    */
    void sendPasteMsg(Timeout __timeout = DefaultTimeout) const noexcept;
    
    /**
    * @brief Send a WM_UNDO message.
    */
    void sendUndoMsg(Timeout __timeout = DefaultTimeout) const noexcept;


    /**
    * @brief Register a global shortcut, when the shortcut
    *        key is pressed, a WM_HOTKEY message
    *        [with wParam = __shortcut.getId()] will be
    *        sent to the current window.
    */
    void setShortcut(
        Shortcut __shortcut,
        bool __enable = true) const noexcept;


    [[nodiscard]]
    std::unique_ptr<Painter> painter() const noexcept;

    [[nodiscard]]
    static Painter* screenPainter() noexcept;

    // --- Create a Window ---

    enum class ModalDialogButtonsId : std::uint32_t
    {
        AbortButton = 3,
        CancelButton = 2,
        ContinueButton = 11,
        IgnoreButton = 5,
        NoButton = 7,
        OkButton = 1,
        RetryButton = 4,
        TryAgainButton = 10,
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

}  // namespace win

#endif  // OPENWIN_HEADER_WIN_H