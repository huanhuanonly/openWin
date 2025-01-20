/**
* Copyright (c) 2024-2025 Yang Huanhuan (3347484963@qq.com).
* 
* This software is provided "as is", without warranty of any kind, express or implied.
*/

/**
* Painter.h In the openWin (https://github.com/huanhuanonly/openWin)
* 
* Created by Yang Huanhuan on January 13, 2025, 20:05:30
* 
* --- This file is a part of openWin ---
* 
* @brief Encapsulates a painter class (Painter) used to draw patterns in the window or screen.
*/

#pragma once

#ifndef OPENWIN_HEADER_PAINTER_H
#define OPENWIN_HEADER_PAINTER_H

#include "Win.h"
#include "Tools.h"

#include <memory>

#include <CppKwargs.h>

namespace win
{

class Painter
{
public:

    using Handle = void*;

    class Resource;

    class Pen;
    class Brush;
    class Font;

    class GeometricPen;

    enum HorizontalAlignment : std::uint32_t
    {
        LeftAlign    = 0x00,
        HCenterAlign = 0x01,
        RightAlign   = 0x02
    };

    enum VerticalAlignment : std::int32_t
    {
        TopAlign     = 0x00,
        VCenterAlign = 0x04,
        BottomAlign  = 0x08
    };

    struct Alignment
    {
        HorizontalAlignment horizontalAlign;
        VerticalAlignment   verticalAlign;

        Alignment() noexcept
            : horizontalAlign(LeftAlign), verticalAlign(TopAlign)
        { }

        Alignment(HorizontalAlignment __h, VerticalAlignment __v) noexcept
            : horizontalAlign(__h), verticalAlign(__v)
        { }

        Alignment(VerticalAlignment __v, HorizontalAlignment __h) noexcept
            : horizontalAlign(__h), verticalAlign(__v)
        { }

        friend inline Alignment operator+(HorizontalAlignment __h, VerticalAlignment __v) noexcept
        { return Alignment(__h, __v); }

        friend inline Alignment operator+(VerticalAlignment __v, HorizontalAlignment __h) noexcept
        { return Alignment(__h, __v); }

        friend inline Alignment operator|(HorizontalAlignment __h, VerticalAlignment __v) noexcept
        { return Alignment(__h, __v); }

        friend inline Alignment operator|(VerticalAlignment __v, HorizontalAlignment __h) noexcept
        { return Alignment(__h, __v); }

        friend inline Alignment operator&(HorizontalAlignment __h, VerticalAlignment __v) noexcept
        { return Alignment(__h, __v); }

        friend inline Alignment operator&(VerticalAlignment __v, HorizontalAlignment __h) noexcept
        { return Alignment(__h, __v); }
    };

    Painter() noexcept;

    explicit Painter(const Win& __win) noexcept;
    explicit Painter(Handle __hdc) noexcept;

    Painter(std::nullptr_t) noexcept;

    Painter(const Painter& __other) noexcept;
    Painter(Painter&& __other) noexcept;

    ~Painter() noexcept;

    Painter& operator=(const Painter& __other) noexcept;
    Painter& operator=(Painter&& __other) noexcept;

    [[nodiscard]] Win window() const noexcept;

    void setHandle(Handle __handle) noexcept;
    [[nodiscard]] Handle handle() const noexcept;

    /**
    * @warning If this painter becomes invalid, the return
    *          values of pen(), brush() and font() will
    *          also become invalid.
    */
    [[nodiscard]] std::unique_ptr<Pen> pen() noexcept;
    [[nodiscard]] std::unique_ptr<Brush> brush() noexcept;
    [[nodiscard]] std::unique_ptr<Font> font() noexcept;


    void setColorUnder(
        const Point& __point,
        const Color& __color) const noexcept;

    [[nodiscard]] Color colorUnder(const Point& __point) const noexcept;

    /**
    * @brief Set text foreground color.
    */
    void setForegroundColor(const Color& __color) const noexcept;
    void setBackgroundColor(const Color& __color) const noexcept;

    void setBackgroundTransparent(bool __enable = true) const noexcept;

    [[nodiscard]] Color foregroundColor() const noexcept;
    [[nodiscard]] Color backgroundColor() const noexcept;

    [[nodiscard]] bool backgroundTransparent() const noexcept;


