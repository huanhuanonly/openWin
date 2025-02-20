/**
* Copyright (c) 2024-2025 Yang Huanhuan (3347484963@qq.com).
* 
* This software is provided "as is", without warranty of any kind, express or implied.
*/

/**
* Cur.cpp In the openWin (https://github.com/huanhuanonly/openWin)
* 
* Created by Yang Huanhuan on December 6, 2024, 16:44:56
* 
* --- This file is a part of openWin ---
* 
* @brief Implement Cur.h
*/

#include <openWin/Cur.h>

#include "Built-in/_Windows.h"

using namespace win;

float Cur::dpi() noexcept
{
    return Win::currentDesktopWindow().dpi();
}

Point Cur::pos() noexcept
{
    POINT point;
    GetCursorPos(&point);

    return Point(
        static_cast<int>(point.x * dpi()),
        static_cast<int>(point.y * dpi()));
}

int Cur::x() noexcept
{
    return pos().x();
}

int Cur::y() noexcept
{
    return pos().y();
}