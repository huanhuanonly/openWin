/**
* Copyright (c) 2024-2025 Yang Huanhuan (3347484963@qq.com).
* 
* This software is provided "as is", without warranty of any kind, express or implied.
*/

/**
* PainterResources.cpp In the openWin (https://github.com/huanhuanonly/openWin)
* 
* Created by Yang Huanhuan on December 25, 2024, 9:26:53
* 
* --- This file is a part of openWin ---
* 
* @brief Implement Pen, Brush, Font and GeometricPen classes for Painter.
*/

#include <openWin/Painter.h>
#include <openWin/Tools.h>

#include "Built-in/_Windows.h"

using namespace win;
using namespace win::tools;

static inline HDC $(Painter::Handle __handle) noexcept
{ return reinterpret_cast<HDC>(__handle); }

#define GetRGBValue(c) GetRValue(c), GetGValue(c), GetBValue(c)


/// --- In Painter::Resource ---

Painter::Resource::Resource(
    Painter* __painter,
    Painter::Resource::Object __object) noexcept
    : _M_painter(__painter), _M_object(__object)
{ }

Painter::Resource::~Resource() noexcept
{
    if (_M_painter == nullptr)
    {
        destroyObject();
    }
}

Painter* Painter::Resource::painter() noexcept
{ return _M_painter; }

const Painter* Painter::Resource::painter() const noexcept
{ return _M_painter; }

Painter::Resource::Object
    Painter::Resource::object() const noexcept
{ return _M_object; }

void Painter::Resource::destroyObject() noexcept
{
    if (_M_object != nullptr)
    {
        DeleteObject(_M_object);
        _M_object = nullptr;
    }
}

void Painter::Resource::onSet() const noexcept
{
    if (_M_painter == nullptr)
    {
        return;
    }

    SelectObject(
        $(_M_painter->handle()),
        _M_object);
}

Painter::Resource::Resource(Painter::Resource&& __other) noexcept
{
    _M_painter = __other._M_painter;
    _M_object = std::exchange(__other._M_object, nullptr);
}

Painter::Resource&
    Painter::Resource::operator=(Painter::Resource&& __other) noexcept
{
    if (_M_painter == nullptr && _M_object != nullptr)
    {
        DeleteObject(_M_object);
    }

    _M_painter = __other._M_painter;
    _M_object = std::exchange(__other._M_object, nullptr);

    return *this;
}

/// --- In Painter::Pen ---

void Painter::Pen::setData(
    const Painter::Pen::Data& __data) noexcept
{
    destroyObject();

    _M_object = CreatePen(
        __data.style,
        __data.width,
        force_cast<COLORREF>(__data.color));

    onSet();
}

void Painter::Pen::getData(
    Painter::Pen::Data* const __data) const noexcept
{
    LOGPEN logPen;
    GetObject(_M_object, sizeof(logPen), &logPen);

    assign_as(__data->style, logPen.lopnStyle);

    assign_as(__data->width, logPen.lopnWidth.x);

    force_assign(__data->color, logPen.lopnColor);
}

Painter::Pen::Pen(
    Painter::Pen::Style __style,
    std::uint32_t __width,
    Color __color) noexcept

    : Painter::Resource(
        nullptr,
        CreatePen(
            static_cast<int>(__style),
            static_cast<int>(__width),
            force_cast<COLORREF>(__color)))
{ }

void Painter::Pen::setColor(Color __color) noexcept
{
    Data data;
    getData(&data);

    data.color = __color;

    setData(data);
}

Color Painter::Pen::color() const noexcept
{
    Data data;
    getData(&data);

    return data.color;
}

void Painter::Pen::setStyle(
    Painter::Pen::Style __style) noexcept
{
    Data data;
    getData(&data);

    data.style = __style;

    setData(data);
}

Painter::Pen::Style Painter::Pen::style() const noexcept
{
    Data data;
    getData(&data);

    return data.style;
}

void Painter::Pen::setWidth(std::uint32_t __width) noexcept
{
    Data data;
    getData(&data);

    data.width = __width;

    setData(data);
}

std::uint32_t Painter::Pen::width() const noexcept
{
    Data data;
    getData(&data);

    return data.width;
}


/// --- In Painter::Brush ---

