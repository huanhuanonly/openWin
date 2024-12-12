/**
* Cur.cpp In the openWin (https://github.com/huanhuanonly/openWin)
*
* Created by Yang Huanhuan on December 6, 2024, 16:44:56
*
* Email -> 3347484963@qq.com
*
* --- This file is a part of openWin ---
*
* @brief Implement Cur.h
*/

#include "../include/Cur.h"

#include <Windows.h>

float Cur::dpi() noexcept
{
    return _S_dpi;
}

Win::Point Cur::pos() noexcept
{
    POINT point;
    GetCursorPos(&point);

    return Win::Point(
        static_cast<int>(point.x * _S_dpi),
        static_cast<int>(point.y * _S_dpi));
}

int Cur::x() noexcept
{
    return pos().x();
}

int Cur::y() noexcept
{
    return pos().y();
}