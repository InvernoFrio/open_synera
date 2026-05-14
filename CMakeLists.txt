cmake_minimum_required(VERSION 3.24)

project(AutoChessEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Vulkan REQUIRED)

set(GLFW_ROOT "D:/glfw/3.4")
set(GLM_ROOT "D:/glm/1.0.3")

file(GLOB_RECURSE ENGINE_SOURCES
    engine/src/*.cpp
)

file(GLOB_RECURSE GAME_SOURCES
    game/src/*.cpp
)

add_executable(AutoChess
    ${ENGINE_SOURCES}
    ${GAME_SOURCES}
)

target_include_directories(AutoChess PRIVATE
    engine/include
    game/src
    "${GLFW_ROOT}/include"
    "${GLM_ROOT}"
)

target_link_libraries(AutoChess PRIVATE
    Vulkan::Vulkan
    "${GLFW_ROOT}/lib-vc2022/glfw3.lib"
)

target_compile_definitions(AutoChess PRIVATE
    GLFW_INCLUDE_NONE
    $<$<CONFIG:Debug>:ENGINE_DEBUG>
)

if (MSVC)
    target_compile_options(AutoChess PRIVATE /W4 /EHsc /MP)
endif()

# Shader compilation

find_program(GLSLC glslc HINTS "$ENV{VULKAN_SDK}/Bin")

if (NOT GLSLC)
    message(FATAL_ERROR "glslc not found. Please install Vulkan SDK and check VULKAN_SDK environment variable.")
endif()

file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/assets/shaders)

file(GLOB SHADER_SOURCES
    ${CMAKE_SOURCE_DIR}/shaders/*.vert
    ${CMAKE_SOURCE_DIR}/shaders/*.frag
)

set(SPIRV_BINARY_FILES)

foreach(SHADER ${SHADER_SOURCES})
    get_filename_component(SHADER_NAME ${SHADER} NAME)

    set(SPIRV_FILE
        ${CMAKE_SOURCE_DIR}/assets/shaders/${SHADER_NAME}.spv
    )

    add_custom_command(
        OUTPUT ${SPIRV_FILE}
        COMMAND ${GLSLC} ${SHADER} -o ${SPIRV_FILE}
        DEPENDS ${SHADER}
        COMMENT "Compiling shader ${SHADER_NAME}"
    )

    list(APPEND SPIRV_BINARY_FILES ${SPIRV_FILE})
endforeach()

add_custom_target(CompileShaders ALL
    DEPENDS ${SPIRV_BINARY_FILES}
)

add_dependencies(AutoChess CompileShaders)