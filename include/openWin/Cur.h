/**
* Copyright (c) 2024-2025 Yang Huanhuan (3347484963@qq.com).
* 
* This software is provided "as is", without warranty of any kind, express or implied.
*/

/**
* Cur.h In the openWin (https://github.com/huanhuanonly/openWin)
* 
* Created by Yang Huanhuan on December 6, 2024, 16:44:55
* 
* --- This file is a part of openWin ---
* 
* @brief Encapsulates a cursor class (Cur) to obtain cursor information or control the cursor.
*/

#pragma once

#ifndef OPENWIN_HEADER_CUR_H
#define OPENWIN_HEADER_CUR_H

#include "Win.h"

namespace win
{

class Cur
{
public:

    Cur() = default;

    [[nodiscard]]
    static float dpi() noexcept;

    [[nodiscard]]
    static Point pos() noexcept;

    [[nodiscard]]
    static int x() noexcept;

    [[nodiscard]]
    static int y() noexcept;

private:

    static inline float _S_dpi = Win::fromDesktop().dpi();
};

}  // namespace win

#endif // OPENWIN_HEADER_CUR_H