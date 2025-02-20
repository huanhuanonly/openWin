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

#include "pg/BasicPathGenerator.h"

/**
* @brief Uses the WIN_FW macro to map a function name with return value is a
*        String to a function name with return value is a WString.
*/
#define WIN_FW(__func) __func##_w

namespace win
{

class Painter;

class [[nodiscard]] Win
{
public:

    using Handle = void*;

    using String = std::string;
    using WString = std::wstring;

    using ThreadId = std::uint32_t;
    using ProcessId = std::uint32_t;

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

    /* ================== error stream ================== */

    void setErrorStream(const ErrorStream& __es);
    [[nodiscard]] ErrorStream& errorStream() noexcept;
    [[nodiscard]] const ErrorStream& errorStream() const noexcept;

    [[nodiscard]] bool success() const noexcept;
    [[nodiscard]] bool failed() const noexcept;

    /* ================== output to ostream ================== */

    friend inline std::ostream& operator<<(std::ostream& __os, const Win& __win) noexcept
    {
        __os << "Win { [" << __win._M_handle;
        __os << "] | title = " << __win.title();
        __os << ", class = " << __win.className();
        __os << " }";

        return __os;
    }

    /* ================== get a window ================== */
    
    /**
     * @param  __point The point to be checked.
     * 
     * @return The window that contains the point. If no window exists at the
     *         given point, returns an empty window. If the point is over a
     *         static text control, returns the window under the static text
     *         control.
     */
    static Win findByPoint(const Point& __point) noexcept;
    
    static inline Win findByPoint(int __x, int __y) noexcept
    { return findByPoint(Point(__x, __y)); }

    /**
     * @brief Retrieves a top-level window whose window name match the
     *        specified strings.
     * 
     * @param __title The window name (the window's title). If this parameter
     *                is empty, all window names match.
     * 
     * @note  This function does not search child windows.
     *        This function does not perform a case-sensitive search.
     */
    static Win findByTitle(const String& __title) noexcept;
    static Win findByTitle(const WString& __title) noexcept;

    /**
     * @brief Retrieves a top-level window whose class name match the specified
     *        strings.
     * 
     * @param __className The class name.
     * 
     * @note  This function does not search child windows.
     *        This function does not perform a case-sensitive search.
     */
    static Win findByClassName(const String& __className) noexcept;
    static Win findByClassName(const WString& __className) noexcept;

    /**
     * @brief Retrieves a top-level window whose window name and class name
     *        match the specified strings.
     * 
     * @param __title     The window name (the window's title). If this
     *                    parameter is empty, all window names match.
     * 
     * @param __className The class name.
     * 
     * @note  This function does not search child windows.
     *        This function does not perform a case-sensitive search.
     */
    static Win findByTitleAndClassName(
        const String& __title,
        const String& __className) noexcept;

    static Win findByTitleAndClassName(
        const WString& __title,
        const WString& __className) noexcept;

    /**
     * @return The foreground window (the window with which the user is
     *         currently working). The system assigns a slightly higher
     *         priority to the thread that creates the foreground window than
     *         it does to other threads.
     * 
     * @note   The foreground window can be empty in certain circumstances,
     *         such as when a window is losing activation.
     */
    static Win currentForegroundWindow() noexcept;

    /**
     * @return The Shell's desktop window. If no Shell process is present,
     *         returns an empty window.
     */
    static Win currentShellWindow() noexcept;

    /**
     * @return The desktop window. The desktop window covers the entire screen.
     *         The desktop window is the area on top of which other windows are
     *         painted.
     */
    static Win currentDesktopWindow() noexcept;

    /**
     * @return The window used by the console associated with the calling
     *         process or empty window if there is no such associated console.
     */
    static Win currentConsoleWindowInCurrentThread() noexcept;

    /**
     * @return The window with the keyboard focus.
     * 
     * @note   If returns an empty window, another thread's queue may be
     *         attached to a window that has the keyboard focus.
     */
    static Win currentFocusWindowInCurrentThread() noexcept;

    /**
     * @return The active window attached to the calling thread's message queue.
     *         Otherwise, returns an empty window.
     */
    static Win currentActiveWindowInCurrentThread() noexcept;

    /**
     * @return The capture window associated with the current thread. If no
     *         window in the thread has captured the mouse, returns an empty
     *         window.
     */
    static Win currentCaptureWindowInCurrentThread() noexcept;

    static Win currentFocusWindowFromThread(ThreadId __threadId) noexcept;
    static Win currentActiveWindowFromThread(ThreadId __threadId) noexcept;
    static Win currentCaptureWindowFromThread(ThreadId __threadId) noexcept;
    
    /* ================== swap and compare ================== */

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

