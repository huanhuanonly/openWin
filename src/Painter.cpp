/**
* Copyright (c) 2024-2025 Yang Huanhuan (3347484963@qq.com).
* 
* This software is provided "as is", without warranty of any kind, express or implied.
*/

/**
* Painter.cpp In the openWin (https://github.com/huanhuanonly/openWin)
* 
* Created by Yang Huanhuan on December 9, 2024, 11:32:44
* 
* --- This file is a part of openWin ---
* 
* @brief Implement Painter.h
*/

#include <openWin/Win.h>
#include <openWin/Painter.h>

#include "Built-in/_Windows.h"

using namespace win;

static inline HDC $(Painter::Handle __handle) noexcept
{ return reinterpret_cast<HDC>(__handle); }

#define GetRGBValue(c) GetRValue(c), GetGValue(c), GetBValue(c)

Painter::Painter() noexcept
    : _M_dc(nullptr), _M_dpi(1.00F)
{ }

Painter::Painter(const Win& __win) noexcept
    : _M_dc(GetDC(reinterpret_cast<HWND>(__win.handle())))
    , _M_dpi(__win.dpi())
{ }

Painter::Painter(Painter::Handle __hdc) noexcept
    : _M_dc(__hdc), _M_dpi(1.00F)
{ }

Painter::Painter(std::nullptr_t) noexcept
    : Painter()
{ }

Painter::Painter(const Painter& __other) noexcept
{ *this = __other; }

Painter::Painter(Painter&& __other) noexcept
{ *this = std::forward<Painter>(__other); }

Painter::~Painter() noexcept
{
    ReleaseDC(reinterpret_cast<HWND>(window().handle()), $(_M_dc));
}

Painter& Painter::operator=(const Painter& __other) noexcept
{
    this->~Painter();
    
    _M_dc = GetDC(reinterpret_cast<HWND>(window().handle()));
    _M_dpi = __other._M_dpi;

    return *this;
}

Painter& Painter::operator=(Painter&& __other) noexcept
{
    this->~Painter();

    _M_dc = __other._M_dc;
    _M_dpi = __other._M_dpi;

    return *this;
}

Win Painter::window() const noexcept
{
    return Win(WindowFromDC($(_M_dc)));
}

void Painter::setHandle(Painter::Handle __handle) noexcept
{
    _M_dc = __handle;
}

Painter::Handle Painter::handle() const noexcept
{
    return _M_dc;
}

std::unique_ptr<Painter::Pen>
    Painter::pen() noexcept
{
    return std::make_unique<Painter::Pen>(
        this,
        GetCurrentObject($(_M_dc), OBJ_PEN));
}

std::unique_ptr<Painter::Brush>
    Painter::brush() noexcept
{
    return std::make_unique<Painter::Brush>(
        this,
        GetCurrentObject($(_M_dc), OBJ_BRUSH));
}

std::unique_ptr<Painter::Font>
    Painter::font() noexcept
{
    return std::make_unique<Painter::Font>(
        this,
        GetCurrentObject($(_M_dc), OBJ_FONT));
}

void Painter::setColorUnder(
    const Point& __point,
    const Color& __color) const noexcept
{
    SetPixel(
        $(_M_dc),
        static_cast<std::int32_t>(__point.x() / _M_dpi),
        static_cast<std::int32_t>(__point.y() / _M_dpi),
        RGB(__color.r(), __color.g(), __color.b()));
}

Color Painter::colorUnder(const Point& __point) const noexcept
{
    COLORREF c = GetPixel($(_M_dc), __point.x(), __point.y());
    return Color(GetRGBValue(c));
}

void Painter::setForegroundColor(
    const Color& __color) const noexcept
{
    SetTextColor(
        $(_M_dc),
        RGB(__color.r(), __color.g(), __color.b()));
}

void Painter::setBackgroundColor(
    const Color& __color) const noexcept
{
    SetBkColor(
        $(_M_dc),
        RGB(__color.r(), __color.g(), __color.b()));
}

void Painter::setBackgroundTransparent(
    bool __enable) const noexcept
{
    SetBkMode($(_M_dc), __enable ? TRANSPARENT : OPAQUE);
}

Color Painter::foregroundColor() const noexcept
{
    COLORREF c = GetTextColor($(_M_dc));
    return Color(GetRGBValue(c));
}

Color Painter::backgroundColor() const noexcept
{
    COLORREF c = GetBkColor($(_M_dc));
    return Color(GetRGBValue(c));
}

bool Painter::backgroundTransparent() const noexcept
{
    int mode = GetBkMode($(_M_dc));
    return mode == TRANSPARENT;
}

void Painter::setBlendingMode(
    Painter::BlendingModes __bm) const noexcept
{
    SetROP2($(_M_dc), static_cast<std::int32_t>(__bm));
}

Painter::BlendingModes Painter::blendingMode() const noexcept
{
    return static_cast<Painter::BlendingModes>(
        GetROP2($(_M_dc)));
}

