/**
* Copyright (c) 2024-2025 Yang Huanhuan (3347484963@qq.com).
* 
* This software is provided "as is", without warranty of any kind, express or implied.
*/

/**
* Tools.h In the openWin (https://github.com/huanhuanonly/openWin)
* 
* Created by Yang Huanhuan on January 13, 2025, 22:17:01
* 
* --- This file is a part of openWin ---
* 
* @brief Some utility functions.
*/

#pragma once

#ifndef OPENWIN_HEADER_TOOLS_H
#define OPENWIN_HEADER_TOOLS_H

namespace win::tools
{

template<typename _Res, typename _Tp>
constexpr _Res& force_cast(const _Tp& __value) noexcept
{ return *reinterpret_cast<_Res*>(const_cast<_Tp*>(&__value)); }


template<typename _Lv, typename _Rv>
constexpr _Lv& assign_as(_Lv& __lvalue, const _Rv& __rvalue) noexcept
{ return __lvalue = static_cast<_Lv>(__rvalue); }


template<typename _Lv, typename _Rv>
constexpr _Lv& force_assign(_Lv& __lvalue, const _Rv& __rvalue) noexcept
{ return __lvalue = force_cast<_Lv>(__rvalue); }


template<typename _Tp>
constexpr bool is_within_range(_Tp __value, _Tp __min, _Tp __max)
	noexcept(noexcept(__value >= __min and __value <= __max))
{ return __value >= __min and __value <= __max; }

}  // namespace win

#endif  // OPENWIN_HEADER_TOOLS_H