    /**
    * +------------------------------+
    * | Operand |       Meaning      |
    * +---------+--------------------+
    * | P       | Selected pen       |
    * | D       | Destination bitmap |
    * +---------+--------------------+
    *
    * +------------------------------+
    * | Operator |       Meaning     |
    * +----------+-------------------+
    * | a        | Bitwise  (&)  AND |
    * | n        | Bitwise  (~)  NOT |
    * | o        | Bitwise  (|)   OR |
    * | x        | Bitwise  (^)  XOR |
    * +----------+-------------------+
    *
    * reverse Polish notation
    */
    enum class BlendingModes : std::uint32_t
    {
        ZERO = 1,  // 0
        DPon,      // NOT(D OR P)
        DPna,      // D AND NOT(P)
        Pn,        // NOT(P)
        PDna,      // P AND NOT(D)
        Dn,        // NOT(D)
        DPx,       // D XOR P
        DPan,      // D AND NOT(P)
        DPa,       // D AND P
        DPxn,      // D XOR NOT(P)
        D,         // D
        DPno,      // D OR NOT(P)
        P,         // P
        PDno,      // D OR NOT(D)
        DPo,       // D OR P
        ONE        // 1
    };

    void setBlendingMode(BlendingModes __bm) const noexcept;
    [[nodiscard]] BlendingModes blendingMode() const noexcept;

    void drawText(
        const Win::String& __text,
        const Point& __point) const noexcept;

    void drawText(
        const Win::WString& __text,
        const Point& __point) const noexcept;

    void drawText(
        const Win::String& __text,
        const Rect& __rect,
        Alignment __align = Alignment(),
        bool __singleLine = false) const noexcept;

    void drawText(
        const Win::WString& __text,
        const Rect& __rect,
        Alignment __align = Alignment(),
        bool __singleLine = false) const noexcept;

protected:

    void _M_setCurrentPosition(const Point& __point) const noexcept;
    [[nodiscard]] Point _M_currentPosition() const noexcept;

    void _M_drawLineTo(const Point& __point) const noexcept;

public:

    void drawLine(
        const Point& __start,
        const Point& __end) const noexcept;

    void drawLines(const Point* __points, int __size) const noexcept;

    template<int _Size>
    inline void drawLines(const Point (&__points)[_Size]) const noexcept
    { drawLines(__points, _Size); }

    template<typename _Container>
    void drawLines(
        const std::enable_if_t<
            std::is_same_v<
                std::remove_const_t<
                    std::remove_reference_t<
                        decltype(std::declval<_Container>().data())>>,
            Point>, _Container>& __c) const noexcept
    { drawLines(__c.data(), static_cast<int>(__c.size())); }

    template<typename _Container>
    void drawLines(
        const std::enable_if_t<
            std::is_same_v<
                void,
                std::void_t<
                    decltype(std::declval<_Container>().begin()),
                    decltype(std::declval<_Container>().end())>>,
            _Container>& __c) const noexcept
    { drawLines(__c.begin(), __c.end()); }

    template<
        typename _ForwardIterator,
        typename = std::enable_if_t<
            std::is_same_v<
                std::remove_const_t<
                    std::remove_reference_t<
                        decltype(*std::declval<_ForwardIterator>())>>,
                Point>>>
    inline void drawLines(_ForwardIterator __first, _ForwardIterator __last) const noexcept
    {
        if (__first == __last)
        {
            return;
        }

        _M_setCurrentPosition(*__first);

        for (++__first; __first != __last; ++__first)
        {
            _M_drawLineTo(*__first);
        }
    }

    void drawPolygon(const Point* __vertexes, int __size) const noexcept;

    void drawRect(const Rect& __rect) const noexcept;

    void drawRect(const Rect& __rect, const Size& __round) const noexcept;

    void drawCircle(
        const Point& __center,
        int __radius,
        float __startAngle = 0,
        float __sweepAngle = 360) const noexcept;

    void drawCircle(
        const Point& __point1,
        const Point& __point2,
        float __startAngle = 0,
        float __sweepAngle = 360) const noexcept;

    void drawEllipse(const Rect& __rect) const noexcept;

    void drawArc(
        const Rect& __rect,
        const Point& __start,
        const Point& __end) const noexcept;

