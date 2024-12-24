/**
* PathGenerator.h In the openWin (https://github.com/huanhuanonly/openWin)
*
* Created by Yang Huanhuan on December 2, 2024, 22:29:10
*
* Email -> 3347484963@qq.com
*
* --- This file is a part of openWin ---
*
* @package pg
* 
* @brief Encapsulates an abstract template class, which is
*        a generator template used to generate a path
*        between two points and control the movement speed.
*        The points can be of any dimension (1D, 2D or 3D,
*        etc.).
*/

#pragma once

#ifndef PATH_GENERATOR_H
#define PATH_GENERATOR_H

#include <type_traits>
#include <utility>
#include <thread>

namespace pg
{
    template<typename _Combination, typename _ElementType, std::size_t _Dimension>
    class PathGenerator
    {
    public:

        static_assert(
            std::is_integral_v<_ElementType> || std::is_floating_point_v<_ElementType>,
            "PathGenerator::_ElementType is not an integral or real type.");

        static_assert(
            _Dimension > 0,
            "PathGenerator::_Dimension equal to zero is meaningless.");

        static_assert(
#if true
            sizeof(_Combination) >= sizeof(_ElementType) * _Dimension,
#else
            sizeof(_Combination) == sizeof(_ElementType) * _Dimension,
#endif
            "_Combination cannot hold _Dimension _ElementType.");

        using point_type = _Combination;
        using value_type = _ElementType;

        constexpr
            std::size_t dimension() const noexcept
        {
            return _Dimension;
        }

        PathGenerator() = default;

        explicit PathGenerator(std::uint32_t __slowDown)
            : _M_slowDown(__slowDown)
        {
        }

        void setSlowDown(std::uint32_t __slowDown) noexcept
        {
            _M_slowDown = __slowDown;
        }

        [[nodiscard]]
        std::uint32_t slowDown() const noexcept
        {
            return _M_slowDown;
        }

        virtual void build(const point_type& __from, const point_type& __to) = 0;

        virtual ~PathGenerator()
        {
        }

        [[nodiscard]]
        inline point_type get() const
        {
            if (_M_slowDown)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(_M_slowDown));
            }

            return _M_get();
        }

        [[nodiscard]]
        inline point_type operator()() const
        {
            return get();
        }

        virtual void next() = 0;

        [[nodiscard]]
        virtual bool atEnd() const = 0;

        inline operator bool() const
        {
            return atEnd() == false;
        }

    protected:

        [[nodiscard]]
        virtual point_type _M_get() const = 0;

    private:

        std::uint32_t _M_slowDown = 0;

    public:

        /**
        * @brief Used to iterate value_type in point_type
        */
        class CombinationIterator
        {
        public:

            using iterator = value_type*;
            using const_iterator = const value_type*;

            using reference = value_type&;
            using const_reference = const value_type&;

            explicit CombinationIterator(point_type& __data) noexcept
                : _M_begin(reinterpret_cast<iterator>(std::addressof(__data)))
            {
            }

            [[nodiscard]] constexpr
                iterator begin() noexcept
            {
                return _M_begin;
            }

            [[nodiscard]] constexpr
                const_iterator begin() const noexcept
            {
                return _M_begin;
            }

            [[nodiscard]] constexpr
                const_iterator cbegin() const noexcept
            {
                return _M_begin;
            }

            [[nodiscard]] constexpr
                const_iterator end() noexcept
            {
                return _M_begin + _Dimension;
            }

            [[nodiscard]] constexpr
                const_iterator end() const noexcept
            {
                return _M_begin + _Dimension;
            }

            [[nodiscard]] constexpr
                const_iterator cend() const noexcept
            {
                return _M_begin + _Dimension;
            }

            [[nodiscard]] constexpr
                reference at(std::size_t __i) noexcept
            {
                return *(_M_begin + __i);
            }

            [[nodiscard]] constexpr
                const_reference at(std::size_t __i) const noexcept
            {
                return *(_M_begin + __i);
            }

            [[nodiscard]] constexpr
                reference operator[](std::size_t __i) noexcept
            {
                return *(_M_begin + __i);
            }

            [[nodiscard]] constexpr
                const_reference operator[](std::size_t __i) const noexcept
            {
                return *(_M_begin + __i);
            }

        private:

            iterator _M_begin;
        };

        [[nodiscard]] static constexpr
            CombinationIterator combinationIterator(point_type& __data) noexcept
        {
            return CombinationIterator(__data);
        }
    };

}  // namespace pg

#endif  // PATH_GENERATOR_H