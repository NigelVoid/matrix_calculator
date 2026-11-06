cmake_minimum_required(VERSION 3.16)
project(matrix_calculator VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

add_compile_options(-Wall -Wextra -Wpedantic)

add_library(matrix_core STATIC src/matrix.cpp)
target_include_directories(matrix_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(matrix_app src/main.cpp)
target_link_libraries(matrix_app PRIVATE matrix_core)

enable_testing()
add_subdirectory(tests)

find_package(Doxygen QUIET)
if(DOXYGEN_FOUND)
    add_custom_target(docs
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Генерация документации Doxygen в docs/html"
    )
endif()