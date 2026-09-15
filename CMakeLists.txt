cmake_minimum_required(VERSION 3.20)

project(Pulse
    VERSION 0.1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ============================================================
# GLFW
# ============================================================

set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)

if (EXISTS "${PROJECT_SOURCE_DIR}/external/glfw/CMakeLists.txt")
    add_subdirectory(
        external/glfw
    )
else()
    include(FetchContent)

    FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.4
        GIT_SHALLOW TRUE
    )

    FetchContent_MakeAvailable(glfw)
endif()

# ============================================================
# Dear ImGui
# ============================================================

set(IMGUI_DIR
    ${PROJECT_SOURCE_DIR}/external/imgui
)

set(IMGUI_SOURCES

    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp

    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

add_library(imgui
    ${IMGUI_SOURCES}
)

target_include_directories(imgui
    PUBLIC

        ${IMGUI_DIR}
        ${IMGUI_DIR}/backends

    PRIVATE

        ${PROJECT_SOURCE_DIR}/external/glfw/include
)

target_link_libraries(imgui
    PUBLIC

        glfw
)

# ============================================================
# Pulse
# ============================================================

add_executable(Pulse

    src/main.cpp

)

target_include_directories(Pulse

    PRIVATE

        ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(Pulse

    PRIVATE

        glfw
        imgui

)

# ============================================================
# OpenGL
# ============================================================

find_package(OpenGL REQUIRED)

target_link_libraries(Pulse

    PRIVATE

        OpenGL::GL

)