void Painter::Brush::setData(
    const Painter::Brush::Data& __data) noexcept
{
    LOGBRUSH logBrush;
    logBrush.lbStyle = __data.style;
    logBrush.lbColor = force_cast<COLORREF>(__data.color);
    logBrush.lbHatch = force_cast<decltype(logBrush.lbHatch)>(__data.info);

    destroyObject();
    _M_object = CreateBrushIndirect(&logBrush);

    onSet();
}

void Painter::Brush::getData(
    Painter::Brush::Data* const __data) const noexcept
{
    LOGBRUSH logBrush;
    GetObject(_M_object, sizeof(logBrush), &logBrush);

    assign_as(__data->style, logBrush.lbStyle);

    force_assign(__data->color, logBrush.lbColor);

    force_assign(__data->info, logBrush.lbHatch);
}

Painter::Brush::Brush(Color __color) noexcept
    : Painter::Resource(
        nullptr,
        CreateSolidBrush(force_cast<COLORREF>(__color)))
{ }

Painter::Brush::Brush(
    Painter::Brush::Style __style,
    Color __color) noexcept
        : Painter::Resource(
            nullptr,
            CreateHatchBrush(__style, force_cast<COLORREF>(__color)))
{ }

Painter::Brush::Brush(Kwargs<"style"_opt, "color"_opt> kwargs) noexcept
    : Painter::Resource(
        nullptr,
        not kwargs["style"_opt].hasValue()
            ? CreateSolidBrush(
                force_cast<COLORREF>(kwargs["color"_opt].valueOr<Color>()))
            : CreateHatchBrush(
                kwargs["style"_opt].valueOr<Style>(),
                force_cast<COLORREF>(kwargs["color"_opt].valueOr<Color>())))
{ }

void Painter::Brush::setStyle(Painter::Brush::Style __style) noexcept
{
    Data data;
    getData(&data);

    data.style = __style;

    setData(data);
}

Painter::Brush::Style
    Painter::Brush::style() const noexcept
{
    Data data;
    getData(&data);

    return data.style;
}

void Painter::Brush::setColor(Color __color) noexcept
{
    Data data;
    getData(&data);

    data.color = __color;

    setData(data);
}

Color Painter::Brush::color() const noexcept
{
    Data data;
    getData(&data);

    return data.color;
}

/// --- In Painter::Font ---

void Painter::Font::setData(const Painter::Font::Data& __data) noexcept
{
    LOGFONTA logFont;
    logFont.lfHeight = __data.size.height();
    logFont.lfWidth  = __data.size.width();

    assign_as(logFont.lfEscapement, __data.escapement * 10);
    assign_as(logFont.lfOrientation, __data.orientation * 10);

    logFont.lfWeight = __data.weight;
    logFont.lfItalic = __data.style & Italic;
    logFont.lfUnderline = __data.style & Underline;
    logFont.lfStrikeOut = __data.style & StrikeOut;
    logFont.lfCharSet = __data.charSet;

    assign_as(logFont.lfOutPrecision, __data.outPrecision);
    assign_as(logFont.lfClipPrecision, __data.clipPrecision);

    logFont.lfQuality = __data.quality;
    logFont.lfPitchAndFamily = __data.pitch;
    logFont.lfPitchAndFamily |=
        static_cast<decltype(logFont.lfPitchAndFamily)>(__data.family) << 2;

    memcpy(logFont.lfFaceName, __data.faceName, faceNameLimit - 1);
    logFont.lfFaceName[faceNameLimit - 1] = '\0';

    destroyObject();
    _M_object = CreateFontIndirectA(&logFont);

    onSet();
}

void Painter::Font::getData(Painter::Font::Data* const __data) const noexcept
{
    LOGFONTA logFont;
    GetObjectA(_M_object, sizeof(logFont), &logFont);

    __data->size = Size(logFont.lfWidth, logFont.lfHeight);
    __data->escapement = logFont.lfEscapement / 10.0f;
    __data->orientation = logFont.lfOrientation / 10.0f;

    assign_as(__data->weight, logFont.lfWeight);

    __data->style =
        (logFont.lfItalic ? Italic : NoStyle) |
        (logFont.lfUnderline ? Underline : NoStyle) |
        (logFont.lfStrikeOut ? StrikeOut : NoStyle);

    assign_as(__data->charSet, logFont.lfCharSet);
    assign_as(__data->outPrecision, logFont.lfOutPrecision);
    assign_as(__data->clipPrecision, logFont.lfClipPrecision);
    assign_as(__data->quality, logFont.lfQuality);
    
    __data->pitch = static_cast<decltype(__data->pitch)>(logFont.lfPitchAndFamily  & 0b000011);
    __data->family = static_cast<decltype(__data->family)>((logFont.lfPitchAndFamily & 0b111100) >> 2);

    memcpy(__data->faceName, logFont.lfFaceName, faceNameLimit - 1);
    __data->faceName[faceNameLimit - 1] = '\0';
}

