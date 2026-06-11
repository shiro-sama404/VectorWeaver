cmake_minimum_required(VERSION 3.16)
project(VectorWeaver VERSION 2.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra -pedantic -O2)

# ==============================================================================
# ESTRUTURA DE DIRETÓRIOS E FONTES
# ==============================================================================
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(CORE_SOURCES
    ${SRC_DIR}/vector_clock.cpp
    ${SRC_DIR}/json_converter.cpp
    ${SRC_DIR}/network_manager.cpp
    ${SRC_DIR}/message_handler.cpp
    ${SRC_DIR}/cli_handler.cpp
)

set(APP_SOURCES ${SRC_DIR}/main.cpp)

# ==============================================================================
# PACOTES E DEPENDÊNCIAS (via MSYS2 / pacman)
# ==============================================================================
find_package(ZeroMQ REQUIRED)
# find_package(glfw3 REQUIRED)
# find_package(OpenGL REQUIRED)

# ==============================================================================
# DEFINIÇÃO DO EXECUTÁVEL
# ==============================================================================
add_executable(${PROJECT_NAME} ${CORE_SOURCES} ${APP_SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE 
    ${INC_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/deps # Para o json.hpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/deps/imgui
)

target_link_libraries(${PROJECT_NAME} PRIVATE 
    libzmq
    # glfw OpenGL::GL
)

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")