add_library(engine STATIC include/engine/test.h
                          src/test.cpp)

target_include_directories(engine PUBLIC include)