    void drawChord(
        const Rect& __rect,
        const Point& __start,
        const Point& __end) const noexcept;

    void drawPie(
        const Rect& __rect,
        const Point& __start,
        const Point& __end) const noexcept;

protected:

    void _M_drawPolyBezier(
        const Point* __points,
        int __size) const noexcept;

public:


    /**
    * @brief Draw a cubic Bezier-curve.
    * 
    * See also https://en.wikipedia.org/wiki/B%C3%A9zier_curve
    * for more detail.
    */
    template<
        typename _ForwardIterator,
        typename = std::enable_if_t<
            std::is_same_v<
                std::remove_const_t<
                    std::remove_reference_t<
                        decltype(*std::declval<_ForwardIterator>())>>,
                BezierVertex>>>
    inline void drawPolyBezier(
        _ForwardIterator __first,
        _ForwardIterator __last,
        std::size_t __size = 0) const noexcept
    {
        std::vector<Point> v;

        if (__size > 0)
        {
            v.reserve(__size * 3);
        }

        if (__first == __last)
        {
            return;
        }

        v.push_back(__first->vertex);
        v.push_back(__first->backwardControl);

        for (++__first; __first != __last; ++__first)
        {
            v.push_back(__first->forwardControl);
            v.push_back(__first->vertex);
            v.push_back(__first->backwardControl);
        }

        v.pop_back();
        _M_drawPolyBezier(v.data(), v.size());
    }

    template<
        typename _Container,
        typename = std::enable_if_t<
            std::is_same_v<typename _Container::value_type, BezierVertex>,
            std::void_t<
                decltype(std::declval<_Container>().begin()),
                decltype(std::declval<_Container>().end()),
                decltype(std::declval<_Container>().size())>>>
    inline void drawPolyBezier(const _Container& __points) const noexcept
    { drawPolyBezier(__points.begin(), __points.end(), __points.size()); }


    void invert(const Rect& __rect) const noexcept;

private:

    Handle _M_dc;
    float _M_dpi;
};


class Painter::Resource
{
public:

    using Object = void*;

    struct Data { };

    void setData(const Data& __data) noexcept { }
    void getData(Data* const __data) const noexcept { }

    [[nodiscard]] Data data() const noexcept
    {
        Data _data;
        getData(&_data);
        return _data;
    }

    explicit Resource(
        Painter* __painter = nullptr,
        Object __object = nullptr) noexcept;

    ~Resource() noexcept;

    [[nodiscard]] Painter* painter() noexcept;
    [[nodiscard]] const Painter* painter() const noexcept;

    [[nodiscard]] Object object() const noexcept;

    void destroyObject() noexcept;

    /**
    * @brief Called after setup is complete.
    */
    void onSet() const noexcept;

    Resource(const Resource&) = delete;
    Resource& operator=(const Resource&) = delete;

    Resource(Resource&& __other) noexcept;
    Resource& operator=(Resource&& __other) noexcept;

protected:

    Painter* _M_painter;
    Object _M_object;
};

class Painter::Pen : public Painter::Resource
{
public:

    using Resource::Resource;

    enum Style : std::uint32_t
    {
        /**
        * The pen is solid.
        */
        Solid = 0,

        /**
        * -------
        * The pen is dashed. This style is valid only when
        * the pen width is one or less in device units.
        * -------
        */
        Dash,

        /**
        * .......
        * The pen is dotted. This style is valid only when
        * the pen width is one or less in device units.
        * .......
        */
        Dot,

        /**
        * _._._._
        * The pen has alternating dashes and dots. This
        * style is valid only when the pen width is one or
        * less in device units.
        * _._._._
        */
        DashDot,

        /**
        * _.._.._
        * The pen has alternating dashes and double dots.
        * This style is valid only when the pen width is
        * one or less in device units.
        * _.._.._
        */
        DashDotDot,

        /**
        * The pen is invisible.
        */
        Null,

        /**
        * The pen is solid. When this pen is used in any
        * GDI drawing function that takes a bounding
        * rectangle, the dimensions of the figure are
        * shrunk so that it fits entirely in the bounding
        * rectangle, taking into account the width of the
        * pen. This applies only to geometric pens.
        */
        InsideFrame
    };

