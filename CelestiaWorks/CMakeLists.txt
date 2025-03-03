cmake_minimum_required(VERSION 3.16)

# Library definition
project(CelestiaWorks VERSION 1.0.0 LANGUAGES CXX)

# Option to build as static or shared library
option(CELESTIAWORKS_BUILD_SHARED_LIBS "Build CelestiaWorks as a shared library" ON)

if(CELESTIAWORKS_BUILD_SHARED_LIBS)
set(CELESTIAWORKS_LIB_TYPE SHARED)
else()
set(CELESTIAWORKS_LIB_TYPE STATIC)
endif()

# Find required packages
find_package(Vulkan REQUIRED)
# Check if Freetype has already been found/configured in the parent scope
if(NOT TARGET Freetype::Freetype)
find_package(Freetype REQUIRED)
endif()
# Source files
# Note: In production, explicitly listing source files is preferred over globbing
file(GLOB_RECURSE CELESTIAWORKS_SOURCES
"source/*.cpp"
"source/*.c"
)

file(GLOB_RECURSE CELESTIAWORKS_HEADERS
"include/*.h"
"include/*.hpp"
"source/*.h"
"source/*.hpp"
)

# Create library target
add_library(CelestiaWorks ${CELESTIAWORKS_LIB_TYPE} 
${CELESTIAWORKS_SOURCES}
${CELESTIAWORKS_HEADERS}
)

# Set C++ standard
target_compile_features(CelestiaWorks PUBLIC cxx_std_17)

# Include directories
target_include_directories(CelestiaWorks
PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/CelestiaWorks>
    $<INSTALL_INTERFACE:include/CelestiaWorks>
PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/source
    ${CMAKE_SOURCE_DIR}/dependencies
)

# Link dependencies
target_link_libraries(CelestiaWorks
PUBLIC
    Vulkan::Vulkan
    Freetype::Freetype
)

# Export targets
install(TARGETS CelestiaWorks
EXPORT CelestiaWorksTargets
LIBRARY DESTINATION lib
ARCHIVE DESTINATION lib
RUNTIME DESTINATION bin
INCLUDES DESTINATION include
)

# Install header files
install(DIRECTORY include/
DESTINATION include
FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp"
)

# Define compile definitions for shared library builds
if(CELESTIAWORKS_BUILD_SHARED_LIBS)
target_compile_definitions(CelestiaWorks 
    PRIVATE CELESTIAWORKS_EXPORTS
    PUBLIC CELESTIAWORKS_SHARED
)
endif()

# Add CELESTIA_WORKS_EXPORTS for all builds
target_compile_definitions(CelestiaWorks PRIVATE CELESTIA_WORKS_EXPORTS UNICODE _UNICODE)

# Add CELESTIA_DEBUG for debug builds only
target_compile_definitions(CelestiaWorks PRIVATE
    $<$<CONFIG:Debug>:CELESTIA_DEBUG>
)

