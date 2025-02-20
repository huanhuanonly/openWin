/**
* Copyright (c) 2024-2025 Yang Huanhuan (3347484963@qq.com).
* 
* This software is provided "as is", without warranty of any kind, express or implied.
*/

/**
* LinkedList.h In the openWin (https://github.com/huanhuanonly/openWin)
* 
* Created by Yang Huanhuan on February 19, 2025, 21:41:59
* 
* --- This file is a part of openWin ---
* 
* @brief A lightweight doubly linked list.
*/

#pragma once

#ifndef OPENWIN_HEADER_DS_LINKEDLIST_H
#define OPENWIN_HEADER_DS_LINKEDLIST_H

#include <utility>
#include <iterator>
#include <optional>
#include <memory>

#include <initializer_list>

#include <cstddef>

namespace win::ds
{

template<typename _Tp>
class LinkedList
{
public:

    using value_type = _Tp;

    using pointer = _Tp*;
    using const_pointer = const _Tp*;

    using reference = _Tp&;
    using const_reference = const _Tp&;

    struct node
    {
        mutable node* prev;
        mutable node* next;

        value_type element;

        node() = default;

        template<typename... _Args>
        node(const node* const __prev, const node* const __next, _Args&& ...__args)
            : prev(const_cast<node*>(__prev))
            , next(const_cast<node*>(__next))
            , element(std::forward<_Args>(__args)...)
        { }
    };

protected:

    struct endnode
    {
        mutable node* prev;
        mutable node* next;
    };

public:

    class iterator
    {
    public:

        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = _Tp;
        using difference_type = std::ptrdiff_t;
        using pointer = _Tp*;
        using reference = _Tp&;

        iterator() = default;

        iterator(node* const __ptr) : _M_current(__ptr) { }

        iterator(endnode* const __ptr)
            : _M_current(reinterpret_cast<node*>(__ptr))
        { }


        reference operator*() noexcept
        { return _M_current->element; }

        const_reference operator*() const noexcept
        { return _M_current->element; }


        pointer operator->() noexcept
        { return &_M_current->element; }

        const_pointer operator->() const noexcept
        { return &_M_current->element; }


        [[nodiscard]] bool operator==(const iterator& __other) const noexcept
        { return _M_current == __other._M_current; }

        [[nodiscard]] bool operator!=(const iterator& __other) const noexcept
        { return _M_current != __other._M_current; }


        iterator& operator++() noexcept
        {
            _M_current = _M_current->next;
            return *this;
        }

        const iterator& operator++() const noexcept
        {
            _M_current = _M_current->next;
            return *this;
        }

        iterator operator++(int) noexcept
        {
            return iterator(std::exchange(_M_current, _M_current->next));
        }

        const iterator operator++(int) const noexcept
        {
            return iterator(std::exchange(_M_current, _M_current->next));
        }

        iterator& operator--() noexcept
        {
            _M_current = _M_current->prev;
            return *this;
        }

        const iterator& operator--() const noexcept
        {
            _M_current = _M_current->prev;
            return *this;
        }

        iterator operator--(int) noexcept
        {
            return iterator(std::exchange(_M_current, _M_current->prev));
        }

        const iterator operator--(int) const noexcept
        {
            return iterator(std::exchange(_M_current, _M_current->prev));
        }


        node* current() noexcept
        { return _M_current; }

        const node* current() const noexcept
        { return _M_current; }

    private:

        mutable node* _M_current = nullptr;
    };

    using const_iterator = const iterator;

    LinkedList() = default;

    LinkedList(const LinkedList& __other)
        : LinkedList(__other.clone())
    { }

    LinkedList(LinkedList&& __other) noexcept
    {
        std::swap(_M_root, __other._M_root);
        std::swap(_M_last, __other._M_last);
    }

    LinkedList(std::initializer_list<value_type> __init)
    {
        for (auto it = __init.end(); it != __init.begin(); --it)
        {
            insert(*(it - 1));
        }
    }

    ~LinkedList()
    {
        clear();

        delete _M_last;
    }

    LinkedList& operator=(const LinkedList& __other)
    {
        return *this = __other.clone();
    }

    LinkedList& operator=(LinkedList&& __other)
    {
        clear();
        
        std::swap(_M_root, __other._M_root);
        std::swap(_M_last, __other._M_last);

        return *this;
    }

