cmake_minimum_required(VERSION 3.20)
project(Gasmox-Server C)

set(CMAKE_C_STANDARD 99)

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/../../../../../externals/enet/include)

# Add the path to the enet library
link_directories(${PROJECT_SOURCE_DIR}/../../../../../externals/enet/lib)

# Source files
set(SOURCES GASMOX_SV.c GASMOX_SV2.c)

# Create the executable
add_executable(gasmox_srv ${SOURCES})

# Link with the enet library
if (WIN32)
    target_link_libraries(gasmox_srv enet winmm ws2_32)
else()
    target_link_libraries(gasmox_srv enet)
endif()

# Compiler options
if(CMAKE_C_COMPILER_ID MATCHES "Clang")
    target_compile_options(gasmox_srv PRIVATE -Wno-int-conversion -Wno-incompatible-function-pointer-types -Wno-implicit-function-declaration -Wno-return-type)
else()
    # Assume GCC
    target_compile_options(gasmox_srv PRIVATE -Wno-implicit-function-declaration -Wno-incompatible-pointer-types)
endif()

# Debug options
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(gasmox_srv PRIVATE -g -gdwarf-2 -O0)
else()
    target_compile_options(gasmox_srv PRIVATE -O2)
endif()
