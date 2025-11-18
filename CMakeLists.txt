cmake_minimum_required(VERSION 3.15)
project(sced)

set(CMAKE_CXX_STANDARD 17)

# --- GLAD ---
add_library(glad external/glad/src/glad.c
        src/parser/SCparse.hpp
        src/parser/SCparse.cpp)
target_include_directories(glad PUBLIC
        external/glad/include
        ${CMAKE_SOURCE_DIR}/external/nlohmann)

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
target_link_libraries(glad PUBLIC glm)

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

add_executable(scparse_tests
        src/tests/SCparseTests.cpp)

target_link_libraries(scparse_tests PRIVATE glad ${GLFW_LIB} glm)
target_compile_definitions(scparse_tests PRIVATE
        SCPARSE_TEST_JSON_PATH="${CMAKE_SOURCE_DIR}/src/tests/data/sample_shapes.json")

# --- System OpenGL ---
if (WIN32)
    set(PLATFORM_LIBS opengl32)
elseif (APPLE)
    find_library(COCOA_LIBRARY Cocoa REQUIRED)
    find_library(OpenGL_LIBRARY OpenGL REQUIRED)
    find_library(IOKIT_LIBRARY IOKit REQUIRED)
    find_library(COREVIDEO_LIBRARY CoreVideo REQUIRED)
    set(PLATFORM_LIBS ${COCOA_LIBRARY} ${OpenGL_LIBRARY} ${IOKIT_LIBRARY} ${COREVIDEO_LIBRARY})
else()
    set(PLATFORM_LIBS GL X11 pthread Xrandr Xi dl)
endif()

foreach(target_name IN ITEMS sced test_scobject_shapes paint_test numbers_test simon scparse_tests)
    if (TARGET ${target_name})
        target_link_libraries(${target_name} PRIVATE ${PLATFORM_LIBS})
    endif()
endforeach()

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
