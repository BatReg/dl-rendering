#pragma once

#if ENGINE_WIN32
    #include "win32/win32_platform.h"
    #define WINDOW_NATIVE_STATE Win32WindowState win32;
#else
    #define WINDOW_NATIVE_STATE
#endif