    /* ================== get window validity ================== */

    /**
     * @return true if the current window is valid; otherwise, it is false.
     */
    [[nodiscard]]
    bool isValid() const noexcept;

    [[nodiscard]]
    static bool isValid(Handle __handle) noexcept;

    /**
     * @brief Same as isValid().
     */
    [[nodiscard]]
    inline bool exists() const noexcept
    { return isValid(); }

    [[nodiscard]]
    static inline bool exists(Handle __handle) noexcept
    { return isValid(__handle); }

    /**
     * @return true if the current window is an empty window; otherwise, it is
     *         false.
     */
    [[nodiscard]]
    bool isEmpty() const noexcept;

    [[nodiscard]]
    static bool isEmpty(Handle __handle) noexcept;

    inline operator bool() const noexcept
    { return isEmpty(); }

    /**
     * @brief Same as isEmpty().
     */
    [[nodiscard]]
    inline bool empty() const noexcept
    { return isEmpty(); }

    [[nodiscard]]
    static inline bool empty(Handle __handle) noexcept
    { return isEmpty(__handle); }


    /**
     * @brief  Determines whether the system considers that a specified
     *         application is not responding. An application is considered to
     *         be not responding if it is not waiting for input, is not in
     *         startup processing, and has not called PeekMessage() within the
     *         internal timeout period of 5 seconds.
     * 
     * @return true if the current window stops responding; otherwise, it is
     *         false. Ghost windows always returns true.
     * 
     * @note   The Windows timeout criteria of 5 seconds is subject to change.
     */
    [[nodiscard]]
    bool isNotResponding() const noexcept;

    /**
     * @brief Same as isNotResponding().
     */
    [[nodiscard]]
    inline bool isHanging() const noexcept
    { return isNotResponding(); }

    /* ================== basic state of the window ================== */

    /**
     * @brief Enables or disables mouse and keyboard input to the current
     *        window or control. When input is disabled, the window does not
     *        receive input such as mouse clicks and key presses. When input is
     *        enabled, the window receives all input.
     */
    void setEnable(bool __enable = true) const noexcept;
    [[nodiscard]] bool isEnabled() const noexcept;

    /**
     * @brief Same as setEnable(true).
     */
    inline void enable() const noexcept
    { setEnable(true); }

    /**
     * @brief Same as setEnable(false).
     */
    inline void disable() const noexcept
    { setEnable(false); }

    /**
     * @brief   Activates the current window.
     * 
     * @warning The window must be attached to the calling thread's message
     *          queue.
     */
    void setActive() const noexcept;
    [[nodiscard]] bool isActive() const noexcept;

    /**
     * @brief If __enable is true, The current window does not become the
     *        foreground window when the user clicks it. The system does not
     *        bring this window to the foreground when the user minimizes or
     *        closes the foreground window. To activate the window, use the
     *        setActive() or setForeground() function.
     */
    void setNoActive(bool __enable = true) const noexcept;
    [[nodiscard]] bool isNoActive() const noexcept;

    /**
     * @brief Brings the thread that created the current window into the
     *        foreground and activates the window. Keyboard input is directed
     *        to the window, and various visual cues are changed for the user.
     *        The system assigns a slightly higher priority to the thread that
     *        created the foreground window than it does to other threads.
     * 
     * @param __lock If true, lockSetForeground() will be called at the end.
     */
    void setForeground(bool __lock = false) const noexcept;
    [[nodiscard]] bool isForeground() const noexcept;

    /**
     * @brief The foreground process can call this function to disable calls to
     *        the setForeground() function.
     */
    static void lockSetForeground() noexcept;
    static void unlockSetForeground() noexcept;

    /**
     * @brief Sets the keyboard focus to the current window.
     */
    void setFocus() const noexcept;
    [[nodiscard]] bool hasFocus() const noexcept;

    /**
     * @brief Sets the mouse capture to the current window belonging to the
     *        current thread.
     * 
     * @note  Only one window can capture the mouse at a time, this window
     *        receives mouse input whether or not the cursor is within its
     *        borders.
     * 
     *        This function cannot be used to capture mouse input meant for
     *        another process.
     * 
     *        If the mouse cursor is over a window created by another thread,
     *        the system will direct mouse input to the specified window only
     *        if a mouse button is down.
     * 
     *        Only the foreground window can capture the mouse. When a
     *        background window attempts to do so, the window receives messages
     *        only for mouse events that occur when the cursor hot spot is
     *        within the visible portion of the window. Also, even if the
     *        foreground window has captured the mouse, the user can still
     *        click another window, bringing it to the foreground.
     * 
     *        When the mouse is captured, menu hotkeys and other keyboard
     *        accelerators do not work.
     */
    void setCapture(bool __enable = true) const noexcept;
    [[nodiscard]] bool hasCapture() const noexcept;