void Painter::drawText(
    const Win::String& __text,
    const Point& __point) const noexcept
{
    TextOutA(
        $(_M_dc),
        static_cast<std::int32_t>(__point.x() / _M_dpi),
        static_cast<std::int32_t>(__point.y() / _M_dpi),
        __text.c_str(),
        static_cast<std::int32_t>(__text.size()));
}

void Painter::drawText(
    const Win::WString& __text,
    const Point& __point) const noexcept
{
    TextOutW(
        $(_M_dc),
        static_cast<std::int32_t>(__point.x() / _M_dpi),
        static_cast<std::int32_t>(__point.y() / _M_dpi),
        __text.c_str(),
        static_cast<std::int32_t>(__text.size()));
}

void Painter::drawText(
    const Win::String& __text,
    const Rect& __rect,
    Alignment __align,
    bool __singleLine) const noexcept
{
    RECT r{
            static_cast<std::int32_t>(__rect.x() / _M_dpi),
            static_cast<std::int32_t>(__rect.y() / _M_dpi),
            static_cast<std::int32_t>((__rect.x() + __rect.w()) / _M_dpi),
            static_cast<std::int32_t>((__rect.y() + __rect.h()) / _M_dpi) };

    DrawTextA(
        $(_M_dc),
        __text.c_str(),
        static_cast<std::int32_t>(__text.size()),
        &r,
        __align.horizontalAlign | __align.verticalAlign | (__singleLine ? DT_SINGLELINE : 0)
            | DT_HIDEPREFIX | DT_NOPREFIX | DT_PATH_ELLIPSIS | DT_WORDBREAK | DT_MODIFYSTRING);
}

void Painter::drawText(
    const Win::WString& __text,
    const Rect& __rect,
    Alignment __align,
    bool __singleLine) const noexcept
{
    RECT r{
            static_cast<std::int32_t>(__rect.x() / _M_dpi),
            static_cast<std::int32_t>(__rect.y() / _M_dpi),
            static_cast<std::int32_t>((__rect.x() + __rect.w()) / _M_dpi),
            static_cast<std::int32_t>((__rect.y() + __rect.h()) / _M_dpi) };

    DrawTextW(
        $(_M_dc),
        __text.c_str(),
        static_cast<std::int32_t>(__text.size()),
        &r,
        __align.horizontalAlign | __align.verticalAlign | (__singleLine ? DT_SINGLELINE : 0)
            | DT_HIDEPREFIX | DT_NOPREFIX | DT_PATH_ELLIPSIS | DT_WORDBREAK | DT_MODIFYSTRING);
}

void Painter::_M_setCurrentPosition(const Point& __point) const noexcept
{
    MoveToEx(
        $(_M_dc),
        static_cast<std::int32_t>(__point.x() / _M_dpi),
        static_cast<std::int32_t>(__point.y() / _M_dpi),
        nullptr);
}

Point Painter::_M_currentPosition() const noexcept
{
    POINT p;
    GetCurrentPositionEx($(_M_dc), &p);

    return Point(p.x, p.y).mapto(_M_dpi);
}

void Painter::_M_drawLineTo(const Point& __point) const noexcept
{
    LineTo(
        $(_M_dc),
        static_cast<std::int32_t>(__point.x() / _M_dpi),
        static_cast<std::int32_t>(__point.y() / _M_dpi));
}

void Painter::drawLine(
    const Point& __start,
    const Point& __end) const noexcept
{
    MoveToEx(
        $(_M_dc),
        static_cast<std::int32_t>(__start.x() / _M_dpi),
        static_cast<std::int32_t>(__start.y() / _M_dpi),
        nullptr);

    LineTo(
        $(_M_dc),
        static_cast<std::int32_t>(__end.x() / _M_dpi),
        static_cast<std::int32_t>(__end.y() / _M_dpi));
}

void Painter::drawLines(
    const Point* __points,
    int __size) const noexcept
{
    if (__size < 2)
    {
        return;
    }

    Polyline(
        $(_M_dc),
        reinterpret_cast<const POINT*>(__points),
        __size);
}

void Painter::drawPolygon(
    const Point* __vertexes,
    int __size) const noexcept
{
    if (__size < 2)
    {
        return;
    }

    Polygon(
        $(_M_dc),
        reinterpret_cast<const POINT*>(__vertexes),
        __size);
}

void Painter::drawRect(const Rect& __rect) const noexcept
{
    Rectangle(
        $(_M_dc),
        static_cast<std::int32_t>(__rect.x() / _M_dpi),
        static_cast<std::int32_t>(__rect.y() / _M_dpi),
        static_cast<std::int32_t>((__rect.x() + __rect.w()) / _M_dpi),
        static_cast<std::int32_t>((__rect.y() + __rect.h()) / _M_dpi));
}