    [[nodiscard]] bool operator==(const LinkedList& __other)
    {
        auto it = begin(), oit = __other.begin();

        for (; it != end() && oit != __other.end(); ++it, ++oit)
        {
            if (*it != *oit)
            {
                return false;
            }
        }

        return it == end() && oit == __other.end();
    }

    [[nodiscard]] bool operator!=(const LinkedList& __other)
    { return not (*this == __other); }


    [[nodiscard]] bool empty() const noexcept
    { return reinterpret_cast<endnode*>(_M_root) == _M_last; }

    [[nodiscard]] LinkedList clone() const
    {
        LinkedList result;

        for (auto it = begin(); it != end(); ++it)
        {
            result.append(*it);
        }

        return result;
    }


    [[nodiscard]] iterator begin() noexcept
    { return iterator(_M_root); }

    [[nodiscard]] iterator end() noexcept
    { return iterator(_M_last); }

    [[nodiscard]] const_iterator begin() const noexcept
    { return const_iterator(_M_root); }

    [[nodiscard]] const_iterator end() const noexcept
    { return const_iterator(_M_last); }

    [[nodiscard]] const_iterator cbegin() const noexcept
    { return const_iterator(_M_root); }

    [[nodiscard]] const_iterator cend() const noexcept
    { return const_iterator(_M_last); }

    
    /**
     * @brief Inserts __value at the beginning.
     */
    void insert(const value_type& __value)
    {
        emplace(__value);
    }

    /**
     * @brief Inserts __value at the beginning.
     */
    void insert(value_type&& __value)
    {
        emplace(std::move(__value));
    }

    /**
     * @brief Inserts __node at the beginning.
     */
    void insert(std::unique_ptr<node> __node)
    {
        __node->prev = nullptr;
        __node->next = _M_root;

        _M_root = __node.release();
        _M_root->next->prev = _M_root;
    }

    /**
     * @brief Inserts __value at the beginning.
     */
    template<typename... _Args>
    void emplace(_Args&& ...__args)
    {
        _M_root = new node(nullptr, _M_root, std::forward<_Args>(__args)...);

        // if (_M_root->next)

        _M_root->next->prev = _M_root;
    }

    /**
     * @brief Inserts __value before __iter.
     */
    void insert(const_iterator __iter, const value_type& __value)
    {
        emplace(__iter, __value);
    }

    /**
     * @brief Inserts __value before __iter.
     */
    void insert(const_iterator __iter, value_type&& __value)
    {
        emplace(__iter, std::move(__value));
    }

    /**
     * @brief Inserts __node before __iter.
     */
    void insert(const_iterator __iter, std::unique_ptr<node> __node)
    {
        if (__iter == begin())
        {
            insert(std::move(__node));
            return;
        }

        __node->prev = __iter.current()->prev;
        __node->next = const_cast<node*>(__iter.current());

        __iter.current()->prev = __node.release();

        if (__iter.current()->prev->prev)
        {
            __iter.current()->prev->prev->next = __iter.current()->prev;
        }
    }

    /**
     * @brief Inserts __value before __iter.
     */
    template<typename... _Args>
    void emplace(const_iterator __iter, _Args&& ...__args)
    {
        if (__iter == begin())
        {
            emplace(std::forward<_Args>(__args)...);
            return;
        }

        __iter.current()->prev = new node(
            __iter.current()->prev,
            __iter.current(),
            std::forward<_Args>(__args)...);

        if (__iter.current()->prev->prev)
        {
            __iter.current()->prev->prev->next = __iter.current()->prev;
        }
    }


    void append(const value_type& __value)
    {
        insert(end(), __value);
    }

    void append(value_type&& __value)
    {
        insert(end(), std::move(__value));
    }

    void append(std::unique_ptr<node> __node)
    {
        insert(end(), std::move(__node));
    }

    template<typename... _Args>
    void append(_Args&& ...__args)
    {
        emplace(end(), std::forward<_Args>(__args)...);
    }


