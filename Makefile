# =============================================================================
#  Makefile  —  AcademiCLI Academic Management System
#  Works with g++ (MinGW on Windows / GCC on Linux)
# =============================================================================

CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic -O2 -Iinclude
TARGET   := AcademiCLI

# Directories
SRC_DIR  := src
OBJ_DIR  := build/obj
BIN_DIR  := build/bin

# Source and object files
SRCS     := $(wildcard $(SRC_DIR)/*.cpp)
OBJS     := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

# Default target
.PHONY: all clean run dirs

all: dirs $(BIN_DIR)/$(TARGET)

# Link
$(BIN_DIR)/$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^
	@echo ""
	@echo "  Build successful → $@"
	@echo ""

# Compile each .cpp
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create build directories
dirs:
	@mkdir -p $(OBJ_DIR) $(BIN_DIR)

# Run the application
run: all
	./$(BIN_DIR)/$(TARGET)

# Clean build artifacts
clean:
	rm -rf build/
	@echo "  Cleaned build directory."
