cmake_minimum_required(VERSION 3.15)
project(CelestiaWorks VERSION 0.1.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
option(CELESTIA_BUILD_APPLICATION "Build the example application" ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(Vulkan REQUIRED)
find_package(Freetype QUIET)
if(NOT FREETYPE_FOUND)
    message(STATUS "FreeType not found. Downloading and building from source...")
    include(FetchContent)
    FetchContent_Declare(
        freetype
        GIT_REPOSITORY https://github.com/freetype/freetype.git
        GIT_TAG VER-2-12-1
    )

    set(FT_DISABLE_HARFBUZZ ON CACHE BOOL "Disable HarfBuzz")
    set(FT_DISABLE_BZIP2 ON CACHE BOOL "Disable BZip2")
    set(FT_DISABLE_PNG ON CACHE BOOL "Disable PNG")
    set(FT_DISABLE_ZLIB ON CACHE BOOL "Disable ZLib")

    FetchContent_MakeAvailable(freetype)


    if(NOT TARGET Freetype::Freetype)
        add_library(Freetype::Freetype ALIAS freetype)
    endif()


    set(FREETYPE_FOUND TRUE)
    set(FREETYPE_INCLUDE_DIRS ${freetype_SOURCE_DIR}/include)
    set(FREETYPE_LIBRARIES freetype)
    message(STATUS "Using system-installed FreeType")
endif()

if(MSVC)
    add_compile_options(/W4)
    add_definitions(-DUNICODE -D_UNICODE)
else()
    add_compile_options(-Wall -Wextra -pedantic)
endif()

add_subdirectory(CelestiaWorks)


if(CELESTIA_BUILD_APPLICATION)
    add_subdirectory(Application)
endif()

