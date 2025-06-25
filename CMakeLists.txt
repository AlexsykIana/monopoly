cmake_minimum_required(VERSION 3.16)

# Project configuration
project(MonopolyGame VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Automatic SFML Dependency Management using FetchContent ---

# 1. Include the FetchContent module
include(FetchContent)

# 2. Set options for the SFML build BEFORE declaring it.
#    We want to build SFML as a static library.
set(SFML_STATIC_LIBRARIES TRUE)
#    We can also disable things we don't need to speed up the build.
set(SFML_BUILD_AUDIO OFF)
set(SFML_BUILD_NETWORK OFF)
set(SFML_BUILD_EXAMPLES OFF)
set(SFML_BUILD_DOC OFF)

# 3. Declare the SFML dependency from its official GitHub repository.
#    We are targeting version 2.6.1, which is the latest stable release.
FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG        2.6.1 # Use the desired version tag from the SFML repo
)

# 4. Make the dependency available.
#    This will download and add SFML's CMakeLists.txt to our project build.
FetchContent_MakeAvailable(SFML)

# --- Your Project's Configuration ---

# Source files (using wildcards can be simpler for many files)
# This will find all .cpp files in the src/ directory.
file(GLOB SOURCES "src/*.cpp")

# Header files are not strictly needed in add_executable,
# but it's good practice for IDEs to see them.
file(GLOB HEADERS "include/*.h")

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Add include directories.
# Since SFML is now part of our build, its include directories are
# automatically handled by target_link_libraries. We only need our own.
target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/include
)

# If you build SFML statically, you must define SFML_STATIC.
# This is required for Windows.
target_compile_definitions(${PROJECT_NAME} PRIVATE SFML_STATIC)

# Link SFML libraries.
# These target names are defined by SFML's own CMakeLists.txt.
# Because SFML is now part of our build, these targets exist automatically.
target_link_libraries(${PROJECT_NAME} PRIVATE
        sfml-graphics # sfml-window and sfml-system are dependencies of this
)

# --- Asset Management (Optional but Recommended) ---

# Copy assets to the same directory as the executable after building.
# This ensures your program can find "assets/font.ttf", etc.
set(ASSETS_DIR ${CMAKE_SOURCE_DIR}/assets)
if(EXISTS ${ASSETS_DIR})
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${ASSETS_DIR} $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
    )
endif()