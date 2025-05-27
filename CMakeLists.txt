cmake_minimum_required(VERSION 3.20)

# Project configuration
project(MonopolyGame VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Windows-specific configurations
if(WIN32)
    # Set Windows subsystem for GUI application
    set(CMAKE_WIN32_EXECUTABLE ON)
    
    # Enable Unicode support
    add_definitions(-DUNICODE -D_UNICODE)
    
    # Set runtime library for Windows
    if(MSVC)
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    endif()
endif()

# Set build type if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# SFML Configuration
set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR "C:/SFML/lib/cmake/SFML" CACHE PATH "Path to SFML CMake files")

# Find SFML
find_package(SFML 2.5 REQUIRED COMPONENTS graphics window system audio network)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/src)

# Source files
set(SOURCES
    src/main.cpp
    src/Player.cpp
)

# Header files
set(HEADERS
    include/Player.h
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Link SFML libraries
target_link_libraries(${PROJECT_NAME} 
    sfml-graphics 
    sfml-window 
    sfml-system 
    sfml-audio 
    sfml-network
)

# Windows-specific linking
if(WIN32)
    target_link_libraries(${PROJECT_NAME} 
        opengl32
        winmm
        gdi32
    )
    
    # Copy SFML DLLs to output directory for dynamic linking
    if(NOT SFML_STATIC_LIBRARIES)
        # Find SFML DLLs and copy them
        find_file(SFML_GRAPHICS_DLL sfml-graphics-2.dll PATHS ${SFML_DIR}/../../../bin)
        find_file(SFML_WINDOW_DLL sfml-window-2.dll PATHS ${SFML_DIR}/../../../bin)
        find_file(SFML_SYSTEM_DLL sfml-system-2.dll PATHS ${SFML_DIR}/../../../bin)
        find_file(SFML_AUDIO_DLL sfml-audio-2.dll PATHS ${SFML_DIR}/../../../bin)
        find_file(SFML_NETWORK_DLL sfml-network-2.dll PATHS ${SFML_DIR}/../../../bin)
        
        if(SFML_GRAPHICS_DLL)
            add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${SFML_GRAPHICS_DLL} $<TARGET_FILE_DIR:${PROJECT_NAME}>)
        endif()
        
        if(SFML_WINDOW_DLL)
            add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${SFML_WINDOW_DLL} $<TARGET_FILE_DIR:${PROJECT_NAME}>)
        endif()
        
        if(SFML_SYSTEM_DLL)
            add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${SFML_SYSTEM_DLL} $<TARGET_FILE_DIR:${PROJECT_NAME}>)
        endif()
        
        if(SFML_AUDIO_DLL)
            add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${SFML_AUDIO_DLL} $<TARGET_FILE_DIR:${PROJECT_NAME}>)
        endif()
        
        if(SFML_NETWORK_DLL)
            add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${SFML_NETWORK_DLL} $<TARGET_FILE_DIR:${PROJECT_NAME}>)
        endif()
    endif()
endif()

# Compiler-specific options
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
    # Disable specific warnings for SFML
    target_compile_definitions(${PROJECT_NAME} PRIVATE _CRT_SECURE_NO_WARNINGS)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Debug configuration
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(${PROJECT_NAME} PRIVATE DEBUG)
endif()

# Create directories for assets
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/assets)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/assets/images)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/assets/sounds)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/assets/fonts)

# Copy assets to build directory
if(EXISTS ${CMAKE_SOURCE_DIR}/assets)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets)
endif()

# Installation
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)

install(DIRECTORY assets/ DESTINATION bin/assets OPTIONAL)

# Package configuration
set(CPACK_PACKAGE_NAME "MonopolyGame")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_DESCRIPTION "Desktop Monopoly Game built with SFML")
set(CPACK_PACKAGE_CONTACT "your-email@example.com")

if(WIN32)
    set(CPACK_GENERATOR "NSIS")
    set(CPACK_NSIS_DISPLAY_NAME "Monopoly Game")
    set(CPACK_NSIS_PACKAGE_NAME "MonopolyGame")
    set(CPACK_NSIS_CONTACT "your-email@example.com")
endif()

include(CPack) 