/**
* Win.cpp In the openWin (https://github.com/huanhuanonly/openWin)
*
* Created by Yang Huanhuan on November 25, 2024, 11:15:05
*
* Email -> 3347484963@qq.com
*
* --- This file is a part of openWin ---
*
* @brief Implement Win.h
*/

#include "../include/Win.h"

#include <Windows.h>
#include <Psapi.h>

#undef min
#undef max

#define _Win_Begin_ \
    ::ErrorStream* const _L_currentErrorStream = this->_M_errorStream; \
    ::ErrorStreamGuard _L_errorStreamGuard(*_L_currentErrorStream, __func__);

#define _Win_Static_Begin_ \
    ::ErrorStream* const _L_currentErrorStream = ::ErrorStream::global(); \
    ::ErrorStreamGuard _L_errorStreamGuard(*ErrorStream::global(), __func__);

#define _Win_Check_ \
    do { if (_L_currentErrorStream->check() == false) { return; } } while (false);

#define _Win_Check_with_(...) \
    do { if (_L_currentErrorStream->check() == false) { return __VA_ARGS__; } } while (false);

#define _Win_Check_Noreturn_ \
    _L_currentErrorStream->check();

#define _Win_Failed_ \
    do { _L_currentErrorStream->setFail(); return; } while (false);

#define _Win_Failed_with_(...) \
    do { _L_currentErrorStream->setFail(); return __VA_ARGS__; } while (false);

#define _Win_Failed_Noreturn_ \
    _L_currentErrorStream->setFail();

#define _Win_Test_(ret, ...) \
    do { if ((ret) == 0) { _Win_Failed_with_(__VA_ARGS__); } } while (false);

#define _Win_Return_on_failed_ \
    do { if (_L_currentErrorStream->failed()) { return; } } while (false);

#define _Win_Return_on_failed_with_(...) \
    do { if (_L_currentErrorStream->failed()) { return __VA_ARGS__; } } while (false);


inline HWND $(Win::Handle __handle) noexcept
{ return reinterpret_cast<HWND>(__handle); }


Win::Win() noexcept
    : _M_handle(nullptr)
    , _M_errorStream(new ErrorStream)
{ }

Win::Win(Win::Handle __handle) noexcept
    : _M_handle(__handle)
    , _M_errorStream(new ErrorStream)
{ }

Win& Win::operator=(Win::Handle __handle) noexcept
{
    _M_handle = __handle;
    return *this;
}

Win::Win(const Win& __win) noexcept
    : Win(__win._M_handle)
{ }

Win& Win::operator=(const Win& __win) noexcept
{
    _M_handle = __win._M_handle;
    return *this;
}

Win::Win(Win&& __win) noexcept
    : _M_handle(__win._M_handle)
    , _M_errorStream(__win._M_errorStream)
{
    __win._M_errorStream = nullptr;
}

Win& Win::operator=(Win&& __win) noexcept
{
    _M_handle = __win._M_handle;
    std::swap(_M_errorStream, __win._M_errorStream);
    return *this;
}

Win::~Win() noexcept
{
    if (_M_errorStream)
    {
        delete _M_errorStream;
    }
}

void Win::setHandle(Win::Handle __handle) noexcept
{
    _M_handle = __handle;
}

Win::Handle Win::handle() const noexcept
{
    return _M_handle;
}

void Win::setErrorStream(ErrorStream& __es)
{
    *_M_errorStream = __es;
}

ErrorStream& Win::errorStream()
{
    return *_M_errorStream;
}

const ErrorStream& Win::errorStream() const
{
    return *_M_errorStream;
}

bool Win::success() const noexcept
{
    return _M_errorStream->success();
}

bool Win::failed() const noexcept
{
    return _M_errorStream->failed();
}

Win Win::fromPoint(const Win::Point& __point) noexcept
{
    _Win_Static_Begin_

    return Win(WindowFromPoint({
        static_cast<decltype(POINT::x)>(__point.x()),
        static_cast<decltype(POINT::y)>(__point.y()) }));
}

Win Win::fromTitle(const Win::String& __title) noexcept
{
    _Win_Static_Begin_
    return Win(FindWindowA(nullptr, __title.c_str()));
}

Win Win::fromTitle(const Win::WString& __title) noexcept
{
    _Win_Static_Begin_
    return Win(FindWindowW(nullptr, __title.c_str()));
}

Win Win::fromClassName(const Win::String& __className) noexcept
{
    _Win_Static_Begin_
    return Win(FindWindowA(__className.c_str(), nullptr));
}

Win Win::fromClassName(const Win::WString& __className) noexcept
{
    _Win_Static_Begin_
    return Win(FindWindowW(__className.c_str(), nullptr));
}

Win Win::fromTitleAndClassName(const Win::String& __title, const Win::String& __className) noexcept
{
    _Win_Static_Begin_
    return Win(FindWindowA(__className.c_str(), __title.c_str()));
}

Win Win::fromTitleAndClassName(const Win::WString& __title, const Win::WString& __className) noexcept
{
    _Win_Static_Begin_
    return Win(FindWindowW(__className.c_str(), __title.c_str()));
}

Win Win::fromFocus() noexcept
{
    _Win_Static_Begin_
    return Win(GetFocus());
}

Win Win::fromForeground() noexcept
{
    _Win_Static_Begin_
    return Win(GetForegroundWindow());
}

Win Win::fromConsole() noexcept
{
    _Win_Static_Begin_
    return Win(GetConsoleWindow());
}

Win Win::fromShell() noexcept
{
    _Win_Static_Begin_
    return Win(GetShellWindow());
}

Win Win::fromDesktop() noexcept
{
    _Win_Static_Begin_
    return Win(GetDesktopWindow());
}

Win Win::fromActiveInCurrentThread() noexcept
{
    _Win_Static_Begin_
    return Win(GetActiveWindow());
}

Win Win::fromCaptureInCurrentThread() noexcept
{
    _Win_Static_Begin_
    return Win(GetCapture());
}


void Win::swap(Win& __other) & noexcept
{
    std::swap(_M_handle, __other._M_handle);
    std::swap(_M_errorStream, __other._M_errorStream);
}

void Win::swap(Win& __other) && noexcept
{
    __other._M_handle = _M_handle;
    std::swap(_M_errorStream, __other._M_errorStream);
}

void Win::swap(Win&& __other) noexcept
{
    _M_handle = __other._M_handle;
    std::swap(_M_errorStream, __other._M_errorStream);
}

int Win::compare(const Win& __other) const noexcept
{
    return compare(*this, __other);
}

int Win::compare(const Win& __first, const Win& __second) noexcept
{
    if (__first._M_handle > __second._M_handle)
        return 1;
    else if (__first._M_handle < __second._M_handle)
        return -1;
    else
        return 0;
}

bool Win::isValid() const noexcept
{
    _Win_Begin_
    return isValid(_M_handle);
}

bool Win::isValid(Win::Handle __handle) noexcept
{
    _Win_Static_Begin_
    return IsWindow($(__handle));
}

Win::operator bool() const noexcept
{
    return isValid();
}

bool Win::isEmpty() const noexcept
{
    return _M_handle == nullptr;
}

bool Win::isEmpty(Handle __handle) noexcept
{
    return __handle == nullptr;
}

