/**
* Copyright (c) 2024-2025 Yang Huanhuan (3347484963@qq.com).
* 
* This software is provided "as is", without warranty of any kind, express or implied.
*/

/**
* Linear.h In the openWin (https://github.com/huanhuanonly/openWin)
* 
* Created by Yang Huanhuan on December 3, 2024, 16:20:36
* 
* --- This file is a part of openWin ---
* 
* @package pg: Encapsulates the class inherited from PathGenerator.
* 
* @brief Encapsulates a linear movement path generator.
*/

#pragma once

#ifndef OPENWIN_HEADER_PG_LINEAR_H
#define OPENWIN_HEADER_PG_LINEAR_H

#include "PathGenerator.h"

#include <array>
#include <algorithm>

namespace win::pg
{

template<typename _Combination, typename _ElementType, std::size_t _Dimension>
class Linear final : public PathGenerator<_Combination, _ElementType, _Dimension>
{
public:

    using typename PathGenerator<_Combination, _ElementType, _Dimension>::point_type;
    using typename PathGenerator<_Combination, _ElementType, _Dimension>::value_type;

    using real = long double;

    static Linear* global() noexcept
    {
        thread_local Linear object;
        return &object;
    }

    Linear() noexcept
        : _M_speed(1.00f)
    { }

    /**
    * @param __speed    The distance between two points
    *                   multiplied by it, it cannot be zero;
    * 
    * @param __slowDown The time interval (millisecond)
    *                   between each move;
    */
    Linear(float __speed, std::uint32_t __slowDown = 0)
        : PathGenerator<_Combination, _ElementType, _Dimension>(__slowDown)
        , _M_speed(__speed)
    { }

    virtual void build(const point_type& __from, const point_type& __to) override
    {
        _M_from = __from;
        _M_to = __to;
        _M_pos = 0;
        _M_block = 1.00L;

        auto from_iter = this->combinationIterator(const_cast<point_type&>(__from));
        auto to_iter = this->combinationIterator(const_cast<point_type&>(__to));
        
        for (int i = 0; i < static_cast<int>(_Dimension); ++i)
        {
            _M_block = std::max(
                _M_block,
                static_cast<decltype(_M_block)>(std::abs(from_iter[i] - to_iter[i])));
        }

        _M_block /= _M_speed;
    }

    virtual void next()
    {
        ++_M_pos;
    }

    [[nodiscard]]
    virtual bool atEnd() const
    {
        return _M_pos > static_cast<int>(std::ceil(_M_block));
    }

protected:

    [[nodiscard]]
    virtual point_type _M_get() const override
    {
        if (_M_pos == 0)
        {
            return _M_from;
        }

        if (_M_pos == static_cast<int>(std::ceil(_M_block)))
        {
            return _M_to;
        }

        point_type res{};
        auto res_iter = this->combinationIterator(res);

        auto from_iter = this->combinationIterator(const_cast<point_type&>(_M_from));
        auto to_iter = this->combinationIterator(const_cast<point_type&>(_M_to));

        for (int i = 0; i < static_cast<int>(_Dimension); ++i)
        {
            res_iter[i] = from_iter[i] + static_cast<int>((to_iter[i] - from_iter[i]) / _M_block * _M_pos);
        }

        return res;
    }

private:

    point_type _M_from;
    point_type _M_to;

    int _M_pos;
    float _M_speed;

    real _M_block;
};

}  // namespace win::pg

#endif  // OPENWIN_HEADER_PG_LINEAR_H