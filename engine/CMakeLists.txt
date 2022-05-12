add_library(engine STATIC include/engine/window.h)

if(WIN32)
    target_sources(engine PRIVATE src/win32/win32_core.h
                                  src/win32/win32_window.cpp)

    target_compile_definitions(engine PUBLIC UNICODE _UNICODE)
endif()

target_include_directories(engine PUBLIC include)