bool Win::isNotResponding() const noexcept
{
    _Win_Begin_
    return IsHungAppWindow($(_M_handle));
}

void Win::setEnable(bool __enable) const noexcept
{
    _Win_Begin_
    EnableWindow($(_M_handle), __enable);
}

bool Win::isEnabled() const noexcept
{
    _Win_Begin_
    return static_cast<bool>(IsWindowEnabled($(_M_handle)));
}

void Win::setActive() const noexcept
{
    _Win_Begin_
    SetActiveWindow($(_M_handle));
}

bool Win::isActived() const noexcept
{
    _Win_Begin_
    return *this == fromActiveInCurrentThread();
}

void Win::setNoActive(bool __enable) const noexcept
{
    if (__enable)
        _M_addExtendStyle(WS_EX_NOACTIVATE);
    else
        _M_delExtendStyle(WS_EX_NOACTIVATE);
}

bool Win::isNoActived() const noexcept
{
    return _M_hasExtendStyle(WS_EX_NOACTIVATE);
}

void Win::setForeground(bool __lock) const noexcept
{
    _Win_Begin_
    if (SetForegroundWindow($(_M_handle)) == false)
    {
        _Win_Failed_
        return;
    }

    if (__lock)
    {
        lockSetForeground();
    }
}

bool Win::isForeground() const noexcept
{
    _Win_Begin_
    return *this == fromForeground();
}

void Win::lockSetForeground() noexcept
{
    _Win_Static_Begin_
    LockSetForegroundWindow(LSFW_LOCK);
}

void Win::unlockSetForeground() noexcept
{
    _Win_Static_Begin_
    LockSetForegroundWindow(LSFW_UNLOCK);
}

void Win::setFocus() const noexcept
{
    _Win_Begin_
    SetFocus($(_M_handle));
}

bool Win::hasFocus() const noexcept
{
    _Win_Begin_
    return *this == fromFocus();
}

void Win::setCapture(bool __enable) const noexcept
{
    _Win_Begin_
    if (__enable)
        SetCapture($(_M_handle));
    else
        ReleaseCapture();
}

Win::CharSetCode Win::charSetCode() const noexcept
{
    _Win_Begin_
    return IsWindowUnicode($(_M_handle)) ? Unicode : ANSI;
}

float Win::dpi() const noexcept
{
    _Win_Begin_
    auto dpi = GetDpiForWindow($(_M_handle));

    _Win_Test_(dpi, 1.00F)

    return static_cast<float>(dpi >= 96 ? dpi + ((dpi - 96) / 24) + 4 : 100) / 100;
}

void Win::setParent(const Win& __win) const noexcept
{
    _Win_Begin_
    SetParent($(_M_handle), $(__win._M_handle));
}

void Win::setParent(std::nullptr_t) const noexcept
{
    _Win_Begin_
    SetParent($(_M_handle), nullptr);
}

Win Win::parent(bool __onlyParent) const noexcept
{
    _Win_Begin_
    HWND fa = __onlyParent ? GetAncestor($(_M_handle), GA_PARENT) : GetParent($(_M_handle));
    return Win(fa ? fa : _M_handle);
}

bool Win::hasParent() const noexcept
{
    _Win_Begin_
    return static_cast<bool>(GetAncestor($(_M_handle), GA_PARENT));
}

Win::WinList Win::children() const noexcept
{
    _Win_Begin_
    Win::WinList buffers;

    EnumChildWindows(
        $(_M_handle),
        static_cast<WNDENUMPROC>(
            [](HWND hWnd, LPARAM lParam) -> BOOL
            {
                reinterpret_cast<Win::WinList*>(lParam)->push_back(Win(hWnd));
                return true;
            }),
        reinterpret_cast<LPARAM>(&buffers));

    return buffers;
}

bool Win::hasChild() const noexcept
{
    _Win_Begin_
    bool has = false;

    EnumChildWindows(
        $(_M_handle),
        static_cast<WNDENUMPROC>(
            [](HWND, LPARAM lParam) -> BOOL
            {
                *reinterpret_cast<bool*>(lParam) = true;
                return false;
            }),
        reinterpret_cast<LPARAM>(&has));
    
    return has;
}

Win Win::topChild() const noexcept
{
    _Win_Begin_
    return Win(GetTopWindow($(_M_handle)));
}

bool Win::isParent() const noexcept
{
    return isChild() == false;
}

bool Win::isChild() const noexcept
{
    _Win_Begin_
    return (GetWindowLong($(_M_handle), GWL_STYLE) & WS_CHILD) == WS_CHILD;
}

bool Win::isChildOf(const Win& __parent) const noexcept
{
    _Win_Begin_
    return IsChild($(__parent._M_handle), $(_M_handle));
}

int Win::setTile(Win::Orientation __orientation) const noexcept
{
    _Win_Begin_
    return static_cast<int>(
        TileWindows(
            $(_M_handle),
            __orientation == Win::Vertical ? MDITILE_VERTICAL : MDITILE_HORIZONTAL,
            nullptr,
            0,
            nullptr));
}

int Win::setTile(Win::Orientation __orientation, const Win::Rect& __clientRect) const noexcept
{
    _Win_Begin_
    RECT r;

    r.left = __clientRect.x();
    r.right = __clientRect.y();
    r.bottom = __clientRect.x() + __clientRect.width();
    r.top = __clientRect.y() + __clientRect.height();
    
    return static_cast<int>(
        TileWindows(
            $(_M_handle),
            __orientation == Win::Vertical ? MDITILE_VERTICAL : MDITILE_HORIZONTAL,
            &r,
            0,
            nullptr));
}

Win::WinList Win::list() noexcept
{
    _Win_Static_Begin_
    Win::WinList buffers;

    EnumWindows(
        static_cast<WNDENUMPROC>(
            [](HWND hWnd, LPARAM lParam) -> BOOL
            {
                reinterpret_cast<Win::WinList*>(lParam)->push_back(Win(hWnd));
                return true;
            }),
        reinterpret_cast<LPARAM>(&buffers));

    return buffers;
}

Win::WinList Win::listInSameThread() const noexcept
{
    _Win_Begin_
    Win::WinList buffers;

    EnumThreadWindows(
        threadId(),
        static_cast<WNDENUMPROC>(
            [](HWND hWnd, LPARAM lParam) -> BOOL
            {
                reinterpret_cast<Win::WinList*>(lParam)->push_back(Win(hWnd));
                return true;
            }),
        reinterpret_cast<LPARAM>(&buffers));

    return buffers;
}

void Win::forward() const noexcept
{
    _Win_Begin_
    SetWindowPos(
        $(_M_handle),
        HWND_TOP,
        0, 0, 0, 0,
        SWP_NOMOVE | SWP_NOSIZE);
}

void Win::backward() const noexcept
{
    _Win_Begin_
    SetWindowPos(
        $(_M_handle),
        HWND_BOTTOM,
        0, 0, 0, 0,
        SWP_NOMOVE | SWP_NOSIZE);
}

void Win::setTopmost(bool __enable) const noexcept
{
    _Win_Begin_
    SetWindowPos(
        $(_M_handle),
        __enable ? HWND_TOPMOST : HWND_NOTOPMOST,
        0, 0, 0, 0,
        SWP_NOMOVE | SWP_NOSIZE);
}

