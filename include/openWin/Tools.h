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
* @brief Some utility functions and type traits.
*/

#pragma once

#ifndef OPENWIN_HEADER_TOOLS_H
#define OPENWIN_HEADER_TOOLS_H

#include <type_traits>

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


template<bool _Test, typename _Ft, typename _St>
constexpr const std::conditional_t<_Test, _Ft, _St>&
    conditional_value(const _Ft& __fv, const _St& __sv) noexcept
{ if constexpr (_Test) { return __fv; } else { return __sv; } }

template<typename _Tp>
constexpr const _Tp&
    conditional_value(bool __test, const _Tp& __fv, const _Tp& __sv) noexcept
{ if (__test) { return __fv; } else { return __sv; } }


template<typename _Tp, typename = std::void_t<>>
struct has_value_type : std::false_type
{ };

template<typename _Tp>
struct has_value_type<_Tp, std::void_t<typename _Tp::value_type>> : std::true_type
{ };

template<typename _Tp>
constexpr bool has_value_type_v = has_value_type<_Tp>::value;


template<typename _Tp, typename _Or = _Tp, bool = has_value_type_v<_Tp>>
struct value_type_or
{ using type = _Or; };

template<typename _Tp, typename _Or>
struct value_type_or<_Tp, _Or, true>
{ using type = _Tp::value_type; };

template<typename _Tp, typename _Or = _Tp>
using value_type_or_t = value_type_or<_Tp, _Or>::type;


template<typename _Tp, typename _KeyType = std::size_t, typename = std::void_t<>>
struct is_accessible_by_index : std::false_type
{ };

template<typename _Tp, typename _KeyType>
struct is_accessible_by_index<_Tp, _KeyType,
    std::void_t<decltype(std::declval<_Tp>()[std::declval<_KeyType>()])>> : std::true_type
{ };

template<typename _Tp, typename _KeyType = std::size_t>
constexpr bool is_accessible_by_index_v = is_accessible_by_index<_Tp, _KeyType>::value;

}  // namespace win::tools

#endif  // OPENWIN_HEADER_TOOLS_H