/**
* PathGenerator.cpp In the openWin (https://github.com/huanhuanonly/openWin)
*
* Created by Yang Huanhuan on December 6, 2024, 16:44:56
*
* Email -> 3347484963@qq.com
*
* --- This file is a part of openWin ---
*
* @brief Implement PathGenerator.h
*/

#include "../include/PathGenerator.h"

#include <Windows.h>

namespace _PathGenerator_
{
	void wait(std::uint32_t __timeout) noexcept
	{
		if (__timeout)
		{
			Sleep(__timeout);
		}
	}
}