    struct Data : Painter::Resource::Data
    {
        Style          style = Solid;
        std::uint32_t  width = 1;
        Color          color = Color();
    };

    void setData(const Data& __data) noexcept;
    void getData(Data* const __data) const noexcept;

    [[nodiscard]] Data data() const noexcept
    {
        Data _data;
        getData(&_data);
        return _data;
    }

    explicit Pen(
        Style __style = Solid,
        std::uint32_t __width = 1,
        Color __color = Color()) noexcept;

    Pen(Kwargs<"style"_opt, "width"_opt, "color"_opt> kwargs) noexcept
        : Pen(
            kwargs["style"_opt].valueOr<Style>(Solid),
            kwargs["width"_opt].valueOr<std::uint32_t>(1),
            kwargs["color"_opt].valueOr<Color>())
    { }

    virtual void setColor(Color __color) noexcept;
    
    [[nodiscard]]
    virtual Color color() const noexcept;

    virtual void setStyle(Style __style) noexcept;
    
    [[nodiscard]]
    virtual Style style() const noexcept;

    virtual void setWidth(std::uint32_t __width) noexcept;

    [[nodiscard]]
    virtual std::uint32_t width() const noexcept;
};

class Painter::Brush : public Painter::Resource
{
public:

    using Resource::Resource;

    enum Style
    {
        Solid = 0,
        Null,
        Hollow = Null,
        Hatched,
        Pattern,
        Indexed,
        DibPattern,
        DibPatternPt,
        Pattern8x8,
        DibPattern8x8,
        MonoPattern
    };

    /**
    * @see also https://learn.microsoft.com/en-us/windows/win32/gdi/images/penhatch.png
    */
    enum HatchStyle : std::uint32_t
    {
        /// Horizontal hatch.
        /// -----
        HatchHorizontal = 0,

        /// Vertical hatch.
        /// |||||
        HatchVertical,

        /// A 45-degree downward, left-to-right hatch.
        /// \\\\\ 
        HatchForwardDiagonal,

        /// A 45-degree upward, left-to-right hatch.
        /// /////
        HatchBackwardDiagonal,

        /// Horizontal and vertical cross-hatch.
        /// +++++
        HatchCross,

        /// 45-degree crosshatch.
        /// xxxxx
        HatchDiagonalCross
    };

    struct Data : Painter::Resource::Data
    {
        Style style = Solid;

        // - If style is the Pattern, it's ignored;
        // - If style is Hatched or Solid, it's as color;
        Color color;

        union Info
        {
            // If style is Hatched.
            std::uint32_t hatch;

            // If style is Pattern.
            void* bitmap = nullptr;
        } info;
    };

    void setData(const Data& __data) noexcept;
    void getData(Data* const __data) const noexcept;

    [[nodiscard]] Data data() const noexcept
    {
        Data _data;
        getData(&_data);
        return _data;
    }

    explicit Brush(Color __color = Color()) noexcept;
    Brush(Style __style, Color __color) noexcept;

    Brush(Kwargs<"style"_opt, "color"_opt> kwargs) noexcept;

    void setStyle(Style __style) noexcept;
    [[nodiscard]] Style style() const noexcept;

    void setColor(Color __color) noexcept;
    [[nodiscard]] Color color() const noexcept;
};

class Painter::Font : public Painter::Resource
{
public:

    using Resource::Resource;

    enum Style : std::uint8_t
    {
        NoStyle   = 0b0000,
        Italic    = 0b0001,
        Underline = 0b0010,
        StrikeOut = 0b0100
    };

    friend Style operator|(Style __first, Style __second) noexcept
    { return static_cast<Style>(static_cast<std::uint8_t>(__first) | static_cast<std::uint8_t>(__second)); }

    friend Style operator&&(Style __first, Style __second) noexcept
    { return __first | __second; }

    enum Weight : std::int16_t
    {
        DontCare   = 000,
        Thin       = 100,
        Extralight = 200,
        Light      = 300,
        Normal     = 400,
        Medium     = 500,
        Semibold   = 600,
        Bold       = 700,
        Extrabold  = 800,
        Heavy      = 900,
        OverWeight = 1000,

        Ultralight = Extralight,
        Regular    = Normal,
        Demibold   = Semibold,
        Ultrabold  = Extrabold,
        Black      = Heavy
    };

