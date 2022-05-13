add_library(engine STATIC include/engine/core.h
                          include/engine/window.h
                          src/int_window.h
                          src/platform.h
                          src/window.cpp)

if(WIN32)
    target_sources(engine PRIVATE src/win32/win32_core.h
                                  src/win32/win32_platform.h
                                  src/win32/win32_window.cpp)

    target_compile_definitions(engine PUBLIC UNICODE _UNICODE ENGINE_WIN32)
endif()

target_include_directories(engine PUBLIC include)
