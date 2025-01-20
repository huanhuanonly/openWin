/**
* Copyright (c) 2024-2025 Yang Huanhuan (3347484963@qq.com).
* 
* This software is provided "as is", without warranty of any kind, express or implied.
*/

/**
* Geometry.h In the openWin (https://github.com/huanhuanonly/openWin)
* 
* Created by Yang Huanhuan on January 13, 2025, 20:45:17
* 
* --- This file is a part of openWin ---
* 
* @brief Defines geometry-related types.
*/

#pragma once

#ifndef OPENWIN_HEADER_GEOMETRY_H
#define OPENWIN_HEADER_GEOMETRY_H

#include <iostream>

#include <cstdint>
#include <cmath>

namespace win
{

template<typename _Tp>
class BasicPoint
{
public:

    using value_type = _Tp;

    BasicPoint() = default;

    BasicPoint(value_type __x, value_type __y) noexcept
        : _M_x(__x), _M_y(__y)
    { }

    bool operator==(const BasicPoint& __other) const noexcept
    { return _M_x == __other._M_x && _M_y == __other._M_y; }

    void setX(value_type __x) noexcept
    { _M_x = __x; }

    void setY(value_type __y) noexcept
    { _M_y = __y; }

    [[nodiscard]] value_type x() const noexcept
    { return _M_x; }

    [[nodiscard]] value_type y() const noexcept
    { return _M_y; }

    [[nodiscard]]
    value_type distance(const BasicPoint& __other) const noexcept
    {
        return static_cast<value_type>(
            std::sqrt(
                (_M_x - __other._M_x) * (_M_x - __other._M_x) +
                (_M_y - __other._M_y) * (_M_y - __other._M_y)));
    }

    [[nodiscard]]
    static value_type distance(const BasicPoint& __first, const BasicPoint& __second) noexcept
    { return __first.distance(__second); }


    template<typename _OtherType>
    [[nodiscard]]
    BasicPoint operator+(_OtherType __other) const noexcept
    {
        return BasicPoint(
            static_cast<value_type>(_M_x + __other),
            static_cast<value_type>(_M_y + __other));
    }

    template<typename _OtherType>
    [[nodiscard]]
    BasicPoint operator-(_OtherType __other) const noexcept
    {
        return BasicPoint(
            static_cast<value_type>(_M_x - __other),
            static_cast<value_type>(_M_y - __other));
    }

    template<typename _OtherType>
    BasicPoint& operator+=(_OtherType __other) noexcept
    { return _M_x += __other, _M_y += __other, *this; }

    template<typename _OtherType>
    BasicPoint& operator-=(_OtherType __other) noexcept
    { return _M_x -= __other, _M_y -= __other, *this; }

    friend inline std::ostream& operator<<(std::ostream& __os, const BasicPoint& __point)
    { return __os << "BasicPoint { x = " << __point.x() << ", y = " << __point.y() << " }"; }

    friend class Win;
    friend class Painter;

public:

    [[nodiscard]] BasicPoint physics(float __dpi) const noexcept
    {
        return BasicPoint(
		    static_cast<value_type>(_M_x / __dpi),
		    static_cast<value_type>(_M_y / __dpi));
    }

    BasicPoint& mapto(float __dpi) noexcept
    {
        _M_x = static_cast<value_type>(_M_x * __dpi);
	    _M_y = static_cast<value_type>(_M_y * __dpi);
	    return *this;
    }

protected:

    value_type _M_x = 0;
    value_type _M_y = 0;
};

using Point = BasicPoint<int>;
using PointF = BasicPoint<double>;


template<typename _Tp>
class BasicSize
{
public:

    using value_type = _Tp;

    BasicSize() = default;

    BasicSize(value_type __width, value_type __height) noexcept
        : _M_width(__width), _M_height(__height)
    { }

    [[nodiscard]] 
    bool operator==(const BasicSize& __other) const noexcept
    { return _M_width == __other._M_width && _M_height == __other._M_height; }

    void setWidth(value_type __width) noexcept
    { _M_width = __width; }

    void setHeight(value_type __height) noexcept
    { _M_height = __height; }

    [[nodiscard]] value_type width() const noexcept
    { return _M_width; }

    [[nodiscard]] value_type height() const noexcept
    { return _M_height; }

    [[nodiscard]] value_type w() const noexcept
    { return width(); }

