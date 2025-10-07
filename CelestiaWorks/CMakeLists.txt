cmake_minimum_required(VERSION 3.16)
project(CelestiaWorks VERSION 1.0.0 LANGUAGES CXX)

option(CELESTIAWORKS_BUILD_SHARED_LIBS "Build CelestiaWorks as a shared library" ON)

if(CELESTIAWORKS_BUILD_SHARED_LIBS)
    set(CELESTIAWORKS_LIB_TYPE SHARED)
else()
    set(CELESTIAWORKS_LIB_TYPE STATIC)
endif()


find_package(Vulkan REQUIRED)
if(NOT TARGET Freetype::Freetype)
    find_package(Freetype REQUIRED)
endif()
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

add_library(CelestiaWorks ${CELESTIAWORKS_LIB_TYPE}
    ${CELESTIAWORKS_SOURCES}
    ${CELESTIAWORKS_HEADERS}
)

target_compile_features(CelestiaWorks PUBLIC cxx_std_17)

set(BASE_INCLUDE_DIRS
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/CelestiaWorks>
    $<INSTALL_INTERFACE:include/CelestiaWorks>
    ${CMAKE_CURRENT_SOURCE_DIR}/source
    ${CMAKE_SOURCE_DIR}/dependencies
)

if(UNIX AND NOT APPLE)
    message(STATUS "Linux detected: adding linux-specific dependencies")
    list(APPEND BASE_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/dependencies/linux)
endif()

target_include_directories(CelestiaWorks
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/CelestiaWorks>
    $<INSTALL_INTERFACE:include/CelestiaWorks>
    PRIVATE
    ${BASE_INCLUDE_DIRS}
)

if(UNIX AND NOT APPLE)
    target_sources(CelestiaWorks PRIVATE
        ${CMAKE_SOURCE_DIR}/dependencies/linux/xdg-shell-client-protocol.c
    )
    target_link_libraries(CelestiaWorks PRIVATE wayland-client)
endif()

target_link_libraries(CelestiaWorks
    PUBLIC
    Vulkan::Vulkan
    Freetype::Freetype
)

install(TARGETS CelestiaWorks
    EXPORT CelestiaWorksTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

install(DIRECTORY include/
    DESTINATION include
    FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp"
)

if(CELESTIAWORKS_BUILD_SHARED_LIBS)
    target_compile_definitions(CelestiaWorks
        PRIVATE CELESTIAWORKS_EXPORTS
        PUBLIC CELESTIAWORKS_SHARED
    )
endif()

target_compile_definitions(CelestiaWorks PRIVATE CELESTIA_WORKS_EXPORTS UNICODE _UNICODE)

target_compile_definitions(CelestiaWorks PRIVATE
    $<$<CONFIG:Debug>:CELESTIA_DEBUG>
)

