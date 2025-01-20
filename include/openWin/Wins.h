/**
* Copyright (c) 2024-2025 Yang Huanhuan (3347484963@qq.com).
* 
* This software is provided "as is", without warranty of any kind, express or implied.
*/

/**
* Wins.h In the openWin (https://github.com/huanhuanonly/openWin)
* 
* Created by Yang Huanhuan on January 18, 2025, 19:13:33
* 
* --- This file is a part of openWin ---
* 
* @brief Define a wrapper for maintaining multiple window classes.
*/

#pragma once

#ifndef OPENWIN_HEADER_WINS_H
#define OPENWIN_HEADER_WINS_H

#include <vector>

namespace win
{

class Win;

template<typename _Base>
class Wins : public _Base
{
public:

    static_assert(
        std::is_base_of_v<Win, typename _Base::value_type>,
        "_Base::value_type must be derived from Win.");
    
    using _Base::_Base;
};

using WinList = Wins<std::vector<Win>>;

}  // namespace win

#endif  // OPENWIN_HEADER_WINS_H