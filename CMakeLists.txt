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

include_directories(${CMAKE_SOURCE_DIR}/src)

# --- GLM (header-only) ---
add_subdirectory(external/glm)

# --- Main executable ---
add_executable(sced
        src/main.cpp
        src/Application/Application.cpp
        src/Renderer/Shader/Shader.cpp
        src/Renderer/Renderer2D.cpp
        src/core/Window.cpp
		src/input/Input.cpp
        src/Renderer/Shapes/IShape2D.hpp
        src/Renderer/Shapes/RectangleShape.hpp
        src/Renderer/Shapes/CircleShape.hpp
        src/Renderer/Shapes/EllipseShape.hpp
        src/Renderer/Shapes/RegularPolygonShape.hpp
        src/objects/SCObject.hpp
)

# --- Link everything ---
target_link_libraries(sced PRIVATE glad ${GLFW_LIB} glm)

if (APPLE)
    find_library(COCOA_LIBRARY Cocoa REQUIRED)
    find_library(OpenGL_LIBRARY OpenGL REQUIRED)
    find_library(IOKIT_LIBRARY IOKit REQUIRED)
    find_library(COREVIDEO_LIBRARY CoreVideo REQUIRED)

    target_link_libraries(sced PRIVATE
        ${COCOA_LIBRARY}
        ${OpenGL_LIBRARY}
        ${IOKIT_LIBRARY}
        ${COREVIDEO_LIBRARY}
    )
endif()

# --- Test executable ---
add_executable(test_scobject_shapes
        src/tests/Test_SCObjectShapes.cpp
        src/Renderer/Shader/Shader.cpp
        src/Renderer/Renderer2D.cpp
        src/core/Window.cpp
        src/input/Input.cpp
        src/objects/SCObject.cpp
)

target_link_libraries(test_scobject_shapes PRIVATE glad ${GLFW_LIB} glm)

if (APPLE)
    find_library(COCOA_LIBRARY Cocoa REQUIRED)
    find_library(OpenGL_LIBRARY OpenGL REQUIRED)
    find_library(IOKIT_LIBRARY IOKit REQUIRED)
    find_library(COREVIDEO_LIBRARY CoreVideo REQUIRED)

    target_link_libraries(test_scobject_shapes PRIVATE
        ${COCOA_LIBRARY}
        ${OpenGL_LIBRARY}
        ${IOKIT_LIBRARY}
        ${COREVIDEO_LIBRARY}
    )
endif()

add_executable(paint_test
        src/tests/Paint_Test.cpp
        src/ui/elements/SCButton.cpp
        src/Renderer/Shader/Shader.cpp
        src/Renderer/Renderer2D.cpp
        src/Renderer/Renderer2D.cpp
        src/core/Window.cpp
        src/input/Input.cpp
        src/objects/SCObject.cpp
)

target_link_libraries(paint_test PRIVATE glad ${GLFW_LIB} glm)

if (APPLE)
    find_library(COCOA_LIBRARY Cocoa REQUIRED)
    find_library(OpenGL_LIBRARY OpenGL REQUIRED)
    find_library(IOKIT_LIBRARY IOKit REQUIRED)
    find_library(COREVIDEO_LIBRARY CoreVideo REQUIRED)

    target_link_libraries(paint_test PRIVATE
        ${COCOA_LIBRARY}
        ${OpenGL_LIBRARY}
        ${IOKIT_LIBRARY}
        ${COREVIDEO_LIBRARY}
    )
endif()

add_executable(pong
        src/tests/Pong.cpp
        src/ui/elements/SCButton.cpp
        src/Renderer/Shader/Shader.cpp
        src/Renderer/Renderer2D.cpp
        src/Renderer/Renderer2D.cpp
        src/core/Window.cpp
        src/input/Input.cpp
        src/objects/SCObject.cpp
)

target_link_libraries(pong PRIVATE glad ${GLFW_LIB} glm)

if (APPLE)
    find_library(COCOA_LIBRARY Cocoa REQUIRED)
    find_library(OpenGL_LIBRARY OpenGL REQUIRED)
    find_library(IOKIT_LIBRARY IOKit REQUIRED)
    find_library(COREVIDEO_LIBRARY CoreVideo REQUIRED)

    target_link_libraries(pong PRIVATE
        ${COCOA_LIBRARY}
        ${OpenGL_LIBRARY}
        ${IOKIT_LIBRARY}
        ${COREVIDEO_LIBRARY}
    )
