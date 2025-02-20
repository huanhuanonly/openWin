/**
* Copyright (c) 2024-2025 Yang Huanhuan (3347484963@qq.com).
* 
* This software is provided "as is", without warranty of any kind, express or implied.
*/

/**
* ForwardList.h In the openWin (https://github.com/huanhuanonly/openWin)
* 
* Created by Yang Huanhuan on February 10, 2025, 1:17:38
* 
* --- This file is a part of openWin ---
* 
* @brief A lightweight singly linked list.
*/

#pragma once

#ifndef OPENWIN_HEADER_DS_FORWARDLIST_H
#define OPENWIN_HEADER_DS_FORWARDLIST_H

#include <utility>
#include <iterator>
#include <optional>

namespace win::ds
{

enum class ForwardListTag
{
    Base = 0,
    FastAppend
};

template<typename _Tp, ForwardListTag = ForwardListTag::Base>
class ForwardList
{
public:

    using value_type = _Tp;

    using pointer = _Tp*;
    using const_pointer = const _Tp*;

    using reference = _Tp&;
    using const_reference = const _Tp&;

    struct node
    {
        value_type element;
        mutable node* next;
    };

    class iterator
    {
    public:

        using iterator_category = std::forward_iterator_tag;
        using value_type = _Tp;
        using difference_type = std::ptrdiff_t;
        using pointer = _Tp*;
        using reference = _Tp&;

        iterator() = default;

        iterator(node* const __ptr) : _M_current(__ptr) { }


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

        node* current() noexcept
        { return _M_current; }

        const node* current() const noexcept
        { return _M_current; }

    private:

        mutable node* _M_current = nullptr;
    };

    using const_iterator = const iterator;


    ForwardList() = default;

    ForwardList(const ForwardList& __other)
        : ForwardList(__other.clone())
    { }

    ForwardList(ForwardList&& __other) noexcept
        : _M_root(__other._M_root)
    { __other._M_root = nullptr; }

    ~ForwardList()
    { clear(); }

    ForwardList& operator=(const ForwardList& __other)
    {
        return *this = __other.clone();
    }

    ForwardList& operator=(ForwardList&& __other)
    {
        clear();

        _M_root = std::exchange(__other._M_root, nullptr);

        return *this;
    }

    [[nodiscard]] bool operator==(const ForwardList& __other)
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

    [[nodiscard]] bool operator!=(const ForwardList& __other)
    { return not (*this == __other); }


    [[nodiscard]] bool empty() const noexcept
    { return _M_root == nullptr; }


    [[nodiscard]] ForwardList clone() const
    {
        ForwardList result;

        if (empty())
        {
            return result;
        }

        result._M_root = new node{ _M_root->element, nullptr };

        for (node* cp = result._M_root,* self_cp = _M_root->next; self_cp; self_cp = self_cp->next)
        {
            cp = cp->next = new node{ self_cp->element, nullptr };
        }

        return result;
    }

    [[nodiscard]] iterator begin() noexcept
    { return iterator(_M_root); }

    [[nodiscard]] iterator end() noexcept
    { return iterator(); }

    [[nodiscard]] const_iterator begin() const noexcept
    { return const_iterator(_M_root); }

    [[nodiscard]] const_iterator end() const noexcept
    { return const_iterator(); }

    [[nodiscard]] const_iterator cbegin() const noexcept
    { return const_iterator(_M_root); }

    [[nodiscard]] const_iterator cend() const noexcept
    { return const_iterator(); }


    /**
     * @brief Inserts __value at the beginning.
     */
    void insert(const value_type& __value)
    {
        _M_root = new node{ __value, _M_root };
    }

    /**
     * @brief Inserts __value at the beginning.
     */
    void insert(value_type&& __value)
    {
        _M_root = new node{ std::move(__value), _M_root };
    }

    /**
     * @brief Inserts __value at the beginning.
     */
    template<typename... _Args>
    void emplace(_Args&& ...__args)
    {
        _M_root = new node{ value_type(std::forward<_Args>(__args)...), _M_root };
    }

    /**
     * @brief Inserts __value after __iter.
     */
    void insertAfter(const_iterator __iter, const value_type& __value)
    {
        __iter.current()->next = new node{ __value, __iter.current()->next };
    }

    /**
     * @brief Inserts __value after __iter.
     */
    void insertAfter(const_iterator __iter, value_type&& __value)
    {
        __iter.current()->next = new node{ std::move(__value), __iter.current()->next };
    }

    template<typename... _Args>
    void emplaceAfter(const_iterator __iter, _Args&& ...__args)
    {
        __iter.current()->next = new node{ value_type(std::forward<_Args>(__args)...), __iter.current()->next };
    }