bool Win::isTopmost() const noexcept
{
    _Win_Begin_
    return (GetWindowLong($(_M_handle), GWL_EXSTYLE) & WS_EX_TOPMOST) == WS_EX_TOPMOST;
}

Win Win::firstLayer() const noexcept
{
    _Win_Begin_
    return Win(GetWindow($(_M_handle), GW_HWNDFIRST));
}

Win Win::lastLayer() const noexcept
{
    _Win_Begin_
    return Win(GetWindow($(_M_handle), GW_HWNDLAST));
}

Win Win::nextLayer() const noexcept
{
    _Win_Begin_
    return Win(GetWindow($(_M_handle), GW_HWNDNEXT));
}

Win Win::prevLayer() const noexcept
{
    _Win_Begin_
    return Win(GetWindow($(_M_handle), GW_HWNDPREV));
}

Win Win::owner() const noexcept
{
    _Win_Begin_
    return Win(GetWindow($(_M_handle), GW_OWNER));
}

Win Win::root() const noexcept
{
    _Win_Begin_
    return Win(GetAncestor($(_M_handle), GA_ROOT));
}

void Win::becomePopup(bool __enable) const noexcept
{
    if (__enable)
        _M_addStyle(WS_POPUP);
    else
        _M_delStyle(WS_POPUP);
}

bool Win::isPopup() const noexcept
{
    return _M_hasStyle(WS_POPUP);
}

void Win::becomeTool(bool __enable) const noexcept
{
    if (__enable)
        _M_addExtendStyle(WS_EX_TOOLWINDOW);
    else
        _M_delExtendStyle(WS_EX_TOOLWINDOW);
}

bool Win::isTool() const noexcept
{
    return _M_hasExtendStyle(WS_EX_TOOLWINDOW);
}

bool Win::isMdiChild() const noexcept
{
    return _M_hasExtendStyle(WS_EX_MDICHILD);
}

void Win::show() const noexcept
{
    _Win_Begin_
    ShowWindow($(_M_handle), SW_SHOW);
}

void Win::hide() const noexcept
{
    _Win_Begin_
    ShowWindow($(_M_handle), SW_HIDE);
}

bool Win::isVisible() const noexcept
{
    _Win_Begin_
    return static_cast<bool>(IsWindowVisible($(_M_handle)));
}

void Win::showPopups() const noexcept
{
    _Win_Begin_
    ShowOwnedPopups($(_M_handle), true);
}

void Win::hidePopups() const noexcept
{
    _Win_Begin_
    ShowOwnedPopups($(_M_handle), false);
}

void Win::maximize() const noexcept
{
    _Win_Begin_
    ShowWindow($(_M_handle), SW_MAXIMIZE);
}

void Win::minimize() const noexcept
{
    _Win_Begin_
    ShowWindow($(_M_handle), SW_MINIMIZE);
}

void Win::restore() const noexcept
{
    _Win_Begin_
    ShowWindow($(_M_handle), SW_RESTORE);
}

bool Win::isMaximized() const noexcept
{
    _Win_Begin_
    return static_cast<bool>(IsZoomed($(_M_handle)));
}

bool Win::isMinimized() const noexcept
{
    _Win_Begin_
    return static_cast<bool>(IsIconic($(_M_handle)));
}

bool Win::isRestored() const noexcept
{
    if (!isMaximized() && !isMinimized() && !isArranged())
        return true;
    else
        return false;
}

bool Win::isArranged() const noexcept
{
    _Win_Begin_
    HMODULE hModule = LoadLibraryA("User32.dll");

    if (hModule == nullptr)
    {
        _Win_Failed_with_(false)
    }

    BOOL (WINAPI *isWindowArranged)(HWND)
        = reinterpret_cast<decltype(isWindowArranged)>(
            GetProcAddress(hModule, "isWindowArranged"));

    _Win_Check_with_(false)

    bool result = static_cast<bool>((*isWindowArranged)($(_M_handle)));

    FreeLibrary(hModule);
    
    return result;
}

void Win::setTitle(const Win::String& __title) const noexcept
{
    _Win_Begin_
    SetWindowTextA($(_M_handle), __title.c_str());
}

void Win::setTitle(const Win::WString& __title) const noexcept
{
    _Win_Begin_
    SetWindowTextW($(_M_handle), __title.c_str());
}

Win::String Win::title() const noexcept
{
    _Win_Begin_

    Win::String buffer(
        static_cast<std::size_t>(GetWindowTextLengthA($(_M_handle)) + 1),
        '\0');

    _Win_Check_with_(Win::String())
    
    GetWindowTextA(
        $(_M_handle),
        const_cast<Win::String::value_type*>(buffer.data()),
        static_cast<int>(buffer.size()));

    buffer.resize(buffer.size() - 1);
    return buffer;
}

Win::WString Win::WIN_FW(title)() const noexcept
{
    _Win_Begin_

    Win::WString buffer(
        static_cast<std::size_t>(GetWindowTextLengthW($(_M_handle)) + 1),
        '\0');

    _Win_Check_with_(Win::WString())

    GetWindowTextW(
        $(_M_handle),
        const_cast<Win::WString::value_type*>(buffer.data()),
        static_cast<int>(buffer.size()));

    buffer.resize(buffer.size() - 1);
    return buffer;
}

Win::String Win::className() const noexcept
{
    _Win_Begin_

    Win::String buffer(1 << 8, '\0');

    unsigned int len = RealGetWindowClassA(
        $(_M_handle),
        const_cast<Win::String::value_type*>(buffer.data()),
        static_cast<unsigned int>(buffer.size()));

    buffer.resize(len);
    return buffer;
}

Win::WString Win::WIN_FW(className)() const noexcept
{
    _Win_Begin_

    Win::WString buffer(1 << 8, '\0');

    unsigned int len = RealGetWindowClassW(
        $(_M_handle),
        const_cast<Win::WString::value_type*>(buffer.data()),
        static_cast<unsigned int>(buffer.size()));

    buffer.resize(len);
    return buffer;
}

void Win::setRect(const Rect& __rect) const noexcept
{
    _Win_Begin_

    Win::Size sz(__rect.size().physics(dpi()));

    SetWindowPos(
        $(_M_handle),
        0,
        __rect.x(),
        __rect.y(),
        sz.w(),
        sz.h(),
        SWP_NOZORDER);
}

Win::Rect Win::rect() const noexcept
{
    _Win_Begin_

    RECT buffer;
    
    GetWindowRect($(_M_handle), &buffer);

    return Win::Rect(
        buffer.left,
        buffer.top,
        buffer.right - buffer.left,
        buffer.bottom - buffer.top).mapto(dpi());
}

Win::Rect Win::clientRect() const noexcept
{
    _Win_Begin_

    RECT buffer;

    GetClientRect($(_M_handle), &buffer);

    return Win::Rect(
        buffer.left,
        buffer.top,
        buffer.right - buffer.left,
        buffer.bottom - buffer.top).mapto(dpi());
}

void Win::setPos(const Point& __point, Win::PgPoint* __pg) const noexcept
{
    _Win_Begin_

    __pg->build(pos(), __point);

    float _dpi = dpi();

    for (__pg->next(); !__pg->atEnd(); __pg->next())
    {
        Win::Point cur(__pg->get().physics(_dpi));

        bool ret = SetWindowPos(
            $(_M_handle),
            0,
            cur.x(),
            cur.y(),
            0,
            0,
            SWP_NOZORDER | SWP_NOSIZE);

        _Win_Test_(ret);
    }
}

