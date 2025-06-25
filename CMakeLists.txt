cmake_minimum_required(VERSION 3.16)

# Project configuration
project(MonopolyGame VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define SFML static linking
set(SFML_STATIC_LIBRARIES TRUE)

# Add SFML directory - adjust these paths to match your SFML installation
set(SFML_DIR "C:/SFML/lib/cmake/SFML")
set(SFML_ROOT "C:/SFML")
set(SFML_INCLUDE_DIR "C:/SFML/include")

# Find SFML
find_package(SFML 2.6.2 COMPONENTS system window graphics REQUIRED)

# # Include directories
# include_directories(${CMAKE_SOURCE_DIR}/include)
# include_directories(${CMAKE_SOURCE_DIR}/src)
# include_directories(${SFML_INCLUDE_DIR})

# Source files
set(SOURCES
        src/main.cpp
        src/Button.cpp
        src/Street.cpp
        src/Player.cpp
        src/StreetInitializer.cpp
        src/Card.cpp
        src/Deck.cpp
        src/BoardTile.cpp
        src/GameBoard.cpp
        src/StreetTile.cpp
        src/GoTile.cpp
        src/ChanceTile.cpp
        src/CommunityChestTile.cpp
        src/TaxTile.cpp
        src/JailOrJustVisitingTile.cpp
        src/GoToJailTile.cpp
        src/FreeParkingTile.cpp
        src/Game.cpp
)

# Header files
set(HEADERS
        include/Button.h
        include/Street.h
        include/Player.h
        include/StreetInitializer.h
        include/Card.h
        include/Deck.h
        include/BoardTile.h
        include/GameBoard.h
        include/StreetTile.h
        include/GoTile.h
        include/ChanceTile.h
        include/CommunityChestTile.h
        include/TaxTile.h
        include/JailOrJustVisitingTile.h
        include/GoToJailTile.h
        include/FreeParkingTile.h
        include/Game.h
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Add include directories (equivalent to -I flag)
target_include_directories(${PROJECT_NAME} PRIVATE
        ${SFML_INCLUDE_DIR}
        ${CMAKE_SOURCE_DIR}/include
)

# Link SFML libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
        sfml-system
        sfml-window
        sfml-graphics
)

# # Create directories for assets
# file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/assets)
# file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/assets/images)
# file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/assets/sounds)
# file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/assets/fonts)

# # Copy assets to build directory
# if(EXISTS ${CMAKE_SOURCE_DIR}/assets)
#     add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#             COMMAND ${CMAKE_COMMAND} -E copy_directory
#             ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets)
# endif()

# # Installation
# install(TARGETS ${PROJECT_NAME}
#         RUNTIME DESTINATION bin
#         LIBRARY DESTINATION lib
#         ARCHIVE DESTINATION lib)

# install(DIRECTORY assets/ DESTINATION bin/assets OPTIONAL)


# Define SFML static linking for the target
target_compile_definitions(${PROJECT_NAME} PRIVATE sfml-window sfml-graphics)