    enum CharSetCode : bool
    {
        ANSI,
        Unicode
    };

    /**
     * @brief Determines whether the current window is a native Unicode window.
     * 
     * @note  The character set used by the current window depends on whether
     *        RegisterClassA() or RegisterClassW() was called when creating the
     *        window class.
     */
    [[nodiscard]]
    CharSetCode charSetCode() const noexcept;

    /**
     * @return the dots per inch (dpi) value for the window.
     */
    [[nodiscard]]
    float dpi() const noexcept;

    /**
     * @return the dots per inch (dpi) value for the system.
     */
    [[nodiscard]]
    static float systemDpi() noexcept;

    /* ================== parent, owner and child ================== */

    /**
     * @brief   Sets the parent window of the current window.
     * 
     * @param   __newParent The new parent window.
     * 
     * @warning Unexpected behavior or errors may occur if the new parent
     *          window and the current window are running in different DPI
     *          awareness modes.
     */
    void setParent(const Win& __newParent) const noexcept;

    /**
     * @brief Sets the parent window of the current window to empty.
     */
    void setParent(std::nullptr_t) const noexcept;

    /**
    * @param __onlyParent If true, returns owner of the current window when the
    *                     current is a child window.
    */
    Win parent(bool __onlyParent = false) const noexcept;
    [[nodiscard]] bool hasParent() const noexcept;

    /**
     * @return The current window's owner window, if any.
     * 
     * @note   An overlapped or pop-up window can be owned by another
     *         overlapped or pop-up window. Being owned places several
     *         constraints on a window.
     * 
     *       - An owned window is always above its owner in the z-order.
     *       - The system automatically destroys an owned window when its owner
     *         is destroyed.
     *       - An owned window is hidden when its owner is minimized.
     * 
     *         A child window cannot be an owner window.
     *         Dialog boxes and message boxes are owned windows by default.
     * 
     *         After creating an owned window, an application cannot transfer
     *         ownership of the window to another window.
     */
    Win owner() const noexcept;

    /**
     * @return The root window by walking the chain of parent windows.
     */
    Win root() const noexcept;

    /**
     * @return The owned root window by walking the chain of parent and owner
     *         windows.
     */
    Win ownedRoot() const noexcept;

    /**
    * @return A list of all child windows of the current window.
    */
    [[nodiscard]] WinList children() const noexcept;

    /**
     * @return true if the current window has at least one child window;
     *         otherwise, returns false.
     */
    [[nodiscard]] bool hasChild() const noexcept;

    /**
    * @return The child window at the top of the Z-order. If the current window
    *         has no child windows, returns an empty window.
    */
    Win topChild() const noexcept;

    /**
     * @return The popup window owned by the current window that was most
     *         recently active.
     * 
     *         Returns the same as the current window if any of the following
     *         conditions are met:
     * 
     *       - The current window was recently active.
     * 
     *       - The current window does not own any pop-up windows.
     * 
     *       - The current window is not a top-level window, or it is owned by
     *         another window.
     */
    Win lastActivePopup() const noexcept;

    [[nodiscard]] bool isParent() const noexcept;
    [[nodiscard]] bool isChild() const noexcept;

    /**
     * @return true if the current window is a child window of __parent;
     *         otherwise, returns false.
     */
    [[nodiscard]] bool isChildOf(const Win& __parent) const noexcept;

    enum Orientation
    {
        Vertical   = 0x01,
        Horizontal = 0x02
    };

    /**
     * @brief Tiles all child windows of the current window (parent).
     * 
     * @return The number of windows arranged.
     * 
     * @note  Calling this function causes all maximized windows to be restored
     *        to their previous size.
     */
    int setTile(Orientation __orientation) const noexcept;
    int setTile(Orientation __orientation, const Rect& __clientRect) const noexcept;

    /**
    * @return A list of all top-level windows on the screen.
    */
    [[nodiscard]]
    static WinList list() noexcept;

    /**
     * @return A list of all non-child windows in the thread that created the
     *         current window.
     */
    [[nodiscard]]
    WinList listInSameThread() const noexcept;

    /**
     * @return A list of all non-child windows in the specified thread.
     */
    [[nodiscard]]
    static WinList listFromThread(ThreadId __threadId) noexcept;

    /* ================== z-order ================== */

    /**
     * @brief Places the current window at the top of the Z-order.
     */
    void setZOrderTop() const noexcept;