void Win::setPos(Win::PosFlag __flag, int __reserve, Win::PgPoint* __pg) const noexcept
{
    switch (__flag)
    {
    case TopLeftCorner:
        setPos(__reserve, __reserve, __pg);
        break;
        
    case TopRightCorner:
        setPos(screenWidth() - width() - __reserve, __reserve, __pg);
        break;

    case Center:
        setPos((screenWidth() - width()) / 2, (screenHeight() - height()) / 2, __pg);
        break;

    case BottomLeftCorner:
        setPos(__reserve, screenHeight() - height() - __reserve, __pg);
        break;

    case BottomRightCorner:
        setPos(screenWidth() - width() - __reserve, screenHeight() - height() - __reserve, __pg);
        break;
    }
}

Win::Point Win::pos() const noexcept
{
    return rect().point();
}

void Win::move(int __addX, int __addY, Win::PgPoint* __pg) const noexcept
{
    Win::Point p(pos());

    setPos(p.x() + __addX, p.y() + __addY, __pg);
}

void Win::setSize(const Size& __size, Win::PgSize* __pg) const noexcept
{
    _Win_Begin_

    __pg->build(size(), __size);

    auto _dpi = dpi();

    for (__pg->next(); !__pg->atEnd(); __pg->next())
    {
        Win::Size cur(__pg->get().physics(_dpi));

        bool ret = SetWindowPos(
            $(_M_handle),
            0,
            0,
            0,
            cur.width(),
            cur.height(),
            SWP_NOZORDER | SWP_NOMOVE);

        _Win_Test_(ret);
    }
}

Win::Size Win::size() const noexcept
{
    return rect().size();
}

Win::Size Win::screenSize() noexcept
{
    _Win_Static_Begin_

    HDC hDc = GetDC(nullptr);

    _Win_Test_(hDc, Win::Size())

    int w = GetDeviceCaps(hDc, DESKTOPHORZRES);
    int h = GetDeviceCaps(hDc, DESKTOPVERTRES);

    ReleaseDC(nullptr, hDc);

    return Win::Size(w, h);
}

void Win::setWidth(int __width, Win::PgInt* __pg) const noexcept
{
    _Win_Begin_

    Win::Size sz(size());

    __pg->build(sz.width(), __width);

    auto _dpi = dpi();

    for (__pg->next(); !__pg->atEnd(); __pg->next())
    {
        bool ret = SetWindowPos(
            $(_M_handle),
            0,
            0,
            0,
            static_cast<int>(__pg->get() / _dpi),
            sz.height(),
            SWP_NOZORDER | SWP_NOMOVE);

        _Win_Test_(ret)
    }
}

void Win::setHeight(int __height, Win::PgInt* __pg) const noexcept
{
    _Win_Begin_

    Win::Size sz(size());

    __pg->build(sz.height(), __height);

    auto _dpi = dpi();

    for (__pg->next(); !__pg->atEnd(); __pg->next())
    {
        bool ret = SetWindowPos(
            $(_M_handle),
            0,
            0,
            0,
            sz.width(),
            static_cast<int>(__pg->get() / _dpi),
            SWP_NOZORDER | SWP_NOMOVE);

        _Win_Test_(ret)
    }
}

int Win::width() const noexcept
{
    return size().width();
}

int Win::height() const noexcept
{
    return size().height();
}

int Win::screenWidth() noexcept
{
    _Win_Static_Begin_

    HDC hDc = GetDC(nullptr);

    _Win_Test_(hDc, 0)

    int w = GetDeviceCaps(hDc, DESKTOPHORZRES);

    ReleaseDC(nullptr, hDc);

    return w;
}

int Win::screenHeight() noexcept
{
    _Win_Static_Begin_

    HDC hDc = GetDC(nullptr);

    _Win_Test_(hDc, 0)

    int h = GetDeviceCaps(hDc, DESKTOPVERTRES);

    ReleaseDC(nullptr, hDc);

    return h;
}

void Win::setZoom(int __addWidth, int __addHeight, Win::PgSize* __pg) const noexcept
{
    Win::Size sz(size());

    setSize(sz.w() + __addWidth, sz.h() + __addHeight, __pg);
}

void Win::setZoom(double __scaleX, double __scaleY, Win::PgSize* __pg) const noexcept
{
    Win::Size sz(size());

    setSize(
        static_cast<int>(static_cast<double>(sz.w()) * __scaleX),
        static_cast<int>(static_cast<double>(sz.h()) * __scaleY), __pg);
}

void Win::setOpacity(int __value, Win::PgInt* __pg) const noexcept
{
    _Win_Begin_

    auto style = GetWindowLong($(_M_handle), GWL_EXSTYLE);
    _Win_Check_

    __pg->build(opacity(), __value);

    if ((style & WS_EX_LAYERED) != WS_EX_LAYERED)
    {
        SetWindowLong($(_M_handle), GWL_EXSTYLE, style | WS_EX_LAYERED);
        _Win_Check_
    }

    for (__pg->next(); !__pg->atEnd(); __pg->next())
    {
        bool ret = SetLayeredWindowAttributes(
            $(_M_handle),
            0,
            static_cast<BYTE>(std::max(0, std::min(0xff, __pg->get()))),
            LWA_ALPHA);

        _Win_Test_(ret)
    }
}

int Win::opacity() const noexcept
{
    _Win_Begin_

    BYTE buffer = 0xff;

    auto style = GetWindowLong($(_M_handle), GWL_EXSTYLE);
    _Win_Check_with_(buffer)

    if ((style & WS_EX_LAYERED) == WS_EX_LAYERED)
    {
        GetLayeredWindowAttributes($(_M_handle), nullptr, &buffer, nullptr);
    }
    return buffer;
}

void Win::setTransparencyColor(const Win::Color& __color) const noexcept
{
    _Win_Begin_

    static_assert(sizeof(Win::Color) == sizeof(COLORREF), "sizeof(Win::Color) != sizeof(COLORREF)");

    auto style = GetWindowLong($(_M_handle), GWL_EXSTYLE);
    _Win_Check_

    if ((style & WS_EX_LAYERED) != WS_EX_LAYERED)
    {
        SetWindowLong($(_M_handle), GWL_EXSTYLE, style | WS_EX_LAYERED);
        _Win_Check_
    }

    SetLayeredWindowAttributes(
        $(_M_handle),
        __color._M_data,
        0,
        LWA_COLORKEY);
}

Win::Color Win::transparencyColor() const noexcept
{
    _Win_Begin_

    static_assert(sizeof(Win::Color) == sizeof(COLORREF), "sizeof(Win::Color) != sizeof(COLORREF)");

    Win::Color buffer{};

    auto style = GetWindowLong($(_M_handle), GWL_EXSTYLE);
    _Win_Check_with_(buffer)

    if ((style & WS_EX_LAYERED) == WS_EX_LAYERED)
    {
        GetLayeredWindowAttributes(
            $(_M_handle),
            reinterpret_cast<COLORREF*>(&buffer._M_data),
            nullptr,
            nullptr);
    }
    return buffer;
}