    std::unique_ptr<node> extract(const_iterator __iter) noexcept
    {
        if (__iter.current()->prev)
        {
            __iter.current()->prev->next = __iter.current()->next;
        }
        else /// __iter == begin()
        {
            /// Only the prev of begin() is nullptr.

            _M_root = _M_root->next;
        }

        /// Only the next of end() is nullptr.

        // if (__iter.current()->next)
        __iter.current()->next->prev = __iter.current()->prev;
        
        return std::unique_ptr<node>(const_cast<node*>(__iter.current()));
    }

    /**
     * @brief Removes the first element.
     */
    void remove()
    {
        // if (empty())
        // {
        //     return;
        // }

        delete std::exchange(_M_root, _M_root->next);

        _M_root->prev = nullptr;
    }

    void remove(const_iterator __iter)
    {
        extract(__iter);
    }

    /**
     * @brief Removes all elements equal to __value.
     */
    void remove(const value_type& __value)
    {
        remove([&](const value_type& __current) -> bool
        {
            return __current == __value;
        });
    }

    template<typename _Predicate>
    void remove(_Predicate __pred)
    {
        for (auto it = begin(); it != end(); ++it)
        {
            if (__pred(*it))
            {
                remove(it);
            }
        }
    }

    
    /**
     * @brief Removes the last element.
     * 
     * @time O(1)
     */
    void removeLast()
    {
        // if (empty())
        // {
        //     return;
        // }

        remove(const_iterator(_M_last->prev));
    }

    
    /**
     * @brief Removes all elements.
     * 
     * @time O(N)
     */
    void clear()
    {
        for (; not empty(); remove());
    }
    

    /**
     * @time O(1)
     */
    [[nodiscard]] reference front() noexcept
    { return _M_root->element; }

    [[nodiscard]] const_reference front() const noexcept
    { return _M_root->element; }


    /**
     * @time O(1)
     */
    [[nodiscard]] reference back() noexcept
    { return _M_last->prev->element; }

    [[nodiscard]] const_reference back() const noexcept
    { return _M_last->prev->element; }


    /**
     * @param __n If it is a positive integer or 0, search from front to back,
     *            otherwise search from back to front.
     * 
     *            For example, 0 means the first element, -1 means the last
     *            element.
     * 
     * @time O(n)
     */
    [[nodiscard]] reference operator[](std::make_signed_t<std::size_t> __n) noexcept
    {
        if (__n >= 0)
            return *std::next(begin(), __n);
        else
            return *std::next(end(), __n);
    }

    [[nodiscard]] const_reference operator[](std::make_signed_t<std::size_t> __n) const noexcept
    {
        if (__n >= 0)
            return *std::next(begin(), __n);
        else
            return *std::next(end(), __n);
    }

    /**
     * @time O(n)
     */
    [[nodiscard]] std::optional<reference> at(std::make_signed_t<std::size_t> __n) noexcept
    {
        if (__n >= 0)
        {
            auto it = begin();

            for (; __n--; ++it)
            {
                if (it == end())
                {
                    return std::nullopt;
                }
            }

            return *it;
        }
        else
        {
            if (empty())
            {
                return std::nullopt;
            }

            auto it = end();
            
            for (--it; ++__n; --it)
            {
                if (it == begin())
                {
                    if (__n + 1)
                    {
                        return std::nullopt;
                    }
                    
                    break;
                }
            }

            return *it;
        }
    }

    [[nodiscard]] std::optional<const_reference>
        at(std::make_signed_t<std::size_t> __n) const noexcept
    {
        if (__n >= 0)
        {
            auto it = begin();

            for (; __n--; ++it)
            {
                if (it == end())
                {
                    return std::nullopt;
                }
            }

            return *it;
        }
        else
        {
            if (empty())
            {
                return std::nullopt;
            }

            auto it = end();
            
            for (--it; ++__n; --it)
            {
                if (it == begin())
                {
                    if (__n + 1)
                    {
                        return std::nullopt;
                    }
                    
                    break;
                }
            }

            return *it;
        }
    }

    /**
     * @time O(N)
     */
    [[nodiscard]] std::size_t size() const noexcept
    {
        return std::distance(begin(), end());
    }

protected:

    node* _M_root = reinterpret_cast<node*>(new endnode{ nullptr, nullptr });
    endnode* _M_last = reinterpret_cast<endnode*>(_M_root);
};

}  // namespace win::ds

#endif  // OPENWIN_HEADER_DS_LINKEDLIST_H