    /**
     * @brief Places the current window at the bottom of the Z-order.
     * 
     * @note  If the current window is a topmost window, the window loses its
     *        topmost status and is placed at the bottom of all other windows.
     */
    void setZOrderBottom() const noexcept;
    
    /**
     * @brief Places the current window above all non-topmost windows. The
     *        current window maintains its topmost position even when it is
     *        deactivated.
     */
    void setTopmost(bool __enable = true) const noexcept;
    [[nodiscard]] bool isTopmost() const noexcept;

    /**
     * @brief Same as setTopmost().
     */
    void setZOrderTopmost(bool __enable = true) const noexcept
    { setTopmost(__enable); }

    /**
     * @brief Same as isTopmost().
     */
    [[nodiscard]] bool isZOrderTopmost() const noexcept
    { return isTopmost(); }

    /**
     * @return The window of the same type that is highest in the Z-order.
     */
    Win highest() const noexcept;

    /**
     * @return The window of the same type that is lowest in the Z-order.
     */
    Win lowest() const noexcept;
    
    /**
     * @return The window below the current window in the Z-order.
     */
    Win below() const noexcept;

    /**
     * @return The window above the current window in the Z-order.
     */
    Win above() const noexcept;

    /* ================== types of windows ================== */

    /**
     * @brief Make the current window a pop-up window.
     * 
     *        About Pop-up Windows:
     * 
     *        A pop-up window is a special type of overlapped window used for
     *        dialog boxes, message boxes, and other temporary windows that
     *        appear outside an application's main window. Title bars are
     *        optional for pop-up windows.
     */
    void becomePopup(bool __enable = true) const noexcept;
    [[nodiscard]] bool isPopup() const noexcept;

    /**
     * @return true if a pop-up window exists, even if the pop-up window is
     *         completely covered by other windows; otherwise, returns false.
     * 
     * @note   This function does not detect unowned pop-up windows or windows
     *         that do not have the WS_VISIBLE (0x10000000) style bit set.
     */
    [[nodiscard]] static bool hasPopupInScreen() noexcept;

    /**
     * @brief Make the current window a tool window.
     * 
     *        About Tool Windows:
     * 
     *        The window is intended to be used as a floating toolbar. A tool
     *        window has a title bar that is shorter than a normal title bar,
     *        and the window title is drawn using a smaller font. A tool window
     *        does not appear in the taskbar or in the dialog that appears when
     *        the user presses <ALT+TAB>. If a tool window has a system menu,
     *        its icon is not displayed on the title bar. However, you can
     *        display the system menu by right-clicking or by typing <ALT+SPACE>.
     */
    void becomeTool(bool __enable = true) const noexcept;
    [[nodiscard]] bool isTool() const noexcept;

    /**
     * @brief Make the current window a layered window.
     * 
     *        About Layered Windows:
     * 
     *        Using a layered window can significantly improve performance and
     *        visual effects for a window that has a complex shape, animates
     *        its shape, or wishes to use alpha blending effects. The system
     *        automatically composes and repaints layered windows and the
     *        windows of underlying applications. As a result, layered windows
     *        are rendered smoothly, without the flickering typical of complex
     *        window regions. In addition, layered windows can be partially
     *        translucent, that is, alpha-blended.
     */
    void becomeLayered(bool __enable = true) const noexcept;
    [[nodiscard]] bool isLayered() const noexcept;

    [[nodiscard]] bool isMDIChild() const noexcept;

    /* ================== window appearance ================== */

    /**
     * @brief Activates the current window and displays it in its current size
     *        and position.
     */
    void show() const noexcept;

    /**
     * @brief Hides the current window and activates another window.
     */
    void hide() const noexcept;

    /**
     * @brief Shows or hides the current window.
     * 
     * @param __enable If true, show() will be called, otherwise, hide() will
     *                 be called.
     */
    inline void setVisible(bool __enable = true) const noexcept
    { return __enable ? show() : hide(); }

    [[nodiscard]] bool isVisible() const noexcept;
    
    /**
    * @brief Shows all pop-up windows owned by the current window.
    */
    void showPopups() const noexcept;

    /**
     * @brief Hides all pop-up windows owned by the current window.
     */
    void hidePopups() const noexcept;

    /**
     * @brief   The current window is displayed only on a monitor. Everywhere
     *          else, the window does not appear at all.
     * 
     * @warning The current window must be a top-level window.
     * 
     *          The current window must belong to the current process.
     */
    void setDisplayProtection(bool __enable = true) const noexcept;
    [[nodiscard]] bool isDisplayProtected() const noexcept;

    /**
     * @brief Activates the current window and displays it as a maximized
     *        window.
     */
    void maximize() const noexcept;

