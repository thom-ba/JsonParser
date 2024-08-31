#!/bin/bash

# Variables
SRC_FILE="./src/*"  # Source file to compile
OUTPUT_FILE="a.out"  # Name of the output executable
CXX=g++              # C++ compiler
CXXFLAGS="-Wall -Wextra -std=c++17 -O2"  # Compiler flags for warnings, C++17 standard, and optimization

# Compile the source file
echo "Compiling $SRC_FILE..."
$CXX $CXXFLAGS -o $OUTPUT_FILE $SRC_FILE

# Check if compilation was successful
if [ $? -eq 0 ]; then
    echo "Build successful. Executable created as $OUTPUT_FILE"
else
    echo "Build failed."
    exit 1
fi

# Optional: Run the executable
# Uncomment the following lines to run the executable automatically after building
echo "Running the executable..."
./$OUTPUT_FILE < input.txt

