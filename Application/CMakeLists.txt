project(CelestiaApp)

file(GLOB_RECURSE APP_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_executable(CelestiaApp ${APP_SOURCES})

target_include_directories(CelestiaApp PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/CelestiaWorks/include
)

target_link_libraries(CelestiaApp PRIVATE CelestiaWorks)

set_target_properties(CelestiaApp PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

add_custom_command(TARGET CelestiaApp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/assets"
    "$<TARGET_FILE_DIR:CelestiaApp>/assets"
    COMMENT "Copying assets to build directory"
)