void Painter::drawRect(
    const Rect& __rect,
    const Size& __round) const noexcept
{
    RoundRect(
        $(_M_dc),
        static_cast<std::int32_t>(__rect.x() / _M_dpi),
        static_cast<std::int32_t>(__rect.y() / _M_dpi),
        static_cast<std::int32_t>((__rect.x() + __rect.w()) / _M_dpi),
        static_cast<std::int32_t>((__rect.y() + __rect.h()) / _M_dpi),
        static_cast<std::int32_t>(__round.w() / _M_dpi),
        static_cast<std::int32_t>(__round.h() / _M_dpi));
}

void Painter::drawCircle(
    const Point& __center,
    int __radius,
    float __startAngle,
    float __sweepAngle) const noexcept
{
    AngleArc(
        $(_M_dc),
        static_cast<std::int32_t>(__center.x() / _M_dpi),
        static_cast<std::int32_t>(__center.y() / _M_dpi),
        static_cast<std::int32_t>(__radius / _M_dpi),
        __startAngle,
        __sweepAngle);
}

void Painter::drawCircle(
    const Point& __point1,
    const Point& __point2,
    float __startAngle,
    float __sweepAngle) const noexcept
{
    Point center(
        (__point1.x() + __point2.x()) / 2,
        (__point1.y() + __point2.y()) / 2);

    drawCircle(
        center,
        center.distance(__point1),
        __startAngle,
        __sweepAngle);
}

void Painter::drawEllipse(
    const Rect& __rect) const noexcept
{
    Ellipse(
        $(_M_dc),
        static_cast<std::int32_t>(__rect.x() / _M_dpi),
        static_cast<std::int32_t>(__rect.y() / _M_dpi),
        static_cast<std::int32_t>((__rect.x() + __rect.w()) / _M_dpi),
        static_cast<std::int32_t>((__rect.y() + __rect.h()) / _M_dpi));
}

void Painter::drawArc(
    const Rect& __rect,
    const Point& __start,
    const Point& __end) const noexcept
{
    Arc(
        $(_M_dc),
        static_cast<std::int32_t>(__rect.x() / _M_dpi),
        static_cast<std::int32_t>(__rect.y() / _M_dpi),
        static_cast<std::int32_t>((__rect.x() + __rect.w()) / _M_dpi),
        static_cast<std::int32_t>((__rect.y() + __rect.h()) / _M_dpi),
        static_cast<std::int32_t>(__start.x() / _M_dpi),
        static_cast<std::int32_t>(__start.y() / _M_dpi),
        static_cast<std::int32_t>(__end.x() / _M_dpi),
        static_cast<std::int32_t>(__end.y() / _M_dpi));
}

void Painter::drawChord(
    const Rect& __rect,
    const Point& __start,
    const Point& __end) const noexcept
{
    Chord(
        $(_M_dc),
        static_cast<std::int32_t>(__rect.x() / _M_dpi),
        static_cast<std::int32_t>(__rect.y() / _M_dpi),
        static_cast<std::int32_t>((__rect.x() + __rect.w()) / _M_dpi),
        static_cast<std::int32_t>((__rect.y() + __rect.h()) / _M_dpi),
        static_cast<std::int32_t>(__start.x() / _M_dpi),
        static_cast<std::int32_t>(__start.y() / _M_dpi),
        static_cast<std::int32_t>(__end.x() / _M_dpi),
        static_cast<std::int32_t>(__end.y() / _M_dpi));
}

void Painter::drawPie(
    const Rect& __rect,
    const Point& __start,
    const Point& __end) const noexcept
{
    Pie(
        $(_M_dc),
        static_cast<std::int32_t>(__rect.x() / _M_dpi),
        static_cast<std::int32_t>(__rect.y() / _M_dpi),
        static_cast<std::int32_t>((__rect.x() + __rect.w()) / _M_dpi),
        static_cast<std::int32_t>((__rect.y() + __rect.h()) / _M_dpi),
        static_cast<std::int32_t>(__start.x() / _M_dpi),
        static_cast<std::int32_t>(__start.y() / _M_dpi),
        static_cast<std::int32_t>(__end.x() / _M_dpi),
        static_cast<std::int32_t>(__end.y() / _M_dpi));
}

void Painter::_M_drawPolyBezier(
    const Point* __points,
    int __size) const noexcept
{
    PolyBezier(
        $(_M_dc),
        reinterpret_cast<const POINT*>(__points),
        static_cast<DWORD>(__size));
}

void Painter::invert(
    const Rect& __rect) const noexcept
{
    RECT r{
        static_cast<std::int32_t>(__rect.x() / _M_dpi),
        static_cast<std::int32_t>(__rect.y() / _M_dpi),
        static_cast<std::int32_t>((__rect.x() + __rect.w()) / _M_dpi),
        static_cast<std::int32_t>((__rect.y() + __rect.h()) / _M_dpi) };

    InvertRect($(_M_dc), &r);
}