    /**
     * @brief Minimizes the current window and activates the next top-level
     *        window in the Z-order, even if the thread (different thread) that
     *        owns the current window is not responding.
     */
    void minimize() const noexcept;

    /**
     * @brief Activates and displays the current window. If the current window
     *        is minimized, maximized, or arranged, the system restores it to
     *        its original size and position.
     */
    void restore() const noexcept;

    [[nodiscard]] bool isMaximized() const noexcept;
    [[nodiscard]] bool isMinimized() const noexcept;
    [[nodiscard]] bool isRestored() const noexcept;
    [[nodiscard]] bool isArranged() const noexcept;


    /**
     * @brief Changes the text of the current window's title bar (if it has one).
     *        If the specified window is a system control, the text of the
     *        control is changed.
     * 
     * @param __title The new title.
     */
    void setTitle(const String& __title) const noexcept;
    void setTitle(const WString& __title) const noexcept;

    [[nodiscard]] String title() const noexcept;
    [[nodiscard]] WString WIN_FW(title)() const noexcept;

    [[nodiscard]] String className() const noexcept;
    [[nodiscard]] WString WIN_FW(className)() const noexcept;

    /**
     * @return The executable file path of the process that created the current
     *         window.
     */
    [[nodiscard]] String path() const noexcept;
    [[nodiscard]] WString WIN_FW(path)() const noexcept;
    
    /**
     * @brief Sets the position and size of the current window on the screen.
     * @param __rect The new position and size.
     */
    void setRect(const Rect& __rect) const noexcept;
    [[nodiscard]] Rect rect() const noexcept;

    /**
     * @return The client's position and size of the current window.
     */
    [[nodiscard]] Rect clientRect() const noexcept;

    /**
     * @brief Sets the position of the current window on the screen.
     * 
     * @param __point The new position.
     */
    void setPos(const Point& __point) const noexcept;
    
    inline void setPos(int __x, int __y) const noexcept
    { setPos(Point(__x, __y)); }

    void setPos(
        const Point& __point,
        const pg::BasicPathGenerator<Point>& __pg) const noexcept;

    inline void setPos(
        int __x, int __y,
        const pg::BasicPathGenerator<Point>& __pg) const noexcept
    { setPos(Point(__x, __y), __pg); }

    enum PosFlag : int
    {
        TopLeftCorner, TopRightCorner,

        Center,  // Ignore the parameter __reserve.

        BottomLeftCorner, BottomRightCorner
    };

    /**
     * @brief Sets the position of the current window on the screen.
     * 
     * @param __flag    The position flag of the screen.
     * @param __reserve Reserves spaces.
     */
    void setPos(
        PosFlag __flag,
        int __reserve = 0) const noexcept;

    inline void setPos(
        PosFlag __flag,
        const pg::BasicPathGenerator<Point>& __pg) const noexcept
    { setPos(__flag, 0, __pg); }

    void setPos(
        PosFlag __flag,
        int __reserve,
        const pg::BasicPathGenerator<Point>& __pg) const noexcept;

    /**
     * @return The position of the current window on the screen.
     */
    [[nodiscard]] Point pos() const noexcept;


    void move(int __addX, int __addY) const noexcept;

    void move(
        int __addX, int __addY,
        const pg::BasicPathGenerator<Point>& __pg) const noexcept;

    /**
     * @brief Same as setPos().
     * 
     * @param __point The new position.
     */
    inline void moveTo(const Point& __point) const noexcept
    { setPos(__point); }

    inline void moveTo(int __x, int __y) const noexcept
    { setPos(Point(__x, __y)); }

    inline void moveTo(PosFlag __flag) const noexcept
    { setPos(__flag, 0); }

    inline void moveTo(PosFlag __flag, int __reserve) const noexcept
    { setPos(__flag, __reserve); }


    inline void moveTo(
        const Point& __point,
        const pg::BasicPathGenerator<Point>& __pg) const noexcept
    { setPos(__point, __pg); }

    inline void moveTo(
        int __x, int __y,
        const pg::BasicPathGenerator<Point>& __pg) const noexcept
    { setPos(Point(__x, __y), __pg); }

    inline void moveTo(
        PosFlag __flag,
        const pg::BasicPathGenerator<Point>& __pg) const noexcept
    { setPos(__flag, 0, __pg); }

    inline void moveTo(
        PosFlag __flag,
        int __reserve,
        const pg::BasicPathGenerator<Point>& __pg) const noexcept
    { setPos(__flag, __reserve, __pg); }


    void setSize(const Size& __size) const noexcept;

    inline void setSize(int __w, int __h) const noexcept
    { setSize(Size(__w, __h)); }

    void setSize(
        const Size& __size,
        const pg::BasicPathGenerator<Size>& __pg) const noexcept;

