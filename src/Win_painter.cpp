/**
* Win_painter.cpp In the openWin (https://github.com/huanhuanonly/openWin)
*
* Created by Yang Huanhuan on December 9, 2024, 11:32:44
*
* Email -> 3347484963@qq.com
*
* --- This file is a part of openWin ---
*
* @brief Implement Painter in Win.h
*/

#include "../include/Win.h"

#include <Windows.h>

extern inline HWND $(Win::Handle __handle) noexcept;

static inline HDC _(Win::Painter::Handle __handle) noexcept
{ return reinterpret_cast<HDC>(__handle); }

#define GetRGBValue(c) GetRValue(c), GetGValue(c), GetBValue(c)

Win::Painter::Painter() noexcept
	: _M_dc(nullptr), _M_dpi(1.00F)
{ }

Win::Painter::Painter(const Win& __win) noexcept
	: _M_dc(GetDC($(__win._M_handle)))
	, _M_dpi(__win.dpi())
{ }

Win::Painter::Painter(Win::Handle __winHandle) noexcept
	: _M_dc(GetDC(reinterpret_cast<HWND>(__winHandle)))
	, _M_dpi(Win(__winHandle).dpi())
{ }

Win::Painter::Painter(std::nullptr_t) noexcept
	: Painter()
{ }

Win::Painter::Painter(const Win::Painter& __other) noexcept
{ *this = __other; }

Win::Painter::Painter(Win::Painter&& __other) noexcept
{ *this = std::forward<Win::Painter>(__other); }

Win::Painter::~Painter() noexcept
{
	ReleaseDC($(window()._M_handle), _(_M_dc));
}

Win::Painter& Win::Painter::operator=(const Win::Painter& __other) noexcept
{
	this->~Painter();
	
	_M_dc = GetDC($(window()._M_handle));
	_M_dpi = __other._M_dpi;

	return *this;
}

Win::Painter& Win::Painter::operator=(Win::Painter&& __other) noexcept
{
	this->~Painter();

	_M_dc = __other._M_dc;
	_M_dpi = __other._M_dpi;

	return *this;
}

Win Win::Painter::window() const noexcept
{
	return Win(WindowFromDC(_(_M_dc)));
}

void Win::Painter::setColorUnder(
	const Win::Point& __point,
	const Win::Color& __color) const noexcept
{
	SetPixel(
		_(_M_dc),
		static_cast<std::int32_t>(__point.x() / _M_dpi),
		static_cast<std::int32_t>(__point.y() / _M_dpi),
		RGB(__color.r(), __color.g(), __color.b()));
}

Win::Color Win::Painter::colorUnder(const Point& __point) const noexcept
{
	COLORREF c = GetPixel(_(_M_dc), __point.x(), __point.y());
	return Win::Color(GetRGBValue(c));
}

void Win::Painter::setForegroundColor(
	const Win::Color& __color) const noexcept
{
	SetTextColor(
		_(_M_dc),
		RGB(__color.r(), __color.g(), __color.b()));
}

void Win::Painter::setBackgroundColor(
	const Win::Color& __color) const noexcept
{
	SetBkColor(
		_(_M_dc),
		RGB(__color.r(), __color.g(), __color.b()));
}

void Win::Painter::setBackgroundTransparent(
	bool __enable) const noexcept
{
	SetBkMode(_(_M_dc), __enable ? TRANSPARENT : OPAQUE);
}

Win::Color Win::Painter::foregroundColor() const noexcept
{
	COLORREF c = GetTextColor(_(_M_dc));
	return Win::Color(GetRGBValue(c));
}

Win::Color Win::Painter::backgroundColor() const noexcept
{
	COLORREF c = GetBkColor(_(_M_dc));
	return Win::Color(GetRGBValue(c));
}

bool Win::Painter::backgroundTransparent() const noexcept
{
	int mode = GetBkMode(_(_M_dc));
	return mode == TRANSPARENT;
}

void Win::Painter::setBlendingMode(
	Win::Painter::BlendingModes __bm) const noexcept
{
	SetROP2(_(_M_dc), static_cast<std::int32_t>(__bm));
}

Win::Painter::BlendingModes Win::Painter::blendingMode() const noexcept
{
	return static_cast<Win::Painter::BlendingModes>(
		GetROP2(_(_M_dc)));
}

void Win::Painter::drawText(
	const Win::String& __text,
	const Win::Point& __point) const noexcept
{
	TextOutA(
		_(_M_dc),
		static_cast<std::int32_t>(__point.x() / _M_dpi),
		static_cast<std::int32_t>(__point.y() / _M_dpi),
		__text.c_str(),
		static_cast<std::int32_t>(__text.size()));
}

