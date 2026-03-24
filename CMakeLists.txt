cmake_minimum_required(VERSION 3.16)

add_compile_definitions(VULKAN_HPP_NO_STRUCT_CONSTRUCTORS)

project(VulkanFramework2D)

set(PROJECT_ROOT ${CMAKE_SOURCE_DIR})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)
find_package(SDL3 REQUIRED)
find_package(glm REQUIRED)

function(add_shader_compilation TARGET INPUT_SHADER OUTPUT_SPv)
    set(ABS_INPUT_SHADER ${CMAKE_CURRENT_SOURCE_DIR}/${INPUT_SHADER})
    set(ABS_OUTPUT_SPv ${CMAKE_CURRENT_SOURCE_DIR}/${OUTPUT_SPv})

    add_custom_command(
        OUTPUT ${ABS_OUTPUT_SPv}
        COMMAND slangc
                ${ABS_INPUT_SHADER}
                -target spirv
                -profile spirv_1_4
                -emit-spirv-directly
                -fvk-use-entrypoint-name
                -entry vertMain -entry fragMain
                -o ${ABS_OUTPUT_SPv}
        DEPENDS ${ABS_INPUT_SHADER}
        COMMENT "Compiling ${INPUT_SHADER} -> ${OUTPUT_SPv}"
        VERBATIM
    )
    add_custom_target(${TARGET} DEPENDS ${OUTPUT_SPv})
endfunction()

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/Logs.cpp
    src/FileUtility.cpp
    src/VulkanShaderModule.cpp
    src/Game.cpp
    src/GameWindow.cpp
    src/VulkanInstance.cpp
    src/WindowSurface.cpp
    src/VulkanPhysicalDevice.cpp
    src/VulkanLogicalDevice.cpp
    src/VulkanSwapchain.cpp
)

add_shader_compilation(BaseShader Assets/Shaders/shader.slang Assets/Shaders/slang.spv)
add_dependencies(${PROJECT_NAME} BaseShader)

set(PCH_INCLUDE_HEADERS pch.h)
target_precompile_headers(${PROJECT_NAME} PRIVATE ${PCH_INCLUDE_HEADERS})

target_include_directories(${PROJECT_NAME} PRIVATE include)

target_compile_definitions(${PROJECT_NAME} PRIVATE PROJECT_ROOT="${PROJECT_ROOT}")

target_link_libraries(${PROJECT_NAME}
    Vulkan::Vulkan
    vulkan
    glfw
    SDL3::SDL3
    glm::glm
)
