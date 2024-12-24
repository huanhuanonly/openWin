/**
* Win_type.cpp In the openWin (https://github.com/huanhuanonly/openWin)
*
* Created by Yang Huanhuan on November 26, 2024, 15:36:02
*
* Email -> 3347484963@qq.com
*
* --- This file is a part of openWin ---
*
* @brief Implement other classes in Win.h
*/

#include "../include/Win.h"

#include <Windows.h>

Win::Point::Point(int __x, int __y) noexcept
	: _M_x(__x), _M_y(__y)
{ }

bool Win::Point::operator==(const Win::Point& __other) const noexcept
{
	return _M_x == __other._M_x && _M_y == __other._M_y;
}

void Win::Point::setX(int __x) noexcept
{ _M_x = __x; }

void Win::Point::setY(int __y) noexcept
{ _M_y = __y; }

int Win::Point::x() const noexcept
{ return _M_x; }

int Win::Point::y() const noexcept
{ return _M_y; }

Win::Point Win::Point::physics(float __dpi) const noexcept
{
	return Win::Point(
		static_cast<int>(_M_x / __dpi),
		static_cast<int>(_M_y / __dpi));
}

Win::Point& Win::Point::mapto(float __dpi) noexcept
{
	_M_x = static_cast<int>(_M_x * __dpi);
	_M_y = static_cast<int>(_M_y * __dpi);
	return *this;
}

Win::Size::Size(int __width, int __height) noexcept
	: _M_width(__width), _M_height(__height)
{ }

bool Win::Size::operator==(const Win::Size& __other) const noexcept
{
	return _M_width == __other._M_width && _M_height == __other._M_height;
}

void Win::Size::setWidth(int __width) noexcept
{ _M_width = __width; }

void Win::Size::setHeight(int __height) noexcept
{ _M_height = __height; }

int Win::Size::width() const noexcept
{ return _M_width; }

int Win::Size::height() const noexcept
{ return _M_height; }

int Win::Size::w() const noexcept
{ return _M_width; }

int Win::Size::h() const noexcept
{ return _M_height; }


Win::Size Win::Size::physics(float __dpi) const noexcept
{
	return Win::Size(
		static_cast<int>(_M_width / __dpi),
		static_cast<int>(_M_height / __dpi));
}

Win::Size& Win::Size::mapto(float __dpi) noexcept
{
	_M_width = static_cast<int>(_M_width * __dpi);
	_M_height = static_cast<int>(_M_height * __dpi);
	return *this;
}

Win::Rect::Rect(int __x, int __y, int __w, int __h) noexcept
	: Win::Rect::Point(__x, __y), Win::Rect::Size(__w, __h)
{ }

Win::Rect::Rect(const Win::Point& __point, const Win::Size& __size) noexcept
	: Win::Rect::Point(__point), Win::Rect::Size(__size)
{ }

Win::Point& Win::Rect::point() noexcept
{ return *this; }

Win::Size& Win::Rect::size() noexcept
{ return *this; }

const Win::Point& Win::Rect::point() const noexcept
{ return *this; }

const Win::Size& Win::Rect::size() const noexcept
{ return *this; }

bool Win::Rect::operator==(const Rect& __other) const noexcept
{ return point() == __other.point() && size() == __other.size(); }

Win::Rect Win::Rect::physics(float __dpi) const noexcept
{ return Win::Rect(point().physics(__dpi), size().physics(__dpi)); }

Win::Rect& Win::Rect::mapto(float __dpi) noexcept
{
	point().mapto(__dpi);
	size().mapto(__dpi);
	return *this;
}

//Win::ErrorCode::ErrorCode(Win::ErrorCode::type __ec) noexcept
//	: errorCode(__ec)
//{ }
//
//Win::ErrorCode::type Win::ErrorCode::toULong() const noexcept
//{
//	return errorCode;
//}
//
//const char* Win::ErrorCode::toString() const noexcept
//{
//	char* buffer = nullptr;
//	FormatMessageA(
//		FORMAT_MESSAGE_ALLOCATE_BUFFER |
//			FORMAT_MESSAGE_FROM_SYSTEM |
//			FORMAT_MESSAGE_IGNORE_INSERTS,
//		NULL,
//		errorCode,
//		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
//		reinterpret_cast<LPSTR>(&buffer),
//		0,
//		NULL
//	);
//	return buffer;
//}
//
//bool Win::ErrorCode::operator==(const Win::ErrorCode& __other) const
//{
//	return errorCode == __other.errorCode;
//}

