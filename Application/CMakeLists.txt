# CMakeLists.txt for CelestiaApp application

# Define the executable
project(CelestiaApp)

# Collect all source files from the src directory
file(GLOB_RECURSE APP_SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

# Create the executable target
add_executable(CelestiaApp ${APP_SOURCES})

# Include directories 
target_include_directories(CelestiaApp PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/CelestiaWorks/include
)

# Link against the CelestiaWorks library
target_link_libraries(CelestiaApp PRIVATE CelestiaWorks)

# Set output directory
set_target_properties(CelestiaApp PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