Painter::Font::Font(
    const Win::String& __faceName,
    int __size,
    Painter::Font::Style __style) noexcept
        : Font(__faceName, Size(0, __size), __style)
{ }

Painter::Font::Font(
    const Win::String& __faceName,
    Size __size,
    Painter::Font::Style __style) noexcept
        : Font({
            { "faceName"_opt, __faceName },
            { "size"_opt, __size },
            { "style"_opt, __style } })
{ }

Painter::Font::Font(Kwargs<
        "size"_opt, /* Or */ "width"_opt, "height"_opt, /* Or */ "w"_opt, "h"_opt,
        "escapement"_opt,
        "orientation"_opt,
        "weight"_opt,
        "style"_opt, /* Or */ "italic"_opt, "underline"_opt, "strikeOut"_opt, /* Or */ "i"_opt, "u"_opt, "s"_opt,
        "charSet"_opt,
        "outPrecision"_opt,
        "clipPrecision"_opt,
        "quality"_opt,
        "pitch"_opt,
        "family"_opt,
        "faceName"_opt, "name"_opt> kwargs) noexcept
    : Painter::Resource::Resource(
        nullptr,
        CreateFontA(
            kwargs["size"_opt].hasValue()
                ? kwargs["size"_opt].valueOr<Size>().h()
                : kwargs["height"_opt or "h"_opt].valueOr<int>(9),
            kwargs["size"_opt].hasValue()
                ? kwargs["size"_opt].valueOr<Size>().w()
                : kwargs["width"_opt or "w"_opt].valueOr<int>(),
            (int)kwargs["escapement"_opt or "orientation"_opt].valueOr<float>() * 10,
            (int)kwargs["orientation"_opt or "escapement"_opt].valueOr<float>() * 10,
            kwargs["weight"_opt].valueOr<Painter::Font::Weight>(Painter::Font::Weight::Normal),
            kwargs["style"_opt].hasValue()
                ? kwargs["style"_opt].valueOr<Painter::Font::Style>(NoStyle) & Italic
                : kwargs["italic"_opt or "i"_opt].valueOr<bool>(),
            kwargs["style"_opt].hasValue()
                ? kwargs["style"_opt].valueOr<Painter::Font::Style>(NoStyle) & Underline
                : kwargs["underline"_opt or "u"_opt].valueOr<bool>(),
            kwargs["style"_opt].hasValue()
                ? kwargs["style"_opt].valueOr<Painter::Font::Style>(NoStyle) & StrikeOut
                : kwargs["strikeOut"_opt or "s"_opt].valueOr<bool>(),
            kwargs["charSet"_opt].valueOr<CharSet>(AnsiCharset),
            (DWORD)kwargs["outPrecision"_opt].valueOr<OutPrecision>(OutPrecision::Default),
            (DWORD)kwargs["clipPrecision"_opt].valueOr<ClipPrecision>(ClipPrecision::Default),
            kwargs["pitch"_opt].valueOr<Pitch>(DefautlPitch),
            kwargs["family"_opt].valueOr<Family>(DontCareFamily),
            kwargs["faceName"_opt or "name"_opt].valueOr<const char*>("")))
{ }

std::vector<std::pair<std::string, Painter::Font::FontType>>
    Painter::Font::list() const noexcept
{
    LOGFONTA logFont;
    logFont.lfCharSet = DEFAULT_CHARSET;
    logFont.lfFaceName[0] = '\0';
    logFont.lfPitchAndFamily = 0;

    std::vector<std::pair<std::string, Painter::Font::FontType>> res;

    HDC hdc = _M_painter ? static_cast<HDC>(_M_painter->handle()) : GetDC(nullptr);

    EnumFontFamiliesExA(
        hdc,
        &logFont,
        static_cast<FONTENUMPROCA>(
            [](const LOGFONTA* info, const TEXTMETRICA*, DWORD type, LPARAM resptr) -> int
            {
                reinterpret_cast<decltype(res)*>(resptr)->
                    emplace_back(info->lfFaceName, static_cast<Painter::Font::FontType>(type));
                return true;
            }),
        reinterpret_cast<LPARAM>(&res),
        0);

    if (_M_painter == nullptr)
    {
        ReleaseDC(nullptr, hdc);
    }

    return res;
}