    [[nodiscard]] value_type h() const noexcept
    { return height(); }

    template<typename _OtherType>
    [[nodiscard]]
    BasicSize operator*(_OtherType __other) const noexcept
    {
        return BasicSize(
            static_cast<value_type>(_M_width * __other),
            static_cast<value_type>(_M_height * __other));
    }

    template<typename _OtherType>
    [[nodiscard]]
    BasicSize operator/(_OtherType __other) const noexcept
    {
        return BasicSize(
            static_cast<value_type>(_M_width / __other),
            static_cast<value_type>(_M_height / __other));
    }

    template<typename _OtherType>
    BasicSize& operator*=(_OtherType __other) noexcept
    { return _M_width *= __other, _M_height *= __other, *this; }

    template<typename _OtherType>
    BasicSize& operator/=(_OtherType __other) noexcept
    { return _M_width /= __other, _M_height /= __other, *this; }

    friend inline std::ostream& operator<<(std::ostream& __os, const BasicSize& __size)
    { return __os << "BasicSize { width = " << __size.w() << ", height = " << __size.h() << " }"; }

    friend class Win;
    friend class Painter;

public:

    [[nodiscard]] BasicSize physics(float __dpi) const noexcept
    {
        return BasicSize(
		    static_cast<value_type>(_M_width / __dpi),
		    static_cast<value_type>(_M_height / __dpi));
    }

    BasicSize& mapto(float __dpi) noexcept
    {
        _M_width = static_cast<value_type>(_M_width * __dpi);
	    _M_height = static_cast<value_type>(_M_height * __dpi);
	    return *this;
    }

protected:

    value_type _M_width = 0;
    value_type _M_height = 0;
};

using Size = BasicSize<int>;
using SizeF = BasicSize<double>;


template<typename _Tp>
class BasicRect : public BasicPoint<_Tp>, public BasicSize<_Tp>
{
public:

    using value_type = _Tp;

    BasicRect() = default;

    BasicRect(value_type __x, value_type __y, value_type __w, value_type __h) noexcept
        : BasicPoint<_Tp>(__x, __y), BasicSize<_Tp>(__w, __h)
    { }

    BasicRect(const BasicPoint<_Tp>& __point, const BasicSize<_Tp>& __size) noexcept
        : BasicPoint<_Tp>(__point), BasicSize<_Tp>(__size)
    { }

    [[nodiscard]] BasicPoint<_Tp>& point() noexcept
    { return *this; }

    [[nodiscard]] BasicSize<_Tp>& size() noexcept
    { return *this; }

    [[nodiscard]] const BasicPoint<_Tp>& point() const noexcept
    { return *this; }

    [[nodiscard]] const BasicSize<_Tp>& size() const noexcept
    { return *this; }

    [[nodiscard]]
    bool operator==(const BasicRect& __other) const noexcept
    { return point() == __other.point() && size() == __other.size(); }

    friend inline std::ostream& operator<<(std::ostream& __os, const BasicRect& __rect)
    { return __os << "BasicRect { " << __rect.point() << ", " << __rect.size() << " }"; }

    friend class Win;
    friend class Painter;

public:

    [[nodiscard]] BasicRect physics(float __dpi) const noexcept
    { return BasicRect(point().physics(__dpi), size().physics(__dpi)); }

    BasicRect& mapto(float __dpi) noexcept
    {
        point().mapto(__dpi);
	    size().mapto(__dpi);
	    return *this;
    }
};

using Rect = BasicRect<int>;
using RectF = BasicRect<double>;



class Color
{
public:

    using Channel = std::uint8_t;

    Color() = default;

    Color(Channel __r, Channel __g, Channel __b) noexcept;

    void setR(Channel __r) noexcept;
    void setG(Channel __g) noexcept;
    void setB(Channel __b) noexcept;

    [[nodiscard]] Channel r() const noexcept;
    [[nodiscard]] Channel g() const noexcept;
    [[nodiscard]] Channel b() const noexcept;

    [[nodiscard]] bool operator==(const Color& __other) const noexcept;

    friend class Win;
    friend class Painter;

protected:

    std::uint32_t _M_data = 0;
};


struct BezierVertex
{
    Point forwardControl;
    Point vertex;
    Point backwardControl;
};


}  // namespace win

#endif  // OPENWIN_HEADER_GEOMETRY_H