    enum CharSet : std::uint8_t
    {
        AnsiCharset        = 0,
        DefaultCharset     = 1,
        SymbolCharset      = 2,
        ShiftjisCharset    = 128,
        HangeulCharset     = 129,
        HangulCharset      = 129,
        Gb2312Charset      = 134,
        Chinesebig5Charset = 136,
        OemCharset         = 255,
        JohabCharset       = 130,
        HebrewCharset      = 177,
        ArabicCharset      = 178,
        GreekCharset       = 161,
        TurkishCharset     = 162,
        VietnameseCharset  = 163,
        ThaiCharset        = 222,
        EasteuropeCharset  = 238,
        RussianCharset     = 204,
        MacCharset         = 77,
        BalticCharset      = 186
    };

    enum class OutPrecision : std::uint8_t
    {
        /// Default value; uses the system's default font
        /// selection method.
        Default = 0,

        /// Reserved value; not recommended for use.
        String,

        /// Used for matching the character set of the font.
        Character,

        /// Reserved value; not recommended for use.
        Stroke,

        /// Forces selection of TrueType fonts. If no
        /// TrueType fonts are found, a default font is
        /// chosen.
        TrueType,

        /// Used for selecting device fonts.
        Device,

        /// Used for selecting raster fonts.
        Raster,

        /// Forces selection of TrueType fonts. If no
        /// TrueType fonts are found, no other font is
        /// chosen.
        TrueTypeOnly,

        /// Forces selection of outline fonts. If no
        /// outline fonts (e.g., TrueType or OpenType) are
        /// available, a default font is chosen.
        Outline,

        /// Supports outline fonts for screen display,
        /// typically used with ClearType technology.
        ScreenOutline,

        /// Forces selection of PostScript fonts. If no
        /// PostScript fonts are found, no other font is
        /// chosen.
        PostScriptOnly
    };

    enum class ClipPrecision : std::uint8_t
    {
        Default    = 0b00000000,
        Character  = 0b00000001,
        Stroke     = 0b00000010,
        LhAngles   = 0b00010000,
        TtAlways   = 0b00100000,
        DfaDisable = 0b01000000,
        Embedded   = 0b10000000
    };

    friend ClipPrecision operator|(ClipPrecision __first, ClipPrecision __second) noexcept
    { return static_cast<ClipPrecision>(static_cast<std::uint8_t>(__first) | static_cast<std::uint8_t>(__second)); }

    friend ClipPrecision operator&&(ClipPrecision __first, ClipPrecision __second) noexcept
    { return __first | __second; }

    enum Quality : std::uint8_t
    {
        /**
         * Description:
         *     This is the default output quality option,
         *     typically determined automatically by the
         *     operating system or printer driver. It
         *     strikes a balance between performance and
         *     display quality, suitable for most purposes.
         * 
         * Use case:
         *     When you don’t need specially optimized font
         *     rendering or printing quality, use the
         *     default value.
         */
        DefaultQuality = 0,

        /**
         * Description:
         *     This quality level is typically used for
         *     draft printing or quick rendering,
         *     sacrificing font rendering precision for
         *     faster rendering speed. It’s often used for
         *     scenarios that don't require high-quality
         *     printing output.
         * 
         * Use case:
         *     For draft printing, document testing, or
         *     situations where fast font rendering is more
         *     important than quality.
         */
        Draft,

        /**
         * Description:
         *     This quality level is used to ensure font
         *     rendering accuracy and consistency, suitable
         *     for high-quality printing output or precise
         *     display. It provides high-precision glyph
         *     rendering, typically used in printing or
         *     high-accuracy scenarios.
         * 
         * Use case:
         *     For professional publishing, printing, or
         *     any output requiring high precision.
         */
        Proof,

        /**
         * Description:
         *     This option disables anti-aliasing, meaning
         *     no font smoothing effects will be applied.
         *     Fonts will have jagged edges, but rendering
         *     speed will be faster. It’s suitable for
         *     displays that don't require smooth effects.
         * 
         * Use case:
         *     For low-resolution displays or simple
         *     graphics applications, or for systems where
         *     anti-aliasing is not needed (e.g., some
         *     low-level embedded systems or older devices).
         */
        NonAntialiased,

