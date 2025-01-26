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

#include "BasicPathGenerator.h"

#include <array>
#include <algorithm>

namespace win::pg
{

OPENWIN_PATHGENERATOR(Linear)
{
public:

    using Base = OPENWIN_PATHGENERATOR_BASE;

    using typename Base::container_type;
    using typename Base::value_type;

    Linear() = default;

    Linear(float __speed, std::uint32_t __waitingTime = 0) noexcept
        : Base(__waitingTime), _M_speed(__speed)
    { }

    class ForwardIterator : public Base::ForwardIterator
    {
    public:

        ForwardIterator(
            const Base* __parent,
            const container_type& __from,
            const container_type& __to)
            : Base::ForwardIterator(__parent, __from, __to)
            , _M_pos(1)
            , _M_block(1.00f)
        {
            for (std::size_t i = 0; i < Base::dimension(); ++i)
            {
                _M_block = std::max<decltype(_M_block)>(
                    _M_block,
                    std::abs(Base::valueAt(__from, i) - Base::valueAt(__to, i)));
            }

            _M_block /= static_cast<const Linear*>(this->parent())->_M_speed;
        }

    protected:

        virtual container_type _V_current() override
        {
            if (_M_pos == static_cast<decltype(_M_pos)>(std::ceil(_M_block)))
            {
                return this->end();
            }

            container_type res;

            for (std::size_t i = 0; i < Base::dimension(); ++i)
            {
                tools::assign_as(
                    Base::valueAt(res, i),
                    Base::valueAt(this->starting(), i) +
                        (Base::valueAt(this->end(), i) - Base::valueAt(this->starting(), i)) /
                        _M_block *
                        _M_pos);
            }

            return res;
        }

        virtual void _V_advance() override
        { ++_M_pos; }

        virtual bool _V_remains() override
        { return _M_pos <= static_cast<decltype(_M_pos)>(std::ceil(_M_block)); }

    private:

        std::uint64_t _M_pos;
        double _M_block;
    };

    [[nodiscard]]
    virtual std::unique_ptr<typename Base::ForwardIterator> build(
        const container_type& __from,
        const container_type& __to) const override
    {
        return std::unique_ptr<typename Base::ForwardIterator>(
            static_cast<typename Base::ForwardIterator*>(new ForwardIterator(this, __from, __to)));
    }

private:

    float _M_speed = 1.00f;
};

}  // namespace win::pg

#endif  // OPENWIN_HEADER_PG_LINEAR_H