void Win::_M_addStyle(std::int32_t __style) const noexcept
{
    _Win_Begin_

    SetWindowLong(
        $(_M_handle),
        GWL_STYLE,
        GetWindowLong(
            $(_M_handle),
            GWL_STYLE) | __style);
}

void Win::_M_delStyle(std::int32_t __style) const noexcept
{
    _Win_Begin_

    SetWindowLong(
        $(_M_handle),
        GWL_STYLE,
        GetWindowLong(
            $(_M_handle),
            GWL_STYLE) & ~__style);
}

bool Win::_M_hasStyle(std::int32_t __style) const noexcept
{
    _Win_Begin_

    return (GetWindowLong($(_M_handle), GWL_STYLE) & __style) == __style;
}

void Win::_M_addExtendStyle(std::int32_t __style) const noexcept
{
    _Win_Begin_

    SetWindowLong(
        $(_M_handle),
        GWL_EXSTYLE,
        GetWindowLong(
            $(_M_handle),
            GWL_EXSTYLE) | __style);
}

void Win::_M_delExtendStyle(std::int32_t __style) const noexcept
{
    _Win_Begin_

    SetWindowLong(
        $(_M_handle),
        GWL_EXSTYLE,
        GetWindowLong(
            $(_M_handle),
            GWL_EXSTYLE) & ~__style);
}

bool Win::_M_hasExtendStyle(std::int32_t __style) const noexcept
{
    _Win_Begin_

    return (GetWindowLong($(_M_handle), GWL_EXSTYLE) & __style) == __style;
}

void Win::setBorder(bool __enable) const noexcept
{
    if (__enable)
        _M_addStyle(WS_BORDER);
    else
        _M_delStyle(WS_BORDER);
}

bool Win::hasBorder() const noexcept
{
    return _M_hasStyle(WS_BORDER);
}

void Win::setSunkenEdge(bool __enable) const noexcept
{
    if (__enable)
        _M_addExtendStyle(WS_EX_CLIENTEDGE);
    else
        _M_delExtendStyle(WS_EX_CLIENTEDGE);
}

bool Win::hasSunkenEdge() const noexcept
{
    return _M_hasExtendStyle(WS_EX_CLIENTEDGE);
}

void Win::setRaisedEdge(bool __enable) const noexcept
{
    if (__enable)
        _M_addExtendStyle(WS_EX_WINDOWEDGE);
    else
        _M_delExtendStyle(WS_EX_WINDOWEDGE);
}

bool Win::hasRaisedEdge() const noexcept
{
    return _M_hasExtendStyle(WS_EX_WINDOWEDGE);
}

void Win::setStaticEdge(bool __enable) const noexcept
{
    if (__enable)
        _M_addExtendStyle(WS_EX_STATICEDGE);
    else
        _M_delExtendStyle(WS_EX_STATICEDGE);
}

bool Win::hasStaticEdge() const noexcept
{
    return _M_hasExtendStyle(WS_EX_STATICEDGE);
}

void Win::setTitlebar(bool __enable) const noexcept
{
    if (__enable)
        _M_addStyle(WS_CAPTION);
    else
        _M_delStyle(WS_CAPTION);
}

bool Win::hasTitlebar() const noexcept
{
    return _M_hasStyle(WS_CAPTION);
}

void Win::setMenubar(bool __enable) const noexcept
{
    if (__enable)
        _M_addStyle(WS_SYSMENU | WS_CAPTION);
    else
        _M_delStyle(WS_SYSMENU);
}

bool Win::hasMenubar() const noexcept
{
    return _M_hasStyle(WS_SYSMENU);
}

void Win::setScroll(Win::Orientation __orientation, bool __enable) const noexcept
{
    std::int32_t style;

    if (__orientation == Win::Vertical)
        style = WS_VSCROLL;
    else if (__orientation == Win::Horizontal)
        style = WS_HSCROLL;
    else
        style = WS_VSCROLL | WS_HSCROLL;

    if (__enable)
        _M_addStyle(style);
    else
        _M_delStyle(style);
}

bool Win::hasScroll(Win::Orientation __orientation) const noexcept
{
    std::int32_t style;

    if (__orientation == Win::Vertical)
        style = WS_VSCROLL;
    else if (__orientation == Win::Horizontal)
        style = WS_HSCROLL;
    else
        style = WS_VSCROLL | WS_HSCROLL;

    return _M_hasStyle(style);
}

void Win::setSizebox(bool __enable) const noexcept
{
    if (__enable)
        _M_addStyle(WS_SIZEBOX);
    else
        _M_delStyle(WS_SIZEBOX);
}

bool Win::hasSizebox() const noexcept
{
    return _M_hasStyle(WS_SIZEBOX);
}

void Win::setTitlebarButtons(
    Win::TitlebarButtons __titlebarButtons,
    bool __enable) const noexcept
{
    _Win_Begin_

    if (__enable)
    {
        setMenubar(true);
    }

    std::int32_t style = 0;

    if (__titlebarButtons & Win::MaximizeButton)
    {
        style |= WS_MAXIMIZEBOX;
    }

    if (__titlebarButtons & Win::MinimizeButton)
    {
        style |= WS_MINIMIZEBOX;
    }

    if (style)
    {
        if (__enable)
            _M_addStyle(style);
        else
            _M_delStyle(style);
    }

    if (__titlebarButtons & Win::HelpButton)
    {
        if (__enable)
            _M_addExtendStyle(WS_EX_CONTEXTHELP);
        else
            _M_delExtendStyle(WS_EX_CONTEXTHELP);
    }

    if (__titlebarButtons & Win::CloseButton)
    {
        if (__enable)
        {
            EnableMenuItem(
                GetSystemMenu($(_M_handle), false),
                SC_CLOSE,
                MF_ENABLED);
        }
        else
        {
            EnableMenuItem(
                GetSystemMenu($(_M_handle), false),
                SC_CLOSE,
                MF_DISABLED);
        }

        DrawMenuBar($(_M_handle));
    }
}

bool Win::hasTitlebarButtons(
    Win::TitlebarButtons __titlebarButtons) const noexcept
{
    _Win_Begin_

    bool result = true;

    if (__titlebarButtons & Win::MaximizeButton)
    {
        result &= _M_hasStyle(WS_MAXIMIZEBOX);
    }

    if (__titlebarButtons & Win::MinimizeButton)
    {
        result &= _M_hasStyle(WS_MINIMIZEBOX);
    }

    if (__titlebarButtons & Win::HelpButton)
    {
        result &= _M_hasExtendStyle(WS_EX_CONTEXTHELP);
    }

    if (__titlebarButtons & Win::CloseButton)
    {
        HMENU hMenu = GetSystemMenu($(_M_handle), false);

        int lastState = EnableMenuItem(
            hMenu,
            SC_CLOSE,
            0);

        result &= (lastState == MF_ENABLED);

        EnableMenuItem(
            hMenu,
            SC_CLOSE,
            lastState);
    }

    return result;
}

void Win::setTaskbarIcon(bool __enable) const noexcept
{
    if (__enable)
        _M_addExtendStyle(WS_EX_APPWINDOW);
    else
        _M_delExtendStyle(WS_EX_APPWINDOW);
}

bool Win::hasTaskbarIcon() const noexcept
{
    return _M_hasExtendStyle(WS_EX_APPWINDOW);
}