        /**
         * Description:
         *     Enables anti-aliasing, applying smoother
         *     font edges. This is suitable for cases where
         *     high-quality, smooth font rendering is
         *     required, especially on high-resolution
         *     screens. Anti-aliasing will make the edges
         *     of the fonts smoother and reduce jaggedness.
         * 
         * Use case:
         *     For applications with higher-quality
         *     graphics, particularly for modern displays
         *     or where clear, smooth text is needed.
         */
        Antialiased,

        /**
         * Description:
         *     This quality option enables the ClearType
         *     technology, which is Microsoft's font
         *     rendering technology optimized for LCD
         *     screens. It provides extremely smooth text
         *     rendering, particularly on RGB screens.
         *     ClearType makes text clearer and easier to
         *     read.
         * 
         * Use case:
         *     For applications on LCD displays, especially
         *     when displaying small text, offering the
         *     clearest visual experience.
         */
        ClearType,

        /**
         * Description:
         *     Building upon ClearType technology, it
         *     further optimizes character shapes to make
         *     them closer to natural handwriting. It is
         *     suitable for high-resolution screens,
         *     offering the best display quality, though it
         *     may come with a slight performance overhead.
         * 
         * Use case:
         *     For applications where text clarity and
         *     readability are critical, especially on
         *     high-resolution displays.
         */
        ClearTypeNatural
    };

    enum Pitch : std::uint8_t
    {
        /// Default pitch; the system automatically selects
        /// the appropriate font pitch.
        DefautlPitch = 0,

        /// Fixed-pitch fonts, where all characters have
        /// the same width (e.g., Courier New).
        FixedPitch,

        /// Variable-pitch fonts, where character widths
        /// may vary (e.g., Arial, Times New Roman).
        VariablePitch
    };

    enum Family : std::uint8_t
    {
        /// No specific font family; the system selects a
        /// suitable font.
        DontCareFamily = 0,

        /// Roman fonts, typically serif fonts with
        /// variable stroke widths (e.g., Times New Roman).
        RomanFamily,

        /// Swiss fonts, typically sans-serif fonts with
        /// even stroke widths (e.g., Arial).
        SwissFamily,

        /// Modern fonts, typically fixed-pitch fonts with
        /// uniform stroke widths (e.g., Courier New).
        ModernFamily,

        /// Script fonts, resembling handwriting or cursive
        /// (e.g., Comic Sans MS).
        ScripyFamily,

        /// Decorative fonts, used for titles or artistic
        /// design (e.g., Wingdings).
        DecorativeFamily
    };

    enum FontType : std::uint32_t
    {
        Device   = 0x0001,
        Raster   = 0x0002,
        TrueType = 0x0004
    };

    static constexpr inline std::size_t faceNameLimit = 32;

    struct Data : Painter::Resource::Data
    {
        Size size;
        float escapement  = 0.0f;
        float orientation = 0.0f;
        Weight weight = Normal;

        Style style = NoStyle;

        CharSet charSet = AnsiCharset;

        OutPrecision  outPrecision = OutPrecision::Default;
        ClipPrecision clipPrecision = ClipPrecision::Default;
        Quality       quality = DefaultQuality;

#if true
        Pitch  pitch  : 2 = DefautlPitch;
        Family family : 4 = DontCareFamily;
#else
        std::uint8_t pitch  : 2 = DefautlPitch;
        std::uint8_t family : 4 = DontCareFamily;
#endif

        char faceName[faceNameLimit] = "";
    };

    static_assert(sizeof(Data) == 56, "Pitch and Family cannot be placed in the same field!");

    void setData(const Data& __data) noexcept;
    void getData(Data* const __data) const noexcept;

    [[nodiscard]] Data data() const noexcept
    {
        Data _data;
        getData(&_data);
        return _data;
    }

    Font(const Win::String& __faceName, int __size, Style __style = NoStyle) noexcept;
    Font(const Win::String& __faceName, Size __size, Style __style = NoStyle) noexcept;

    Font(Kwargs<
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
        "faceName"_opt, "name"_opt> kwargs = { }) noexcept;

    [[nodiscard]]
    std::vector<std::pair<std::string, FontType>>
        list() const noexcept;

    void setStyle(Style __style) noexcept;
    [[nodiscard]] Style style() const noexcept;

