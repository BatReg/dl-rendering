if(WIN32)
    add_executable(app WIN32 src/win32/win32_min.h
                             src/win32/win32_main.cpp)
endif()

target_link_libraries(app engine)