void Win::Painter::drawText(
	const Win::WString& __text,
	const Win::Point& __point) const noexcept
{
	TextOutW(
		_(_M_dc),
		static_cast<std::int32_t>(__point.x() / _M_dpi),
		static_cast<std::int32_t>(__point.y() / _M_dpi),
		__text.c_str(),
		static_cast<std::int32_t>(__text.size()));
}

void Win::Painter::drawText(
	const Win::String& __text,
	const Win::Rect& __rect,
	Win::Alignment __align,
	bool __singleLine) const noexcept
{
	RECT r{
			static_cast<std::int32_t>(__rect.x() / _M_dpi),
			static_cast<std::int32_t>(__rect.y() / _M_dpi),
			static_cast<std::int32_t>((__rect.x() + __rect.w()) / _M_dpi),
			static_cast<std::int32_t>((__rect.y() + __rect.h()) / _M_dpi) };

	DrawTextA(
		_(_M_dc),
		__text.c_str(),
		static_cast<std::int32_t>(__text.size()),
		&r,
		__align.horizontalAlign | __align.verticalAlign | (__singleLine ? DT_SINGLELINE : 0)
			| DT_HIDEPREFIX | DT_NOPREFIX | DT_PATH_ELLIPSIS | DT_WORDBREAK | DT_MODIFYSTRING);
}

void Win::Painter::drawText(
	const Win::WString& __text,
	const Win::Rect& __rect,
	Win::Alignment __align,
	bool __singleLine) const noexcept
{
	RECT r{
			static_cast<std::int32_t>(__rect.x() / _M_dpi),
			static_cast<std::int32_t>(__rect.y() / _M_dpi),
			static_cast<std::int32_t>((__rect.x() + __rect.w()) / _M_dpi),
			static_cast<std::int32_t>((__rect.y() + __rect.h()) / _M_dpi) };

	DrawTextW(
		_(_M_dc),
		__text.c_str(),
		static_cast<std::int32_t>(__text.size()),
		&r,
		__align.horizontalAlign | __align.verticalAlign | (__singleLine ? DT_SINGLELINE : 0)
			| DT_HIDEPREFIX | DT_NOPREFIX | DT_PATH_ELLIPSIS | DT_WORDBREAK | DT_MODIFYSTRING);
}

void Win::Painter::_M_setCurrentPosition(const Win::Point& __point) const noexcept
{
	MoveToEx(
		_(_M_dc),
		static_cast<std::int32_t>(__point.x() / _M_dpi),
		static_cast<std::int32_t>(__point.y() / _M_dpi),
		nullptr);
}

Win::Point Win::Painter::_M_currentPosition() const noexcept
{
	POINT p;
	GetCurrentPositionEx(_(_M_dc), &p);

	return Win::Point(p.x, p.y).mapto(_M_dpi);
}

void Win::Painter::_M_drawLineTo(const Win::Point& __point) const noexcept
{
	LineTo(
		_(_M_dc),
		static_cast<std::int32_t>(__point.x() / _M_dpi),
		static_cast<std::int32_t>(__point.y() / _M_dpi));
}

void Win::Painter::drawLine(
	const Win::Point& __start,
	const Win::Point& __end) const noexcept
{
	MoveToEx(
		_(_M_dc),
		static_cast<std::int32_t>(__start.x() / _M_dpi),
		static_cast<std::int32_t>(__start.y() / _M_dpi),
		nullptr);

	LineTo(
		_(_M_dc),
		static_cast<std::int32_t>(__end.x() / _M_dpi),
		static_cast<std::int32_t>(__end.y() / _M_dpi));
}

void Win::Painter::drawLines(
	const Win::Point* __points,
	int __size) const noexcept
{
	if (__size < 2)
	{
		return;
	}

	Polyline(
		_(_M_dc),
		reinterpret_cast<const POINT*>(__points),
		__size);
}

void Win::Painter::drawPolygon(
	const Win::Point* __vertexes,
	int __size) const noexcept
{
	if (__size < 2)
	{
		return;
	}

	Polygon(
		_(_M_dc),
		reinterpret_cast<const POINT*>(__vertexes),
		__size);
}

void Win::Painter::drawRect(const Win::Rect& __rect) const noexcept
{
	Rectangle(
		_(_M_dc),
		static_cast<std::int32_t>(__rect.x() / _M_dpi),
		static_cast<std::int32_t>(__rect.y() / _M_dpi),
		static_cast<std::int32_t>((__rect.x() + __rect.w()) / _M_dpi),
		static_cast<std::int32_t>((__rect.y() + __rect.h()) / _M_dpi));
}