void Painter::Font::setStyle(Painter::Font::Style __style) noexcept
{
    Data data;
    getData(&data);

    data.style = __style;

    setData(data);
}

Painter::Font::Style Painter::Font::style() const noexcept
{
    Data data;
    getData(&data);

    return data.style;
}

void Painter::Font::setFaceName(const std::string& __faceName) noexcept
{
    Data data;
    getData(&data);

    memcpy(data.faceName, __faceName.data(), faceNameLimit - 1);

    setData(data);
}

std::string Painter::Font::faceName() const noexcept
{
    Data data;
    getData(&data);

    return data.faceName;
}

void Painter::Font::setSize(const Size& __size) noexcept
{
    Data data;
    getData(&data);

    data.size = __size;

    setData(data);
}

void Painter::Font::setSize(int __size) noexcept
{
    Data data;
    getData(&data);

    data.size = Size(0, __size);

    setData(data);
}

Size Painter::Font::size() const noexcept
{
    Data data;
    getData(&data);

    return data.size;
}

void Painter::Font::setWeight(int __weight) noexcept
{
    Data data;
    getData(&data);

    assign_as(data.weight, __weight);

    setData(data);
}

int Painter::Font::weight() const noexcept
{
    Data data;
    getData(&data);

    return data.weight;
}

void Painter::Font::setEscapement(float __escapement) noexcept
{
    Data data;
    getData(&data);

    data.escapement = __escapement;

    setData(data);
}

float Painter::Font::escapement() const noexcept
{
    Data data;
    getData(&data);

    return data.escapement;
}

void Painter::Font::setOrientation(float __orientation) noexcept
{
    Data data;
    getData(&data);

    data.orientation = __orientation;

    setData(data);
}

float Painter::Font::orientation() const noexcept
{
    Data data;
    getData(&data);

    return data.orientation;
}

/// --- In Painter::GeometricPen ---

void Painter::GeometricPen::setData(
    const Painter::GeometricPen::Data& __data) noexcept
{
    LOGBRUSH logBrush;
    logBrush.lbStyle = __data.brush.style;

    force_assign(logBrush.lbColor, __data.brush.color);
    force_assign(logBrush.lbHatch, __data.brush.info);

    _M_object = ExtCreatePen(
        __data.style | PS_GEOMETRIC,
        __data.width,
        &logBrush,
        __data.numEntries,
        force_cast<DWORD*>(__data.styleEntry));

    destroyObject();
    onSet();
}

void Painter::GeometricPen::getData(
    Painter::GeometricPen::Data* const __data) const noexcept
{
    EXTLOGPEN logPen;
    GetObject(_M_object, sizeof(*__data), &logPen);

    __data->style = static_cast<decltype(__data->style)>(
        logPen.elpPenStyle);

    __data->width = logPen.elpWidth;

    __data->brush.style = static_cast<
        decltype(__data->brush.style)>(logPen.elpBrushStyle);

    force_assign(__data->brush.color, logPen.elpColor);

    force_assign(__data->brush.info, logPen.elpHatch);

    __data->numEntries = logPen.elpNumEntries;

    std::copy(
        logPen.elpStyleEntry,
        logPen.elpStyleEntry + logPen.elpNumEntries,
        __data->styleEntry);
}

Painter::GeometricPen::GeometricPen(
    Painter::GeometricPen::Style __style,
    const Painter::Brush::Data& __brush,
    std::uint32_t __width,
    Color __color) noexcept
{
    LOGBRUSH logBrush;
    logBrush.lbStyle = __brush.style;
    logBrush.lbColor = force_cast<decltype(logBrush.lbColor)>(__brush.color);
    logBrush.lbHatch = force_cast<decltype(logBrush.lbHatch)>(__brush.info);

    _M_object = ExtCreatePen(
        PS_GEOMETRIC | __style,
        __width,
        &logBrush,
        0,
        nullptr);
}

template<
    std::size_t _Size,
    std::enable_if_t<tools::is_within_range(_Size, 1ULL, 16ULL), int>>