void Win::setDoubleBufferDrawing(bool __enable) const noexcept
{
    if (__enable)
        _M_addExtendStyle(WS_EX_COMPOSITED);
    else
        _M_delExtendStyle(WS_EX_COMPOSITED);
}

bool Win::isDoubleBufferDrawing() const noexcept
{
    return _M_hasExtendStyle(WS_EX_COMPOSITED);
}

void Win::setAcceptFiles(bool __enable) const noexcept
{
    if (__enable)
        DragAcceptFiles($(_M_handle), true),
        _M_addExtendStyle(WS_EX_ACCEPTFILES);
    else
        DragAcceptFiles($(_M_handle), false),
        _M_delExtendStyle(WS_EX_ACCEPTFILES);
}

bool Win::isAcceptFiles() const noexcept
{
    return _M_hasExtendStyle(WS_EX_ACCEPTFILES);
}

void Win::flash(bool __enable) const noexcept
{
    _Win_Begin_

    if (__enable)
        FlashWindow($(_M_handle), true);
    else
    {
        FLASHWINFO info {
            static_cast<decltype(info.cbSize)>(
                sizeof(decltype(info))),
            $(_M_handle),
            FLASHW_STOP,
            0,
            0
        };
        
        FlashWindowEx(&info);
    }
}

void Win::flash(int __count, Win::Timeout __timeout, bool __caption) const noexcept
{
    _Win_Begin_

    FLASHWINFO info {
            static_cast<decltype(info.cbSize)>(
                sizeof(decltype(info))),
            $(_M_handle),
            static_cast<decltype(info.dwFlags)>(
                __count > 0 ? (__caption ? FLASHW_ALL : FLASHW_TRAY) : FLASHW_TIMER),
            static_cast<decltype(info.uCount)>(std::max(__count, 0)),
            __timeout
    };

    FlashWindowEx(&info);
}

void Win::flash(Win::FlashFlag, Win::Timeout __timeout, bool __caption) const noexcept
{
    _Win_Begin_

    FLASHWINFO info {
            static_cast<decltype(info.cbSize)>(
                sizeof(decltype(info))),
            $(_M_handle),
            static_cast<decltype(info.dwFlags)>(
                (__caption ? FLASHW_ALL : FLASHW_TRAY) | FLASHW_TIMERNOFG),
            0,
            __timeout
    };

    FlashWindowEx(&info);
}

void Win::update(bool __eraseBackground) const noexcept
{
    _Win_Begin_

    bool ret = InvalidateRect($(_M_handle), nullptr, __eraseBackground);
    _Win_Test_(ret)

    ret = UpdateWindow($(_M_handle));
    _Win_Test_(ret)
}

void Win::update(bool __eraseBackground, const Win::Rect& __clientRect) const noexcept
{
    _Win_Begin_

    RECT buffer {
        __clientRect.x(),
        __clientRect.y(),
        __clientRect.x() + __clientRect.width(),
        __clientRect.y() + __clientRect.height()
    };

    bool ret = InvalidateRect($(_M_handle), &buffer, __eraseBackground);
    _Win_Test_(ret)

    ret = UpdateWindow($(_M_handle));
    _Win_Test_(ret)
}

void Win::lockUpdate() const noexcept
{
    _Win_Begin_

    bool ret = LockWindowUpdate($(_M_handle));
    _Win_Test_(ret)
}

void Win::unlockUpdate() noexcept
{
    _Win_Static_Begin_

    bool ret = LockWindowUpdate(nullptr);
    _Win_Test_(ret)
}

Win::ThreadId Win::threadId() const noexcept
{
    _Win_Begin_

    static_assert(sizeof(Win::ThreadId) == sizeof(DWORD), "sizeof(Win::ThreadId) != sizeof(DWORD)");
    
    return GetWindowThreadProcessId($(_M_handle), nullptr);
}

Win::ProcessId Win::processId() const noexcept
{
    _Win_Begin_

    static_assert(sizeof(Win::ProcessId) == sizeof(DWORD), "sizeof(Win::ProcessId) != sizeof(DWORD)");

    Win::ProcessId id = 0;
    GetWindowThreadProcessId($(_M_handle), reinterpret_cast<DWORD*>(&id));
    return id;
}

bool Win::isInCurrentThread() const noexcept
{
    _Win_Begin_
    return threadId() == GetCurrentThreadId();
}

bool Win::isInCurrentProcess() const noexcept
{
    _Win_Begin_
    return processId() == GetCurrentProcessId();
}

void Win::close() const noexcept
{
    _Win_Begin_
    CloseWindow($(_M_handle));
}

void Win::destroy() const noexcept
{
    _Win_Begin_
    DestroyWindow($(_M_handle));
}

void Win::killThread(int __exitCode) const noexcept
{
    _Win_Begin_

    HANDLE hProcess = OpenThread(THREAD_TERMINATE, false, threadId());
    _Win_Test_(hProcess)

    TerminateThread(hProcess, __exitCode);
    _Win_Check_Noreturn_

    CloseHandle(hProcess);
}

void Win::killProcess(int __exitCode) const noexcept
{
    _Win_Begin_

    HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, false, processId());
    _Win_Test_(hProcess)

    TerminateProcess(hProcess, __exitCode);
    _Win_Check_Noreturn_

    CloseHandle(hProcess);
}

Win::String Win::path() const noexcept
{
    _Win_Begin_

    Win::String::value_type buffer[MAX_PATH];

    buffer[0] = '\0';

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, processId());
    _Win_Test_(hProcess, Win::String())

    GetModuleFileNameExA(hProcess, nullptr, buffer, MAX_PATH);
    _Win_Check_Noreturn_

    CloseHandle(hProcess);

    return Win::String(buffer);
}

Win::WString Win::WIN_FW(path)() const noexcept
{
    _Win_Begin_

    Win::WString::value_type buffer[MAX_PATH];

    buffer[0] = '\0';

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, processId());
    _Win_Test_(hProcess, Win::WString())

    GetModuleFileNameExW(hProcess, nullptr, buffer, MAX_PATH);
    _Win_Check_Noreturn_

    CloseHandle(hProcess);

    return Win::WString(buffer);
}

bool Win::hasPopupInScreen() noexcept
{
    _Win_Static_Begin_

    return static_cast<bool>(AnyPopup());
}

void Win::_M_sendMessageA(
    Win::Message::msg_type __msg,
    Win::Message::wparam_type __wParam,
    Win::Message::lparam_type __lParam,
    Win::Timeout __timeout) const noexcept
{
    _Win_Begin_
    
    if (__timeout == Win::InfiniteTimeout)
    {
        SendMessageA(
            $(_M_handle),
            __msg,
            __wParam,
            __lParam);
    }
    else
    {
        auto ret = SendMessageTimeoutA(
            $(_M_handle),
            __msg,
            __wParam,
            __lParam,
            SMTO_ABORTIFHUNG | SMTO_NORMAL | SMTO_ERRORONEXIT,
            __timeout,
            nullptr);

        _Win_Test_(ret)
    }
}

