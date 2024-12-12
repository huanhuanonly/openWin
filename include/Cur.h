/**
* Cur.h In the openWin (https://github.com/huanhuanonly/openWin)
*
* Created by Yang Huanhuan on December 6, 2024, 16:44:55
*
* Email -> 3347484963@qq.com
* 
* --- This file is a part of openWin ---
*
* @brief Encapsulates a cursor class (Cur) to obtain cursor
*        information or control the cursor.
*/

#pragma once

#ifndef CUR_H
#define CUR_H

#include "Win.h"

class Cur
{
public:

    Cur() = default;

    [[nodiscard]]
    static float dpi() noexcept;

    [[nodiscard]]
    static Win::Point pos() noexcept;

    [[nodiscard]]
    static int x() noexcept;

    [[nodiscard]]
    static int y() noexcept;

private:

    static inline float _S_dpi = Win::fromDesktop().dpi();
};

#endif // CUR_H