    inline void setSize(
        int __w, int __h,
        const pg::BasicPathGenerator<Size>& __pg) const noexcept
    { setSize(Size(__w, __h), __pg); }

    [[nodiscard]] Size size() const noexcept;

    [[nodiscard]] static Size screenSize() noexcept;

    void setWidth(int __width) const noexcept;

    void setWidth(
        int __width,
        const pg::BasicPathGenerator<int>& __pg) const noexcept;

    void setHeight(int __height) const noexcept;

    void setHeight(
        int __height,
        const pg::BasicPathGenerator<int>& __pg) const noexcept;

    [[nodiscard]] int width() const noexcept;
    [[nodiscard]] int height() const noexcept;

    [[nodiscard]] static int screenWidth() noexcept;
    [[nodiscard]] static int screenHeight() noexcept;

    void setZoom(int __additionalWidth, int __additionalHeight) const noexcept;

    inline void setZoom(int __additional) const noexcept
    { setZoom(__additional, __additional); }

    void setZoom(
        int __additionalWidth, int __additionalHeight,
        const pg::BasicPathGenerator<Size>& __pg) const noexcept;

    inline void setZoom(
        int __additional,
        const pg::BasicPathGenerator<Size>& __pg) const noexcept
    { setZoom(__additional, __additional, __pg); }


    void setZoom(double __scaleX, double __scaleY) const noexcept;

    inline void setZoom(double __scale) const noexcept
    { setZoom(__scale, __scale); }

    void setZoom(
        double __scaleX, double __scaleY,
        const pg::BasicPathGenerator<Size>& __pg) const noexcept;

    inline void setZoom(
        double __scale,
        const pg::BasicPathGenerator<Size>& __pg) const noexcept
    { setZoom(__scale, __scale, __pg); }

    
    /**
    * @brief Sets the opacity of the current window.
    * 
    * @param __value [0, 255] The new opacity, if it is 0, the current window
    *                is completely transparent.
    */
    void setOpacity(int __value) const noexcept;

    void setOpacity(
        int __value,
        const pg::BasicPathGenerator<int>& __pg) const noexcept;

    /**
     * @return [0, 255] The opacity of the current window, if it is 0, the
     *         current window is completely transparent.
     */
    [[nodiscard]] int opacity() const noexcept;

    /**
     * @brief Sets the transparency color key of the current window.
     * 
     * @param __color The new transparency color key. All pixels painted by the
     *                currnet window in this color will be transparent.
     */
    void setTransparencyColor(const Color& __color) const noexcept;
    [[nodiscard]] Color transparencyColor() const noexcept;

    /* ================== coordinate system transformation ================== */

    /**
     * @brief Converts the client-area coordinates of a specified point to
     *        screen coordinates.
     * 
     * @param __clientPoint The client-area coordinates.
     */
    [[nodiscard]] Point toScreenCoordinates(const Point& __clientPoint) const noexcept;

    /**
     * @brief Converts the screen coordinates of a specified point on the
     *        screen to client-area coordinates.
     * 
     * @param __screenPoint The screen coordinates.
     */
    [[nodiscard]] Point toClientCoordinates(const Point& __screenPoint) const noexcept;

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

    /* ================== system window frame ================== */

    /**
     * @brief The current window will have a thin-line border.
     */
    void setBorder(bool __enable = true) const noexcept;
    [[nodiscard]] bool hasBorder() const noexcept;

    /**
     * @brief The current window will have a border with a sunken edge.
     */
    void setSunkenEdge(bool __enable = true) const noexcept;
    [[nodiscard]] bool hasSunkenEdge() const noexcept;

    /**
     * @brief The current window will have a border with a raised edge.
     */
    void setRaisedEdge(bool __enable = true) const noexcept;
    [[nodiscard]] bool hasRaisedEdge() const noexcept;

    /**
     * @brief The current window will have a three-dimensional border style
     *        intended to be used for items that do not accept user input.
     */
    void setStaticEdge(bool __enable = true) const noexcept;
    [[nodiscard]] bool hasStaticEdge() const noexcept;

    /**
     * @brief The current window will have a title bar and a thin-line border.
     */
    void setTitlebar(bool __enable = true) const noexcept;
    [[nodiscard]] bool hasTitlebar() const noexcept;

    /**
     * @brief The current window will have a window menu on its title bar.
     */
    void setMenubar(bool __enable = true) const noexcept;
    [[nodiscard]] bool hasMenubar() const noexcept;

