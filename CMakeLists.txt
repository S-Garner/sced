cmake_minimum_required(VERSION 3.15)
project(sced)

set(CMAKE_CXX_STANDARD 17)

# --- GLAD ---
add_library(glad external/glad/src/glad.c)
target_include_directories(glad PUBLIC external/glad/include)

# --- GLFW paths per platform ---
if (WIN32)
    set(GLFW_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/libs/win64/include)
    set(GLFW_LIB_DIR ${CMAKE_SOURCE_DIR}/libs/win64/lib-vc2022)
    set(GLFW_LIB glfw3)
elseif (APPLE)
    set(GLFW_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/libs/macos/include)
    set(GLFW_LIB_DIR ${CMAKE_SOURCE_DIR}/libs/macos/lib-universal)
    set(GLFW_DLL_DIR ${CMAKE_SOURCE_DIR}/libs/macos/lib-universal)
    set(GLFW_LIB libglfw3.a)
else()
    set(GLFW_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/libs/linux/include)
    set(GLFW_LIB_DIR ${CMAKE_SOURCE_DIR}/libs/linux/so)
    set(GLFW_DLL_DIR ${CMAKE_SOURCE_DIR}/libs/linux/so)
    set(GLFW_LIB glfw)  # links libglfw.so*
endif()

include_directories(${GLFW_INCLUDE_DIR})
link_directories(${GLFW_LIB_DIR})

# --- GLM (header-only) ---
add_subdirectory(external/glm)

# --- Main executable ---
add_executable(sced
        src/main.cpp
        src/Renderer/Shader/Shader.cpp
        src/Renderer/Renderer2D.cpp
        src/core/Window.cpp
		src/input/Input.cpp
)

# --- Link everything ---
target_link_libraries(sced PRIVATE glad ${GLFW_LIB} glm)

# --- System OpenGL ---
if (WIN32)
    target_link_libraries(sced PRIVATE opengl32)
elseif (APPLE)
    find_library(COCOA_LIBRARY Cocoa REQUIRED)
    find_library(OpenGL_LIBRARY OpenGL REQUIRED)
    find_library(IOKIT_LIBRARY IOKit REQUIRED)
    find_library(COREVIDEO_LIBRARY CoreVideo REQUIRED)
    target_link_libraries(sced PRIVATE ${COCOA_LIBRARY} ${OpenGL_LIBRARY} ${IOKIT_LIBRARY} ${COREVIDEO_LIBRARY})
else()
    target_link_libraries(sced PRIVATE GL X11 pthread Xrandr Xi dl)
endif()

# --- Windows DLL copy (safe version) ---
if (WIN32)
    set(DLL_SOURCE_DIR "${CMAKE_SOURCE_DIR}/libs/win64/lib-mingw-w64")
    if (EXISTS "${DLL_SOURCE_DIR}")
        add_custom_command(TARGET sced POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E echo "Copying GLFW runtime files..."
                COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${DLL_SOURCE_DIR}"
                "$<TARGET_FILE_DIR:sced>"
        )
    else()
        message(WARNING "GLFW DLL directory not found: ${DLL_SOURCE_DIR}")
    endif()
endif()

