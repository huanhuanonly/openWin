/**
* Copyright (c) 2024-2025 Yang Huanhuan (3347484963@qq.com).
* 
* This software is provided "as is", without warranty of any kind, express or implied.
*/

/**
* BasicPathGenerator.h In the openWin (https://github.com/huanhuanonly/openWin)
* 
* Created by Yang Huanhuan on December 2, 2024, 22:29:10
* 
* --- This file is a part of openWin ---
* 
* @package pg
* 
* @brief Encapsulates an abstract template class, which is a generator template used to generate a
*        path between two points and control the movement speed. The points can be of any dimension
*        (1D, 2D or 3D, etc.).
*/

#pragma once

#ifndef OPENWIN_HEADER_PATH_GENERATOR_H
#define OPENWIN_HEADER_PATH_GENERATOR_H

#include <type_traits>
#include <utility>
#include <memory>
#include <thread>

#include "../Tools.h"

namespace win::pg
{

#define OPENWIN_PATHGENERATOR_TEMPLATE \
    template< \
        typename _Container, \
        typename _ValueType = ::win::tools::value_type_or_t<_Container>, \
        ::std::size_t _DataOffset = ::win::tools::conditional_value<::std::is_polymorphic_v<_Container>>(sizeof(void*), 0), \
        ::std::size_t _ElementCount = (sizeof(_Container) - _DataOffset) / sizeof(_ValueType)>

#define OPENWIN_PATHGENERATOR_BASE \
        BasicPathGenerator<_Container, _ValueType, _DataOffset, _ElementCount>

#define OPENWIN_PATHGENERATOR(className) \
    OPENWIN_PATHGENERATOR_TEMPLATE \
    class className : public OPENWIN_PATHGENERATOR_BASE


OPENWIN_PATHGENERATOR_TEMPLATE
class BasicPathGenerator
{
public:

    using container_type = _Container;
    using value_type = _ValueType;

    static_assert(
        std::disjunction_v<
            std::negation<std::is_same<container_type, value_type>>,
            std::negation<std::is_class<container_type>>,
            tools::has_value_type<container_type>>,
        "The second template parameter _ValueType may need to be explicitly specified.");

    BasicPathGenerator() = default;

    explicit BasicPathGenerator(std::uint32_t __waitingTime)
        : _M_waitingTime(__waitingTime)
    { }

    virtual ~BasicPathGenerator() { }

    [[nodiscard]] static constexpr std::size_t dimension() noexcept
    { return _ElementCount; }

    void setWaitingTime(std::uint32_t __ms) noexcept
    { _M_waitingTime = __ms; }

    [[nodiscard]] std::uint32_t waitingTime() const noexcept
    { return _M_waitingTime; }

    void wait() const noexcept
    {
        if (_M_waitingTime)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(_M_waitingTime));
        }
    }

#if true
public:
#else
protected:
#endif

    [[nodiscard]] static value_type&
        valueAt(_Container& __c, std::size_t __i) noexcept
    {
        if constexpr (tools::is_accessible_by_index_v<container_type>)
        {
            return __c[__i];
        }
        else
        {
            return *(reinterpret_cast<value_type*>(reinterpret_cast<std::byte*>(std::addressof(__c)) + _DataOffset) + __i);
        }
    }

    [[nodiscard]] static const value_type&
        valueAt(const _Container& __c, std::size_t __i) noexcept
    {
        if constexpr (tools::is_accessible_by_index_v<container_type>)
        {
            return __c[__i];
        }
        else
        {
            return *(reinterpret_cast<const value_type*>(reinterpret_cast<const std::byte*>(std::addressof(__c)) + _DataOffset) + __i);
        }
    }

public:

    class ForwardIterator
    {
    public:

        ForwardIterator(
            const BasicPathGenerator* const __parent,
            const container_type& __from,
            const container_type& __to)
            : _M_parent(__parent)
            , _M_starting(__from)
            , _M_end(__to)
        { }

        [[nodiscard]] const BasicPathGenerator* parent() const noexcept
        { return _M_parent; }

        [[nodiscard]] const container_type& starting() const noexcept
        { return _M_starting; }

        [[nodiscard]] const container_type& end() const noexcept
        { return _M_end; }


        [[nodiscard]] container_type current()
        { return _V_current(); }

        void advance()
        { _V_advance(); parent()->wait(); }

        [[nodiscard]] bool remains()
        { return _V_remains(); }


        [[nodiscard]] container_type operator*()
        { return current(); }

        [[nodiscard]] container_type operator()()
        { return current(); }

        ForwardIterator& operator++()
        { return advance(), *this; }

        [[nodiscard]] operator bool()
        { return remains(); }

    protected:

        virtual container_type _V_current() = 0;
        virtual void _V_advance() = 0;
        virtual bool _V_remains() = 0;

    private:

        const BasicPathGenerator* const _M_parent;

        container_type _M_starting;
        container_type _M_end;
    };

    [[nodiscard]]
    virtual std::unique_ptr<ForwardIterator> build(
        const container_type& __from,
        const container_type& __to) const = 0;

private:

    std::uint32_t _M_waitingTime = 0;
};

}  // namespace win::pg

#endif  // OPENWIN_HEADER_PATH_GENERATOR_H