    /**
     * @brief The current window will have a scroll bar.
     * 
     * @param __orientation Specifies vertical or horizontal or both vertical
     *                      and horizontal.
     * 
     * @note  Does not affect the state of the scroll bar that is not specified
     *        in the __orientation parameter.
     */
    void setScroll(Orientation __orientation, bool __enable = true) const noexcept;
    [[nodiscard]] bool hasScroll(Orientation __orientation) const noexcept;

    /**
     * @brief The current window will have a sizing border. 
     */
    void setSizingBorder(bool __enable = true) const noexcept;
    [[nodiscard]] bool hasSizingBorder() const noexcept;

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
    {
        return static_cast<TitlebarButtons>(
            static_cast<std::underlying_type_t<TitlebarButtons>>(__first) |
            static_cast<std::underlying_type_t<TitlebarButtons>>(__second));
    }

    /**
     * @brief The current window will have some buttons on its menu bar.
     * 
     * @param __enable If true, enables the title bar, menu bar, and the
     *                 buttons specified in the __titlebarButtons parameter;
     *                 otherwise, disables the buttons specified in
     *                 __titlebarButtons.
     * 
     * @note  Does not affect the state of the buttons that is not specified
     *        in the __titlebarButtons parameter.
     */
    void setTitlebarButtons(
        TitlebarButtons __titlebarButtons,
        bool __enable = true) const noexcept;

    [[nodiscard]] bool hasTitlebarButtons(
        TitlebarButtons __titlebarButtons) const noexcept;

    /* ================== window properties ================== */
    
    /**
     * @brief Forces the current window (must be a top-level window) onto the
     *        taskbar when the window is visible.
     */
    void setTaskbarIcon(bool __enable = true) const noexcept;
    [[nodiscard]] bool hasTaskbarIcon() const noexcept;

    /**
     * @brief Paints all descendants of the current window in bottom-to-top
     *        painting order using double-buffering. Bottom-to-top painting
     *        order allows a descendent window to have translucency (alpha) and
     *        transparency (color-key) effects, but only if the descendent
     *        window also has the WS_EX_TRANSPARENT (0x00000020) bit set.
     *        Double-buffering allows the window and its descendents to be
     *        painted without flicker.
     */
    void setDoubleBufferDrawing(bool __enable = true) const noexcept;
    [[nodiscard]] bool isDoubleBufferDrawing() const noexcept;

    /**
     * @brief Sets the current window to accept drag-drop files.
     */
    void setAcceptFiles(bool __enable = true) const noexcept;
    [[nodiscard]] bool isAcceptingFiles() const noexcept;

    using Timeout = std::uint32_t;

    enum TimeoutFlag : Timeout
    {
        DefaultTimeout = 5000U,
        InfiniteTimeout = 0xFFFFFFFFU
    };

    /**
     * @brief Flashes the current window one time.
     * 
     *        Typically, a window is flashed to inform the user that the window
     *        requires attention but that it does not currently have the
     *        keyboard focus.
     * 
     * @param __enable If false, the current stops flashing and the system
     *                 restores the window to its original state.
     * 
     * @note  This function does not change the active state of the current
     *        window.
     */
    void flash(bool __enable = true) const noexcept;

    /**
     * @brief Flashes the current window.
     * 
     * @param __count   The number of times to flashes the current window. If
     *                  zero or a negative number is specified, the current
     *                  window will flash continuously.
     * 
     * @param __timeout The rate at which the window is to be flashed, in
     *                  milliseconds. If dwTimeout is zero, the function uses
     *                  the default caret blink rate.
     * 
     * @param __caption If true, the current window caption and the taskbar
     *                  button will flash; otherwise, only the taskbar button
     *                  will flash.
     */
    void flash(int __count, Timeout __timeout = 0U, bool __caption = false) const noexcept;
    
    enum FlashFlag { UntilIsForeground };

    /**
     * @brief Flashs continuously until the current window comes to the
     *        foreground.
     */
    void flash(FlashFlag, Timeout __timeout = 0U, bool __caption = false) const noexcept;

    /**
     * @brief Repaints the current window.
     * 
     * @param __eraseBackground If true, the background will be erased before
     *                          redrawing.
     */
    void update(bool __eraseBackground = true) const noexcept;

    /**
     * @brief Repaints the specified area of the current window.
     * 
     * @param __clientRect The specified area.
     */
    void update(const Rect& __clientRect, bool __eraseBackground = true) const noexcept;

    /**
     * @brief Disables drawing in the current window.
     * 
     * @note  Only one window can be locked at a time.
     * 
     *        A locked window cannot be moved.
     */
    void lockUpdate() const noexcept;

    /**
     * @brief Enables drawing in any window.
     */
    static void unlockUpdate() noexcept;


    /**
     * @return The identifier of the thread that created the current window.
     */
    [[nodiscard]] ThreadId threadId() const noexcept;

