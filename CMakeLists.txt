cmake_minimum_required(VERSION 3.16)
project(AcademiCLI VERSION 1.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Include directories
include_directories(include)

# Source files
set(SOURCES
    src/main.cpp
    src/Student.cpp
    src/FileManager.cpp
    src/MenuController.cpp
)

# Executable target
add_executable(AcademiCLI ${SOURCES})

# Target properties & compiler options
if (MSVC)
    target_compile_options(AcademiCLI PRIVATE /W4 /EHsc /utf-8 /O2)
else()
    target_compile_options(AcademiCLI PRIVATE -Wall -Wextra -Wpedantic -O2)
endif()

# Set output directories
set_target_properties(AcademiCLI PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