void Win::_M_sendMessageW(
    Win::Message::msg_type __msg,
    Win::Message::wparam_type __wParam,
    Win::Message::lparam_type __lParam,
    Win::Timeout __timeout) const noexcept
{
    _Win_Begin_

    if (__timeout == Win::InfiniteTimeout)
    {
        SendMessageW(
            $(_M_handle),
            __msg,
            __wParam,
            __lParam);
    }
    else
    {
        auto ret = SendMessageTimeoutW(
            $(_M_handle),
            __msg,
            __wParam,
            __lParam,
            SMTO_ABORTIFHUNG | SMTO_NORMAL | SMTO_ERRORONEXIT,
            __timeout,
            nullptr);

        _Win_Test_(ret)
    }
}

void Win::send(
    const Win::String& __text,
    Win::Timeout __timeout,
    bool __linebreakKey) const noexcept
{
    _Win_Begin_

    if (__linebreakKey)
        for (int i = 0; i < static_cast<int>(__text.size()); ++i)
        {
            switch (__text[i])
            {
            case '\r':
                break;

            case '\n':
                send(Win::Key_Return, __timeout);
                break;

            default:
                send(__text[i], __timeout);
                break;
            }

            _Win_Return_on_failed_
        }
    else
        for (int i = 0; i < static_cast<int>(__text.size()); ++i)
        {
            send(__text[i], __timeout);

            _Win_Return_on_failed_
        }
}

void Win::post(const Win::String& __text, bool __linebreakKey) const noexcept
{
    _Win_Begin_

    if (__linebreakKey)
        for (int i = 0; i < static_cast<int>(__text.size()); ++i)
        {
            switch (__text[i])
            {
            case '\r':
                break;

            case '\n':
                post(Win::Key_Return);
                break;

            default:
                post(__text[i]);
                break;
            }

            _Win_Return_on_failed_
        }
    else
        for (int i = 0; i < static_cast<int>(__text.size()); ++i)
        {
            post(__text[i]);
            
            _Win_Return_on_failed_
        }
}

void Win::send(
    const Win::WString& __text,
    Win::Timeout __timeout,
    bool __linebreakKey) const noexcept
{
    _Win_Begin_

    if (__linebreakKey)
        for (int i = 0; i < static_cast<int>(__text.size()); ++i)
        {
            switch (__text[i])
            {
            case '\r':
                break;

            case '\n':
                send(Win::Key_Return, __timeout);
                break;

            default:
                send(__text[i], __timeout);
                break;
            }

            _Win_Return_on_failed_
        }
    else
        for (int i = 0; i < static_cast<int>(__text.size()); ++i)
        {
            send(__text[i], __timeout);

            _Win_Return_on_failed_
        }
}

void Win::post(const Win::WString& __text, bool __linebreakKey) const noexcept
{
    _Win_Begin_

    if (__linebreakKey)
        for (int i = 0; i < static_cast<int>(__text.size()); ++i)
        {
            switch (__text[i])
            {
            case '\r':
                break;

            case '\n':
                post(Win::Key_Return);
                break;

            default:
                post(__text[i]);
                break;
            }

            _Win_Return_on_failed_
        }
    else
        for (int i = 0; i < static_cast<int>(__text.size()); ++i)
        {
            post(__text[i]);

            _Win_Return_on_failed_
        }
}

void Win::send(char __word, Win::Timeout __timeout) const noexcept
{
    WM_CHAR_LPARAM lParam;
    lParam._uint_v = 0;
    lParam._struct_v.repeatCount = 1;

    _M_sendMessageA(WM_CHAR, __word, lParam._uint_v, __timeout);
}

void Win::post(char __word) const noexcept
{
    WM_CHAR_LPARAM lParam;
    lParam._uint_v = 0;
    lParam._struct_v.repeatCount = 1;

    PostMessageA($(_M_handle), WM_CHAR, __word, lParam._uint_v);
}

void Win::send(wchar_t __word, Win::Timeout __timeout) const noexcept
{
    WM_CHAR_LPARAM lParam;
    lParam._uint_v = 0;
    lParam._struct_v.repeatCount = 1;

    _M_sendMessageW(WM_CHAR, __word, lParam._uint_v, __timeout);
}

void Win::post(wchar_t __word) const noexcept
{
    WM_CHAR_LPARAM lParam;
    lParam._uint_v = 0;
    lParam._struct_v.repeatCount = 1;

    PostMessageW($(_M_handle), WM_CHAR, __word, lParam._uint_v);
}

void Win::send(Win::Key __key, Win::Timeout __timeout) const noexcept
{
    send(__key, OnlyPress, __timeout);
    send(__key, OnlyRelease, __timeout);
}

void Win::post(Win::Key __key) const noexcept
{
    post(__key, OnlyPress);
    post(__key, OnlyRelease);
}

void Win::send(Win::Key __key, Win::KeyAction __action, Win::Timeout __timeout) const noexcept
{
    WM_CHAR_LPARAM lParam;

    lParam._uint_v = 0;

    lParam._struct_v.repeatCount = 1;
    lParam._struct_v.scanCode = MapVirtualKeyW(__key, MAPVK_VK_TO_VSC);

    if (__action == OnlyPress)
    {
        _M_sendMessageW(
            WM_KEYDOWN,
            static_cast<WPARAM>(__key),
            static_cast<LPARAM>(lParam._uint_v),
            __timeout);
    }
    else // if (__action == OnlyRelease)
    {
        lParam._struct_v.transitionState = 1;

        _M_sendMessageW(
            WM_KEYUP,
            static_cast<WPARAM>(__key),
            static_cast<LPARAM>(lParam._uint_v),
            __timeout);
    }
}

void Win::post(Win::Key __key, Win::KeyAction __action) const noexcept
{
    WM_CHAR_LPARAM lParam;

    lParam._uint_v = 0;

    lParam._struct_v.repeatCount = 1;
    lParam._struct_v.scanCode = MapVirtualKey(__key, MAPVK_VK_TO_VSC);

    if (__action == OnlyPress)
    {
        PostMessageW(
            $(_M_handle),
            WM_KEYDOWN,
            static_cast<WPARAM>(__key),
            static_cast<LPARAM>(lParam._uint_v));
    }
    else // if (__action == OnlyRelease)
    {
        lParam._struct_v.transitionState = 1;

        PostMessageW(
            $(_M_handle),
            WM_KEYUP,
            static_cast<WPARAM>(__key),
            static_cast<LPARAM>(lParam._uint_v));
    }
}

Win::Message Win::waitMsg() const noexcept
{
    _Win_Begin_

    MSG msg;
    if (GetMessage(&msg, $(_M_handle), 0, 0) == -1)
    {
        _Win_Failed_with_(Win::Message())
    }

    return Win::Message(msg.message, msg.wParam, msg.lParam);
}

Win::Message Win::waitMsg(Win::Message::msg_type __msg) const noexcept
{
    _Win_Begin_

    MSG msg;
    if (GetMessage(&msg, $(_M_handle), __msg, __msg) == -1)
    {
        _Win_Failed_with_(Win::Message())
    }

    return Win::Message(msg.message, msg.wParam, msg.lParam);
}

Win::Message Win::waitCurrentThreadMsg() noexcept
{
    _Win_Static_Begin_

    MSG msg;
    if (GetMessage(&msg, reinterpret_cast<HWND>(-1), 0, 0) == -1)
    {
        _Win_Failed_with_(Win::Message())
    }

    return Win::Message(msg.message, msg.wParam, msg.lParam);
}