endif()

add_executable(numbers_test
        src/tests/Numbers.cpp
        src/Renderer/Shader/Shader.cpp
        src/Renderer/Renderer2D.cpp
        src/Renderer/Renderer2D.cpp
        src/core/Window.cpp
        src/input/Input.cpp
        src/objects/SCObject.cpp
)

target_link_libraries(numbers_test PRIVATE glad ${GLFW_LIB} glm)

if (APPLE)
    find_library(COCOA_LIBRARY Cocoa REQUIRED)
    find_library(OpenGL_LIBRARY OpenGL REQUIRED)
    find_library(IOKIT_LIBRARY IOKit REQUIRED)
    find_library(COREVIDEO_LIBRARY CoreVideo REQUIRED)

    target_link_libraries(numbers_test PRIVATE
        ${COCOA_LIBRARY}
        ${OpenGL_LIBRARY}
        ${IOKIT_LIBRARY}
        ${COREVIDEO_LIBRARY}
    )
endif()

add_executable(simon
        src/tests/Simon.cpp
        src/ui/elements/SCButton.cpp
        src/Renderer/Shader/Shader.cpp
        src/Renderer/Renderer2D.cpp
        src/Renderer/Renderer2D.cpp
        src/core/Window.cpp
        src/input/Input.cpp
        src/objects/SCObject.cpp
)

target_link_libraries(simon PRIVATE glad ${GLFW_LIB} glm)

if (APPLE)
    find_library(COCOA_LIBRARY Cocoa REQUIRED)
    find_library(OpenGL_LIBRARY OpenGL REQUIRED)
    find_library(IOKIT_LIBRARY IOKit REQUIRED)
    find_library(COREVIDEO_LIBRARY CoreVideo REQUIRED)

    target_link_libraries(simon PRIVATE
        ${COCOA_LIBRARY}
        ${OpenGL_LIBRARY}
        ${IOKIT_LIBRARY}
        ${COREVIDEO_LIBRARY}
    )
endif()

add_executable(new_paint
        src/tests/New_Paint.cpp
        src/ui/elements/SCButton.cpp
        src/ui/UIManager.hpp
        src/ui/FrameState.hpp
        src/input/ScreenToWorld.hpp
        src/scene/LayerStack.hpp
        src/Renderer/Shader/Shader.cpp
        src/Renderer/Renderer2D.cpp
        src/core/Window.cpp
        src/input/Input.cpp
        src/objects/SCObject.cpp
)

target_link_libraries(new_paint PRIVATE glad ${GLFW_LIB} glm)

if (APPLE)
    find_library(COCOA_LIBRARY Cocoa REQUIRED)
    find_library(OpenGL_LIBRARY OpenGL REQUIRED)
    find_library(IOKIT_LIBRARY IOKit REQUIRED)
    find_library(COREVIDEO_LIBRARY CoreVideo REQUIRED)

    target_link_libraries(new_paint PRIVATE
        ${COCOA_LIBRARY}
        ${OpenGL_LIBRARY}
        ${IOKIT_LIBRARY}
        ${COREVIDEO_LIBRARY}
    )
endif()

# --- System OpenGL ---
if (WIN32)
    target_link_libraries(test_scobject_shapes PRIVATE opengl32)
elseif (APPLE)
    find_library(COCOA_LIBRARY Cocoa REQUIRED)
    find_library(OpenGL_LIBRARY OpenGL REQUIRED)
    find_library(IOKIT_LIBRARY IOKit REQUIRED)
    find_library(COREVIDEO_LIBRARY CoreVideo REQUIRED)
    target_link_libraries(test_scobject_shapes PRIVATE ${COCOA_LIBRARY} ${OpenGL_LIBRARY} ${IOKIT_LIBRARY} ${COREVIDEO_LIBRARY})
else()
    target_link_libraries(test_scobject_shapes PRIVATE GL X11 pthread Xrandr Xi dl)
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
