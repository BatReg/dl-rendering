if(WIN32)
    add_executable(app WIN32 
                             src/controller.h
                             src/controller.cpp
                             src/fps_counter.h
                             src/fps_counter.cpp
                             src/scene.h
                             src/win32/win32_win.h
                             src/win32/win32_main.cpp)
    
    target_compile_definitions(app PUBLIC UNICODE _UNICODE)
endif()

target_include_directories(app PUBLIC include)

target_link_libraries(app engine)
