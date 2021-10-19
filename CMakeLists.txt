cmake_minimum_required(VERSION 3.9)
project(RNGDungeon DESCRIPTION "RNGDungeon executable and libraries")

set(PROJECT_ROOT ./)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include(GNUInstallDirs)
include(FetchContent)

##############################
# ImGui lib
##############################

set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/imgui)
add_library(imgui STATIC)

target_sources(imgui
    PRIVATE
        ${IMGUI_DIR}/imgui_demo.cpp
        ${IMGUI_DIR}/imgui_draw.cpp
        ${IMGUI_DIR}/imgui_tables.cpp
        ${IMGUI_DIR}/imgui_widgets.cpp
        ${IMGUI_DIR}/imgui.cpp
)

target_include_directories(imgui PUBLIC ${IMGUI_DIR})


##############################
# RNGDungeon executable
##############################
set(SRC_FILES
    ${PROJECT_ROOT}/src/main.cpp
    ${PROJECT_ROOT}/src/GenCellSystem.cpp
    ${PROJECT_ROOT}/src/Generator.cpp
    ${PROJECT_ROOT}/lib/Lib.cpp
    ${PROJECT_ROOT}/lib/std/DLLoader.cpp
    ${PROJECT_ROOT}/lib/std/Exception.cpp
    ${PROJECT_ROOT}/lib/std/Stopwatch.cpp
    ${PROJECT_ROOT}/triangulate.cpp
)
set(RLIMGUI_SRC
    ${PROJECT_ROOT}/rlImGui/rlImGui.cpp
)

add_executable(RNGDungeon
    ${SRC_FILES}
    ${RLIMGUI_SRC}
)
target_include_directories(RNGDungeon
    PRIVATE
        ${PROJECT_ROOT}/include/
        ${PROJECT_ROOT}/rlImGui/
        ${IMGUI_DIR}
)

if (MSVC)
    target_compile_options(RNGDungeon PRIVATE "/ZM" "/Wall" "/std:c++17")
    set_target_properties(RNGDungeon PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "../../"
    )
else()
    target_compile_options(RNGDungeon PRIVATE -W -Wall -Wextra -Wno-missing-field-initializers -g3)
    set_target_properties(RNGDungeon PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "../"
    )
endif()

target_link_libraries(RNGDungeon imgui)
target_link_libraries(RNGDungeon raylib)
if (NOT raylib_FOUND)
    include(FetchContent)
    FetchContent_Declare(raylib URL https://github.com/raysan5/raylib/archive/master.tar.gz)
    FetchContent_GetProperties(raylib)
    if (NOT raylib_POPULATED)
        set(FETCHCONTENT_QUIET NO)
        FetchContent_Populate(raylib)
        set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
        add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})
        set(raylib_FOUND TRUE)
    endif()
endif()

# TARGET_LINK_LIBRARIES(RNGDungeon ImGui)
# FetchContent_Declare(
#     ImGui
#     GIT_REPOSITORY https://github.com/ocornut/imgui.git
#     GIT_TAG master
# )
# FetchContent_MakeAvailable(ImGui)

# TARGET_LINK_LIBRARIES(RNGDungeon rlImGui)
# FetchContent_Declare(
#     rlImGui
#     GIT_REPOSITORY https://github.com/raylib-extras/rlImGui.git
#     GIT_TAG main
# )
# FetchContent_GetProperties(rlImGui)
# if(NOT rlImGui_POPULATED)
#   FetchContent_Populate(rlImGui)
#   add_subdirectory(${rlImGui_SOURCE_DIR} ${rlImGui_BINARY_DIR})
# endif()
# FetchContent_MakeAvailable(rlImGui)