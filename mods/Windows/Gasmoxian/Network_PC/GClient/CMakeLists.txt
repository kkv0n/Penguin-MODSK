cmake_minimum_required(VERSION 3.20)
project(Gasmoxian-Client CXX C)

set(CMAKE_C_STANDARD 99)

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/../../../../../externals/enet/include)

# Add the path to the enet library
link_directories(${PROJECT_SOURCE_DIR}/../../../../../externals/enet/lib)

# Source files
set(SOURCES GASMOX_CLIENT.cpp)

# Create the executable
add_executable(gasmox_cl ${SOURCES})

# Link with the enet library
if (WIN32)
  target_link_libraries(gasmox_cl enet winmm ws2_32 psapi wsock32 advapi32)
else()
  target_link_libraries(gasmox_cl enet rt)
endif()

# Compiler options
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  target_compile_options(gasmox_cl PRIVATE -Wno-int-conversion -Wno-incompatible-function-pointer-types -Wno-implicit-function-declaration -Wno-return-type)
else()
  # Assume GCC
  target_compile_options(gasmox_cl PRIVATE -Wno-implicit-function-declaration -Wno-incompatible-pointer-types)
endif()

# Debug options
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  target_compile_options(gasmox_cl PRIVATE -g -gdwarf-2 -O0)
else()
  target_compile_options(gasmox_cl PRIVATE -O2)
endif()