    /**
     * @brief Removes the first element.
     */
    void remove()
    {
        delete std::exchange(_M_root, _M_root->next);
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
        while (_M_root && __pred(_M_root->element))
        {
            remove();
        }

        if (_M_root == nullptr)
        {
            return;
        }

        for (node* cp = _M_root; cp->next; cp = cp->next)
        {
            while (cp->next && __pred(cp->next->element))
            {
                remove(cp);
            }
        }
    }

    /**
     * @brief Removes the next element of __prevIter.
     */
    void remove(const_iterator __prevIter)
    {
        delete std::exchange(__prevIter.current()->next, __prevIter.current()->next->next);
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
     * @time O(N)
     */
    [[nodiscard]] reference back() noexcept
    {
        for (node* cp = _M_root; ; cp = cp->next)
        {
            if (cp->next == nullptr)
            {
                return cp->element;
            }
        }
    }

    /**
     * @time O(N)
     */
    [[nodiscard]] const_reference back() const noexcept
    {
        for (const node* cp = _M_root; ; cp = cp->next)
        {
            if (cp->next == nullptr)
            {
                return cp->element;
            }
        }
    }

    /**
     * @time O(n)
     */
    [[nodiscard]] reference operator[](std::size_t __n) noexcept
    {
        return *std::next(begin(), __n);
    }

    [[nodiscard]] const_reference operator[](std::size_t __n) const noexcept
    {
        return *std::next(begin(), __n);
    }

    /**
     * @time O(n)
     */
    [[nodiscard]] std::optional<reference> at(std::size_t __n) noexcept
    {
        node* cp = _M_root;

        for (; __n-- && cp; cp = cp->next);

        if (cp)
            return cp->element;
        else
            return std::nullopt;
    }

    [[nodiscard]] std::optional<const_reference> at(std::size_t __n) const noexcept
    {
        node* cp = _M_root;

        for (; __n-- && cp; cp = cp->next);

        if (cp)
            return cp->element;
        else
            return std::nullopt;
    }


    /**
     * @time O(N)
     */
    [[nodiscard]] std::size_t size() const noexcept
    {
        return std::distance(begin(), end());
    }

protected:

    node* _M_root = nullptr;
};

template<typename _Tp>
class ForwardList<_Tp, ForwardListTag::FastAppend> : public ForwardList<_Tp, ForwardListTag::Base>
{
    using Base = ForwardList<_Tp, ForwardListTag::Base>;

public:

    using typename Base::value_type;
    using typename Base::node;

    using typename Base::pointer;
    using typename Base::const_pointer;

    using typename Base::reference;
    using typename Base::const_reference;

    using typename Base::iterator;
    using typename Base::const_iterator;

    ForwardList() = default;

    ForwardList(const ForwardList& __other)
        : ForwardList(__other.clone())
    { }

    ForwardList(ForwardList&& __other) noexcept
        : Base(__other), _M_last(__other._M_last)
    { _M_last = nullptr; }


    ForwardList& operator=(const ForwardList& __other)
    {
        return *this = __other.clone();
    }

    ForwardList& operator=(ForwardList&& __other)
    {
        clear();

        this->_M_root = std::exchange(__other._M_root, nullptr);
        this->_M_last = std::exchange(__other._M_last, nullptr);
    }


    void clear()
    {
        Base::clear();
        _M_last = nullptr;
    }

    [[nodiscard]] ForwardList clone() const
    {
        ForwardList result;

        for (auto it = this->cbegin(); it != this->cend(); ++it)
        {
            result.append(*it);
        }

        return result;
    }


    /**
     * @time O(1)
     */
    void append(const value_type& __value)
    {
        Base::insertAfter(const_iterator(_M_last), __value);

        _M_last = _M_last->next;
    }

    /**
     * @time O(1)
     */
    void append(value_type&& __value)
    {
        Base::insertAfter(const_iterator(_M_last), std::move(__value));

        _M_last = _M_last->next;
    }

    template<typename... _Args>
    void append(_Args&& ...__args)
    {
        Base::emplaceAfter(const_iterator(_M_last), std::forward<_Args>(__args)...);

        _M_last = _M_last->next;
    }

    /**
     * @time O(1)
     */
    [[nodiscard]] reference back() noexcept
    { return _M_last->element; }

    /**
     * @time O(1)
     */
    [[nodiscard]] const_reference back() const noexcept
    { return _M_last->element; }


    void remove()
    {
        Base::remove();

        if (this->empty())
        {
            _M_last = nullptr;
        }
    }

    void remove(const value_type& __value)
    {
        Base::remove(__value);
        
        if (this->empty())
        {
            _M_last = nullptr;
        }
    }

    void remove(const_iterator __prevIter)
    {
        Base::remove(__prevIter);

        if (this->empty())
        {
            _M_last = nullptr;
        }
    }

private:

    node* _M_last = nullptr;
};

}  // namespace win::ds

#endif  // OPENWIN_HEADER_DS_FORWARDLIST_H