void Win::Painter::drawRect(
	const Win::Rect& __rect,
	const Win::Size& __round) const noexcept
{
	RoundRect(
		_(_M_dc),
		static_cast<std::int32_t>(__rect.x() / _M_dpi),
		static_cast<std::int32_t>(__rect.y() / _M_dpi),
		static_cast<std::int32_t>((__rect.x() + __rect.w()) / _M_dpi),
		static_cast<std::int32_t>((__rect.y() + __rect.h()) / _M_dpi),
		static_cast<std::int32_t>(__round.w() / _M_dpi),
		static_cast<std::int32_t>(__round.h() / _M_dpi));
}

void Win::Painter::drawCircle(
	const Win::Point& __center,
	int __radius,
	float __startAngle,
	float __sweepAngle) const noexcept
{
	AngleArc(
		_(_M_dc),
		static_cast<std::int32_t>(__center.x() / _M_dpi),
		static_cast<std::int32_t>(__center.y() / _M_dpi),
		static_cast<std::int32_t>(__radius / _M_dpi),
		__startAngle,
		__sweepAngle);
}

void Win::Painter::drawEllipse(
	const Win::Rect& __rect) const noexcept
{
	Ellipse(
		_(_M_dc),
		static_cast<std::int32_t>(__rect.x() / _M_dpi),
		static_cast<std::int32_t>(__rect.y() / _M_dpi),
		static_cast<std::int32_t>((__rect.x() + __rect.w()) / _M_dpi),
		static_cast<std::int32_t>((__rect.y() + __rect.h()) / _M_dpi));
}

void Win::Painter::drawArc(
	const Win::Rect& __rect,
	const Win::Point& __start,
	const Win::Point& __end) const noexcept
{
	Arc(
		_(_M_dc),
		static_cast<std::int32_t>(__rect.x() / _M_dpi),
		static_cast<std::int32_t>(__rect.y() / _M_dpi),
		static_cast<std::int32_t>((__rect.x() + __rect.w()) / _M_dpi),
		static_cast<std::int32_t>((__rect.y() + __rect.h()) / _M_dpi),
		static_cast<std::int32_t>(__start.x() / _M_dpi),
		static_cast<std::int32_t>(__start.y() / _M_dpi),
		static_cast<std::int32_t>(__end.x() / _M_dpi),
		static_cast<std::int32_t>(__end.y() / _M_dpi));
}

void Win::Painter::drawChord(
	const Win::Rect& __rect,
	const Win::Point& __start,
	const Win::Point& __end) const noexcept
{
	Chord(
		_(_M_dc),
		static_cast<std::int32_t>(__rect.x() / _M_dpi),
		static_cast<std::int32_t>(__rect.y() / _M_dpi),
		static_cast<std::int32_t>((__rect.x() + __rect.w()) / _M_dpi),
		static_cast<std::int32_t>((__rect.y() + __rect.h()) / _M_dpi),
		static_cast<std::int32_t>(__start.x() / _M_dpi),
		static_cast<std::int32_t>(__start.y() / _M_dpi),
		static_cast<std::int32_t>(__end.x() / _M_dpi),
		static_cast<std::int32_t>(__end.y() / _M_dpi));
}

void Win::Painter::drawPie(
	const Win::Rect& __rect,
	const Win::Point& __start,
	const Win::Point& __end) const noexcept
{
	Pie(
		_(_M_dc),
		static_cast<std::int32_t>(__rect.x() / _M_dpi),
		static_cast<std::int32_t>(__rect.y() / _M_dpi),
		static_cast<std::int32_t>((__rect.x() + __rect.w()) / _M_dpi),
		static_cast<std::int32_t>((__rect.y() + __rect.h()) / _M_dpi),
		static_cast<std::int32_t>(__start.x() / _M_dpi),
		static_cast<std::int32_t>(__start.y() / _M_dpi),
		static_cast<std::int32_t>(__end.x() / _M_dpi),
		static_cast<std::int32_t>(__end.y() / _M_dpi));
}

void Win::Painter::_M_drawPolyBezier(
	const Win::Point* __points,
	int __size) const noexcept
{
	PolyBezier(
		_(_M_dc),
		reinterpret_cast<const POINT*>(__points),
		static_cast<DWORD>(__size));
}

void Win::Painter::invert(
	const Win::Rect& __rect) const noexcept
{
	RECT r {
		static_cast<std::int32_t>(__rect.x() / _M_dpi),
		static_cast<std::int32_t>(__rect.y() / _M_dpi),
		static_cast<std::int32_t>((__rect.x() + __rect.w()) / _M_dpi),
		static_cast<std::int32_t>((__rect.y() + __rect.h()) / _M_dpi) };

	InvertRect(_(_M_dc), &r);
}