Painter::GeometricPen::GeometricPen(
        const std::array<std::uint32_t, _Size>& __custom,
        std::uint32_t __width,
        Color __color) noexcept
{
    LOGBRUSH logBrush;
    logBrush.lbStyle = BS_SOLID;
    force_assign(logBrush.lbColor, __color);
    logBrush.lbHatch = 0;

    _M_object = ExtCreatePen(
        PS_GEOMETRIC | PS_USERSTYLE,
        __width,
        &logBrush,
        static_cast<std::uint32_t>(_Size),
        force_cast<const DWORD*>(__custom.data()));
}

#if /* Instantiate all Painter::GeometricPen::GeometricPen */ true

template Painter::GeometricPen::GeometricPen(
    const std::array<std::uint32_t, 1>&, std::uint32_t, Color) noexcept;

template Painter::GeometricPen::GeometricPen(
    const std::array<std::uint32_t, 2>&, std::uint32_t, Color) noexcept;

template Painter::GeometricPen::GeometricPen(
    const std::array<std::uint32_t, 3>&, std::uint32_t, Color) noexcept;

template Painter::GeometricPen::GeometricPen(
    const std::array<std::uint32_t, 4>&, std::uint32_t, Color) noexcept;

template Painter::GeometricPen::GeometricPen(
    const std::array<std::uint32_t, 5>&, std::uint32_t, Color) noexcept;

template Painter::GeometricPen::GeometricPen(
    const std::array<std::uint32_t, 6>&, std::uint32_t, Color) noexcept;

template Painter::GeometricPen::GeometricPen(
    const std::array<std::uint32_t, 7>&, std::uint32_t, Color) noexcept;

template Painter::GeometricPen::GeometricPen(
    const std::array<std::uint32_t, 8>&, std::uint32_t, Color) noexcept;

template Painter::GeometricPen::GeometricPen(
    const std::array<std::uint32_t, 9>&, std::uint32_t, Color) noexcept;

template Painter::GeometricPen::GeometricPen(
    const std::array<std::uint32_t, 10>&, std::uint32_t, Color) noexcept;

template Painter::GeometricPen::GeometricPen(
    const std::array<std::uint32_t, 11>&, std::uint32_t, Color) noexcept;

template Painter::GeometricPen::GeometricPen(
    const std::array<std::uint32_t, 12>&, std::uint32_t, Color) noexcept;

template Painter::GeometricPen::GeometricPen(
    const std::array<std::uint32_t, 13>&, std::uint32_t, Color) noexcept;

template Painter::GeometricPen::GeometricPen(
    const std::array<std::uint32_t, 14>&, std::uint32_t, Color) noexcept;

template Painter::GeometricPen::GeometricPen(
    const std::array<std::uint32_t, 15>&, std::uint32_t, Color) noexcept;

template Painter::GeometricPen::GeometricPen(
    const std::array<std::uint32_t, 16>&, std::uint32_t, Color) noexcept;

#endif

void Painter::GeometricPen::setColor(
    Color __color) noexcept
{
    Data data;
    getData(&data);

    data.brush.color = __color;

    setData(data);
}

Color Painter::GeometricPen::color() const noexcept
{
    Data data;
    getData(&data);

    return data.brush.color;
}

void Painter::GeometricPen::setStyle(
    Painter::GeometricPen::Style __style) noexcept
{
    Data data;
    getData(&data);

    data.style = __style;

    setData(data);
}

Painter::GeometricPen::Style
    Painter::GeometricPen::style() const noexcept
{
    Data data;
    getData(&data);

    return data.style;
}

void Painter::GeometricPen::setBrushData(
    const Painter::Brush::Data& __brush) noexcept
{
    Data data;
    getData(&data);

    data.brush = __brush;

    setData(data);
}

Painter::Brush::Data
    Painter::GeometricPen::brushData() const noexcept
{
    Data data;
    getData(&data);

    return data.brush;
}

void Painter::GeometricPen::setWidth(
    std::uint32_t __width) noexcept
{
    Data data;
    getData(&data);

    data.width = __width;

    setData(data);
}

std::uint32_t Painter::GeometricPen::width() const noexcept
{
    Data data;
    getData(&data);

    return data.width;
}

void Painter::GeometricPen::setMiterLimit(float __limit) noexcept
{
    SetMiterLimit($(_M_painter->handle()), __limit, nullptr);
}

float Painter::GeometricPen::miterLimit() const noexcept
{
    float limit = 10.0f;
    GetMiterLimit($(_M_painter->handle()), &limit);

    return limit;
}