    /**
     * @return The identifier of the process that created the current window.
     */
    [[nodiscard]] ProcessId processId() const noexcept;

    /**
     * @return true if the current window was created by the current thread;
     *         otherwise, returns false.
     */
    [[nodiscard]] bool isCreatedByCurrentThread() const noexcept;

    /**
     * @return true if the current window was created by the current process;
     *         otherwise, returns false.
     */
    [[nodiscard]] bool isCreatedByCurrentProcess() const noexcept;

    /* ================== close or destroy ================== */

    /**
     * @brief Sends a WM_CLOSE (0x0010) message to the current window.
     * 
     * @param __timeout Maximum waiting time (millisecond).
     */
    void close(Timeout __timeout = 0U) const noexcept;

    /**
     * @brief Destroys the current window.
     * 
     *        If the current window is a parent or owner window, this function
     *        automatically destroys the associated child or owned windows when
     *        it destroys the parent or owner window. This function first
     *        destroys child or owned windows, and then it destroys the parent
     *        or owner window.
     * 
     * @warning A thread cannot use this function to destroy a window created
     *          by a different thread.
     */
    void destroy() const noexcept;
    
    /**
     * @brief Kills the thread that created the current window.
     * 
     * @param __exitCode The exit code of the thread.
     */
    [[deprecated("Using Win::killThread() does not allow proper thread clean up.")]]
    void killThread(int __exitCode = -1) const noexcept;

    /**
     * @brief Kills the process that created the current window.
     * 
     * @param __exitCode The exit code of the process.
     */
    void killProcess(int __exitCode = -1) const noexcept;
    
    /* ================== send or post messages ================== */

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
    * - send(): returns after processing the message;
    * 
    * - post(): posts the message to the message queue of the current window
    *           thread and returns immediately;
    */

    /**
    * @param __timeout      Maximum waiting time for each message, the function
    *                       returns without waiting for the time-out period to
    *                       elapse if the receiving thread appears to not
    *                       respond or "hangs".
    * 
    *                       Calls isHanging() to determine whether the window
    *                       is hanging.
    * 
    * @param __linebreadKey If true, call send(Key_Return) when __text[i] is
    *                       '\n' and ignore '\r'.
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
    * @warning The current window must be created by the current thread.
    */
    Message waitMsg() const noexcept;
    Message waitMsg(Message::msg_type __msg) const noexcept;

    static Message waitCurrentThreadMsg() noexcept;
    static Message waitCurrentThreadMsg(Message::msg_type __msg) noexcept;

    /**
    * @brief Sends a WM_CLEAR (0x0303) message.
    */
    void sendClearMsg(Timeout __timeout = DefaultTimeout) const noexcept;
    
    /**
    * @brief Sends a WM_COPY (0x0301) message.
    */
    void sendCopyMsg(Timeout __timeout = DefaultTimeout) const noexcept;
    
    /**
    * @brief Sends a WM_CUT (0x0300) message.
    */
    void sendCutMsg(Timeout __timeout = DefaultTimeout) const noexcept;
    
    /**
    * @brief Sends a WM_PASTE (0x0302) message.
    */
    void sendPasteMsg(Timeout __timeout = DefaultTimeout) const noexcept;
    
    /**
    * @brief Sends a WM_UNDO (0x0304) message.
    */
    void sendUndoMsg(Timeout __timeout = DefaultTimeout) const noexcept;


    /**
    * @brief Registers a global shortcut, when the shortcut key is pressed, a
    *        WM_HOTKEY (0x0312) message [with wParam = __shortcut.getId()] will
    *        be sent to the current window.
    */
    void setShortcut(
        Shortcut __shortcut,
        bool __enable = true) const noexcept;

    /* ================== painter ================== */

    [[nodiscard]]
    std::unique_ptr<Painter> painter() const noexcept;

    [[nodiscard]]
    static Painter* screenPainter() noexcept;

    /* ================== player ================== */

    enum SystemSoundFlag : std::uint32_t
    {
        DefaultSound     = 0x00000000,
        ErrorSound       = 0x00000010,
        QuestionSound    = 0x00000020,
        WarningSound     = 0x00000030,
        InformationSound = 0x00000040,
        BeepSound        = 0xFFFFFFFF
    };

    /**
     * @brief Plays a system-defined waveform sound.
     */
    static void play(SystemSoundFlag __flag) noexcept;

    /* ================== new windows ================== */

    enum MessageLeval : std::int32_t
    {
        Information,  // (Ok)
        Question,     // (Yes), No, Cancel
        Warning,      // Cancel, (Try Again), Continue
        Error         // Abort, (Retry), Ignore
    };

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