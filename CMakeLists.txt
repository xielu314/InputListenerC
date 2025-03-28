cmake_minimum_required(VERSION 3.10)
project(InputListener LANGUAGES C)

set(CMAKE_C_STANDARD 11)

set(SRC_FILES 
    src/InputListenerCore.c
    src/APIs/InputListenerLuaAPIs.c
)

include_directories(
    ${PROJECT_SOURCE_DIR}/include
    "D:/Library/SDL3-3.2.8_x86_64-w64-mingw32/include"
    "D:/Library/lua-5.4.2_Win64_dllw6_lib/include"
)

link_directories(
    "D:/Library/SDL3-3.2.8_x86_64-w64-mingw32/lib"
    "D:/Library/lua-5.4.2_Win64_dllw6_lib"
)

add_library(InputListener SHARED ${SRC_FILES})

set_target_properties(InputListener PROPERTIES PREFIX "")

target_link_libraries(InputListener 
    SDL3
    lua54
)
