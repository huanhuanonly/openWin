/**
* pg_Linear.h In the openWin (https://github.com/huanhuanonly/openWin)
*
* Created by Yang Huanhuan on December 3, 2024, 11:18:27
*
* Email -> 3347484963@qq.com
*
* --- This file is a part of openWin ---
*
* @package pg, Encapsulates the class inherited from
*              PathGenerator.
*
* @brief Encapsulates a path generator that contains
*        only the starting point and the end point.
*/

#pragma once

#ifndef PG_DIRECT_H
#define PG_DIRECT_H

#include "PathGenerator.h"

#include <array>

namespace pg
{

template<typename _Combination, typename _ElementType, std::size_t _Dimension>
class Direct final : public PathGenerator<_Combination, _ElementType, _Dimension>
{
public:

    using typename PathGenerator<_Combination, _ElementType, _Dimension>::point_type;
    using typename PathGenerator<_Combination, _ElementType, _Dimension>::value_type;

    using PathGenerator<_Combination, _ElementType, _Dimension>::PathGenerator;

    static Direct* global() noexcept
    {
        thread_local Direct object;
        return &object;
    }

    Direct() = default;

    virtual void build(const point_type& __from, const point_type& __to) override
    {
        _M_data = { __from, __to };
        _M_pos = 0;
    }

    virtual void next()
    { ++_M_pos; }

    [[nodiscard]]
    virtual bool atEnd() const
    { return _M_pos >= 2; }

protected:

    [[nodiscard]]
    virtual point_type _M_get() const override
    {
        return _M_data[_M_pos];
    }

private:

    std::array<point_type, 2> _M_data;
    int _M_pos;
};

}  // namespace pg

#endif  // PG_DIRECT_H