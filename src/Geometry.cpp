/**
* Copyright (c) 2024-2025 Yang Huanhuan (3347484963@qq.com).
* 
* This software is provided "as is", without warranty of any kind, express or implied.
*/

/**
* Geometry.cpp In the openWin (https://github.com/huanhuanonly/openWin)
* 
* Created by Yang Huanhuan on January 13, 2025, 21:44:33
* 
* --- This file is a part of openWin ---
* 
* @brief Implement Geometry.h
*/

#include <openWin/Geometry.h>

using namespace win;

Color::Color(Color::Channel __r, Color::Channel __g, Color::Channel __b) noexcept
    : _M_data((__r) | (__g << 8U) | (__b << 16U))
{ }

void Color::setR(Color::Channel __r) noexcept
{
    _M_data = _M_data & ~0x0000ffU | static_cast<decltype(_M_data)>(__r);
}

void Color::setG(Color::Channel __g) noexcept
{
    _M_data = _M_data & ~0x00ff00U | (static_cast<decltype(_M_data)>(__g) << 8U);
}

void Color::setB(Color::Channel __b) noexcept
{
    _M_data = _M_data & ~0xff0000U | (static_cast<decltype(_M_data)>(__b) << 16U);
}

Color::Channel Color::r() const noexcept
{ return static_cast<Color::Channel>(_M_data & 0xffU); }

Color::Channel Color::g() const noexcept
{ return static_cast<Color::Channel>((_M_data >> 8U) & 0xffU); }

Color::Channel Color::b() const noexcept
{ return static_cast<Color::Channel>((_M_data >> 16U) & 0xffU); }

bool Color::operator==(const Color& __other) const noexcept
{ return _M_data == __other._M_data; }