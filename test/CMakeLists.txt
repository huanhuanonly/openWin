# ----------------------------------------------------------------------------
# Copyright (c) 2024-2025 Yang Huanhuan (3347484963@qq.com).
# 
# This software is provided "as is", without warranty of any kind, express or implied.
# 
# 
# Test CMake file for openWin (https://github.com/huanhuanonly/openWin)
# 
# Created by Yang Huanhuan on January 19, 2025, 17:39:58
# ----------------------------------------------------------------------------

set (CURRENT_TEST_SOURCE_FILE "keys-get_name.cpp")

add_executable (openWin-test ${CURRENT_TEST_SOURCE_FILE})

set_property (TARGET openWin-test PROPERTY CXX_STANDARD 20)

target_link_libraries (openWin-test openWin)