Win::Message Win::waitCurrentThreadMsg(Win::Message::msg_type __msg) noexcept
{
    _Win_Static_Begin_

    MSG msg;
    if (GetMessage(&msg, reinterpret_cast<HWND>(-1), __msg, __msg) == -1)
    {
        _Win_Failed_with_(Win::Message())
    }

    return Win::Message(msg.message, msg.wParam, msg.lParam);
}

void Win::clear(Win::Timeout __timeout) const noexcept
{
    _M_sendMessageW(WM_CLEAR, 0, 0, __timeout);
}

void Win::copy(Win::Timeout __timeout) const noexcept
{
    _M_sendMessageW(WM_COPY, 0, 0, __timeout);
}

void Win::cut(Win::Timeout __timeout) const noexcept
{
    _M_sendMessageW(WM_CUT, 0, 0, __timeout);
}

void Win::paste(Win::Timeout __timeout) const noexcept
{
    _M_sendMessageW(WM_PASTE, 0, 0, __timeout);
}

void Win::undo(Win::Timeout __timeout) const noexcept
{
    _M_sendMessageW(WM_UNDO, 0, 0, __timeout);
}

void Win::wait(Win::Timeout __ms) noexcept
{
    Sleep(__ms);
}

void Win::setShortcut(Win::Shortcut __shortcut, bool __enable) const noexcept
{
    _Win_Begin_

    if (__enable)
    {
        RegisterHotKey(
            $(_M_handle),
            __shortcut.getId(),
            __shortcut.modifiers,
            __shortcut.key);
    }
    else
    {
        UnregisterHotKey($(_M_handle), __shortcut.getId());
    }
}

void Win::_S_bindShortcutToFunction_Proc() noexcept
{
    _Win_Static_Begin_

    MSG msg;

    auto timerId = SetTimer(nullptr, 1, 200, nullptr);

    int ret;

    while ((ret = GetMessage(&msg, NULL, 0, 0)) != 0)
    {
        if (ret == -1)
        {
            _Win_Check_Noreturn_
            _Win_Failed_Noreturn_
            break;
        }

        if (msg.message == WM_TIMER)
        {
            _S_bindShortcutToFunction_Data->_M_tryTodo();
        } else

        if (msg.message == WM_HOTKEY)
        {
            auto [func, param] = _S_bindShortcutToFunction_Data->_M_find(
                Win::Shortcut::fromId(static_cast<int>(msg.wParam)));

            (*func)(param);
        }
    }

    KillTimer(nullptr, timerId);
}

void Win::bindShortcutToFunction(
    Win::Shortcut __shortcut,
    Win::ShortcutFunction __function,
    Win::ShortcutFunctionParam __param,
    bool __enable) noexcept
{
    if (__enable)
    {
        if (_S_bindShortcutToFunction_Data == nullptr)
        {
            _S_bindShortcutToFunction_Data =
                new BindShortcutToFunction_Data(
                    &Win::_S_bindShortcutToFunction_Proc);
        }

        _S_bindShortcutToFunction_Data->_M_applyRegister(
            __shortcut,
            __function,
            __param);
    }
    else
    {
        if (_S_bindShortcutToFunction_Data == nullptr)
        {
            return;
        }

        _S_bindShortcutToFunction_Data->_M_applyUnregister(__shortcut);
    }
}

Win::ShortcutFunction Win::functionFromBoundShortcut(
    Win::Shortcut __shortcut,
    Win::ShortcutFunctionParam* __param) noexcept
{
    auto [func, param] = _S_bindShortcutToFunction_Data->_M_find(__shortcut);

    if (__param != nullptr)
    {
        *__param = param;
    }

    return func;
}

std::unique_ptr<Win::Painter> Win::painter() const noexcept
{
    return std::make_unique<Win::Painter>(_M_handle);
}

Win::Painter* Win::screenPainter() noexcept
{
    static Win::Painter p(nullptr);
    return &p;
}

Win::Color Win::colorUnder(
    const Win::Point& __point) const noexcept
{
    return painter()->colorUnder(__point);
}

Win::ModalDialogButtonsId Win::createModalDialogBox(
    const Win::String& __text,
    Win::MessageLeval __leval) const noexcept
{
    _Win_Begin_

    const char* caption = "Message";
    std::uint32_t type = MB_HELP;

    switch (__leval)
    {
    case Win::MessageLeval::Information:
        caption = "Information";
        type |=
            MB_ICONINFORMATION |
            MB_OK |
            MB_DEFBUTTON1 |
            MB_SETFOREGROUND;
        break;

    case Win::MessageLeval::Question:
        caption = "Question";
        type |=
            MB_ICONQUESTION |
            MB_YESNOCANCEL |
            MB_DEFBUTTON1 |
            MB_SETFOREGROUND;
        break;

    case Win::MessageLeval::Warning:
        caption = "Warning";
        type |=
            MB_ICONWARNING |
            MB_CANCELTRYCONTINUE |
            MB_DEFBUTTON1 |
            MB_SYSTEMMODAL |
            MB_SETFOREGROUND |
            MB_TOPMOST;
        break;

    case Win::MessageLeval::Error:
        caption = "Error";
        type |=
            MB_ICONERROR |
            MB_ABORTRETRYIGNORE |
            MB_DEFBUTTON2 |
            MB_SYSTEMMODAL |
            MB_SETFOREGROUND |
            MB_TOPMOST;
        break;

    default:
        caption = "Unknow";
        type |=
            MB_ICONQUESTION |
            MB_OK |
            MB_DEFBUTTON1;
        break;
    }

    return static_cast<Win::ModalDialogButtonsId>(
        MessageBoxA($(_M_handle), __text.c_str(), caption, type));
}

Win::ModalDialogButtonsId Win::createModalDialogBox(
    const Win::WString& __text,
    Win::MessageLeval __leval) const noexcept
{
    _Win_Begin_

    const wchar_t* caption = L"Message";
    std::uint32_t type = MB_HELP;

    switch (__leval)
    {
    case Win::MessageLeval::Information:
        caption = L"Information";
        type |=
            MB_ICONINFORMATION |
            MB_OK |
            MB_DEFBUTTON1 |
            MB_SETFOREGROUND;
        break;

    case Win::MessageLeval::Question:
        caption = L"Question";
        type |=
            MB_ICONQUESTION |
            MB_YESNOCANCEL |
            MB_DEFBUTTON1 |
            MB_SETFOREGROUND;
        break;

    case Win::MessageLeval::Warning:
        caption = L"Warning";
        type |=
            MB_ICONWARNING |
            MB_CANCELTRYCONTINUE |
            MB_DEFBUTTON2 |
            MB_SYSTEMMODAL |
            MB_SETFOREGROUND |
            MB_TOPMOST;
        break;

    case Win::MessageLeval::Error:
        caption = L"Error";
        type |=
            MB_ICONERROR |
            MB_ABORTRETRYIGNORE |
            MB_DEFBUTTON2 |
            MB_SYSTEMMODAL |
            MB_SETFOREGROUND |
            MB_TOPMOST;
        break;

    default:
        caption = L"Unknow";
        type |=
            MB_ICONQUESTION |
            MB_OK |
            MB_DEFBUTTON1;
        break;
    }

    return static_cast<Win::ModalDialogButtonsId>(
        MessageBoxW($(_M_handle), __text.c_str(), caption, type));
}