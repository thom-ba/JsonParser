# JSON Parser

A simple C++ JSON parser that reads and parses JSON data from a file, supporting basic JSON types such as objects, arrays, strings, numbers, booleans, and null values. This parser is a really simple one, as I needed it for my webserver.

## Features

- Parses JSON objects, arrays, strings, numbers, booleans, and null values.
- Outputs parsed JSON in a readable format.
- Handles white spaces and various JSON structures.
- Basic error handling for malformed JSON.

## Getting Started

### Prerequisites

- C++ compiler supporting C++11 or later (e.g., GCC, Clang, MSVC).
- Make sure you have `g++` installed if you're using GCC.

### Building the Project

Clone the repository and navigate to the project directory:

```bash
git clone https://github.com/yourusername/json-parser.git
cd json-parser
```

To compile the project, run the provided build script or use the following command:
```bash
./build.sh
```
Alternatively, you can compile it manually using:
```bash
g++ -std=c++11 -o json_parser src/*.cpp
```