Win::Color::Color(Win::Color::Channel __r, Win::Color::Channel __g, Win::Color::Channel __b) noexcept
	: _M_data(RGB(__r, __g, __b))
{ }

void Win::Color::setR(Win::Color::Channel __r) noexcept
{
	_M_data = _M_data & ~0x0000ffU | static_cast<decltype(_M_data)>(__r);
}

void Win::Color::setG(Win::Color::Channel __g) noexcept
{
	_M_data = _M_data & ~0x00ff00U | (static_cast<decltype(_M_data)>(__g) << 8U);
}

void Win::Color::setB(Win::Color::Channel __b) noexcept
{
	_M_data = _M_data & ~0xff0000U | (static_cast<decltype(_M_data)>(__b) << 16U);
}

Win::Color::Channel Win::Color::r() const noexcept
{ return GetRValue(_M_data); }

Win::Color::Channel Win::Color::g() const noexcept
{ return GetGValue(_M_data); }

Win::Color::Channel Win::Color::b() const noexcept
{ return GetBValue(_M_data); }

bool Win::Color::operator==(const Win::Color& __other) const noexcept
{
	return _M_data == __other._M_data;
}

int Win::Shortcut::getId() const noexcept
{
	return static_cast<int>(modifiers | (key << 4));
}

Win::Shortcut Win::Shortcut::fromId(int __id) noexcept
{
	return Win::Shortcut(
		static_cast<Win::Modifiers>(__id & 0x0F),
		static_cast<Win::Key>((__id >> 4) & 0xFF));
}

Win::GlobalShortcutBindingData::~GlobalShortcutBindingData() noexcept
{
	for (auto it = _M_shortcutBindings.begin(); it != _M_shortcutBindings.end(); ++it)
	{
		_M_unregister(it->first);
	}
}

void Win::GlobalShortcutBindingData::_M_applyRegister(
	Win::Shortcut __shortcut,
	Win::ShortcutFunction __function,
	Win::ShortcutFunctionParam __param) noexcept
{
	_M_mutex.lock();
	_M_taskQueue.push_front(std::make_tuple(true, __shortcut, __function, __param));
	_M_mutex.unlock();
}

void Win::GlobalShortcutBindingData::_M_applyUnregister(
	Win::Shortcut __shortcut) noexcept
{
	_M_mutex.lock();
	_M_taskQueue.push_front(std::make_tuple(false, __shortcut, nullptr, nullptr));
	_M_mutex.unlock();
}

void Win::GlobalShortcutBindingData::_M_register(
	Win::Shortcut __shortcut,
	Win::ShortcutFunction __function,
	Win::ShortcutFunctionParam __param) noexcept
{
	if (RegisterHotKey(nullptr, __shortcut.getId(), __shortcut.modifiers, __shortcut.key))
	{
		_M_shortcutBindings[__shortcut] = std::make_pair(__function, __param);
	}
}

void Win::GlobalShortcutBindingData::_M_unregister(
	Win::Shortcut __shortcut) noexcept
{
	if (UnregisterHotKey(nullptr, __shortcut.getId()))
	{
		if (auto it = _M_shortcutBindings.find(__shortcut); it != _M_shortcutBindings.end())
		{
			_M_shortcutBindings.erase(it);
		}
	}
}

void Win::GlobalShortcutBindingData::_M_tryTodo() noexcept
{
	_M_mutex.lock();

	while (_M_taskQueue.empty())
	{
		auto& [flag, sc, sf, sfp] = _M_taskQueue.front();

		if (flag)
			_M_register(sc, sf, sfp);
		else
			_M_unregister(sc);

		_M_taskQueue.pop_front();
	}

	_M_mutex.unlock();
}

std::pair<Win::ShortcutFunction, Win::ShortcutFunctionParam>
	Win::GlobalShortcutBindingData::_M_find(Win::Shortcut __shortcut) const noexcept
{
	std::lock_guard<std::mutex> lg(_M_mutex);

	auto fit = _M_shortcutBindings.find(__shortcut);

	if (fit == _M_shortcutBindings.end())
	{
		return { nullptr, nullptr };
	}
	
	return fit->second;
}