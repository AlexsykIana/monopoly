# Makefile for Windows Monopoly Game with SFML

# Compiler and tools
CXX = cl.exe
CMAKE = cmake
MSBUILD = msbuild.exe

# Build type (Release or Debug)
BUILD_TYPE = Release

# Directories
BUILD_DIR = build
INSTALL_DIR = install
SOURCE_DIR = .

# Default target
all: create_build configure build

# Create build directory
create_build:
	@if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)

# Configure with CMake
configure:
	@cd $(BUILD_DIR) && $(CMAKE) .. -G "Visual Studio 17 2022" -A Win32 ^
		-DCMAKE_BUILD_TYPE=$(BUILD_TYPE) ^
		-DSFML_DIR="C:/SFML/lib/cmake/SFML"

# Build the project
build:
	@cd $(BUILD_DIR) && $(MSBUILD) MonopolyGame.sln /p:Configuration=$(BUILD_TYPE)

# Install the project
install:
	@cd $(BUILD_DIR) && $(CMAKE) --install . --prefix ../$(INSTALL_DIR)

# Create distribution package
package:
	@cd $(BUILD_DIR) && $(CMAKE) --build . --target package

# Clean build files
clean:
	@if exist $(BUILD_DIR) rmdir /s /q $(BUILD_DIR)
	@if exist $(INSTALL_DIR) rmdir /s /q $(INSTALL_DIR)

# Create project structure
setup:
	@if not exist src mkdir src
	@if not exist include mkdir include
	@if not exist assets mkdir assets
	@if not exist assets\images mkdir assets\images
	@if not exist assets\sounds mkdir assets\sounds
	@if not exist assets\fonts mkdir assets\fonts

# Run the game (after building)
run:
	@if exist $(BUILD_DIR)\bin\$(BUILD_TYPE)\MonopolyGame.exe ^
		$(BUILD_DIR)\bin\$(BUILD_TYPE)\MonopolyGame.exe
	@if not exist $(BUILD_DIR)\bin\$(BUILD_TYPE)\MonopolyGame.exe ^
		echo Error: Game executable not found. Please build the project first.

# Help target
help:
	@echo Available targets:
	@echo   make          - Build the project
	@echo   make setup    - Create project directory structure
	@echo   make clean    - Remove build and install directories
	@echo   make install  - Install the game
	@echo   make package  - Create distribution package
	@echo   make run      - Run the game
	@echo.
	@echo Build configuration:
	@echo   BUILD_TYPE = $(BUILD_TYPE) (Release or Debug)

# Phony targets
.PHONY: all create_build configure build install package clean setup run help

# Build type specific configurations
debug: BUILD_TYPE = Debug
debug: all

release: BUILD_TYPE = Release
release: all 