    void setFaceName(const std::string& __faceName) noexcept;
    [[nodiscard]] std::string faceName() const noexcept;

    void setSize(const Size& __size) noexcept;
    void setSize(int __size) noexcept;

    inline void setSize(int __width, int __height) noexcept
    { setSize(Size(__width, __height)); }

    [[nodiscard]] Size size() const noexcept;

    void setWeight(int __weight) noexcept;

    inline void setWeight(Weight __weight) noexcept
    { setWeight(static_cast<int>(__weight)); }

    [[nodiscard]] int weight() const noexcept;

    void setEscapement(float __escapement) noexcept;
    [[nodiscard]] float escapement() const noexcept;

    void setOrientation(float __orientation) noexcept;
    [[nodiscard]] float orientation() const noexcept;
};


class Painter::GeometricPen : public Painter::Pen
{
public:

    using Pen::Pen;
    
    /**
    * @note Style, EndcapStyle and JoinStyle can be
    *       combined with OR (|).
    */

    using Pen::Style;

    /**
    * @see also https://learn.microsoft.com/en-us/windows/win32/gdi/images/cspen-04.png
    */
    enum EndcapStyle : std::uint32_t
    {
        /// End caps are round.
        EndcapRound = 0x00000000,

        /// End caps are square.
        EndcapSquare = 0x00000100,

        /// End caps are flat.
        EndcapFlat   = 0x00000200
    };

    /**
    * @see also https://learn.microsoft.com/en-us/windows/win32/gdi/images/cspen-05.png
    */
    enum JoinStyle : std::uint32_t
    {
        /// Joins are round.
        JoinRound = 0x00000000,

        /// Joins are beveled.
        JoinBevel = 0x00001000,

        /// Joins are mitered when they are within the
        /// current limit set by the setMiterLimit()
        /// function. If it exceeds this limit, the join is
        /// beveled.
        JoinMiter = 0x00002000
    };

    struct Data : Painter::Resource::Data
    {
        using BrushData = Painter::Brush::Data;

        Style           style = Solid;
        std::uint32_t   width = 1;
        
        BrushData       brush;

        std::uint32_t   numEntries = 0;
        std::uint32_t   styleEntry[16] = {};
    };

    void setData(const Data& __data) noexcept;
    void getData(Data* const __data) const noexcept;

    [[nodiscard]] Data data() const noexcept
    {
        Data _data;
        getData(&_data);
        return _data;
    }

    GeometricPen(Style, int, Color) = delete;

    explicit GeometricPen(
        Style __style = Solid,
        const Painter::Brush::Data& __brush = Painter::Brush::Data(),
        std::uint32_t __width = 1,
        Color __color = Color()) noexcept;

    template<
        std::size_t _Size,
        std::enable_if_t<tools::is_within_range(_Size, 1ULL, 16ULL), int> = 0>
    explicit GeometricPen(
        const std::array<std::uint32_t, _Size>& __custom,
        std::uint32_t __width = 1,
        Color __color = Color()) noexcept;


    virtual void setColor(Color __color) noexcept override;

    [[nodiscard]]
    virtual Color color() const noexcept override;

    virtual void setStyle(Style __style) noexcept override;

    [[nodiscard]]
    virtual Style style() const noexcept override;

    void setBrushData(
        const Painter::Brush::Data& __brush) noexcept;

    [[nodiscard]]
    Painter::Brush::Data brushData() const noexcept;

    virtual void setWidth(std::uint32_t __width) noexcept override;

    [[nodiscard]]
    virtual std::uint32_t width() const noexcept override;


    /**
    * @brief The miter length is defined as the distance
    *        from the intersection of the line walls on the
    *        inside of the join to the intersection of the
    *        line walls on the outside of the join. The
    *        miter limit is the maximum allowed ratio of
    *        the miter length to the line width.
    * 
    * @note  The default miter limit is 10.0.
    * 
    *        Setting __limit to a float value less than
    *        1.0f will cause the function to fail.
    */
    void setMiterLimit(float __limit) noexcept;

    [[nodiscard]]
    float miterLimit() const noexcept;
};


/// TODO: class Painter::Bitmap

}  // namespace win

#endif  // OPENWIN_HEADER_PAINTER_H