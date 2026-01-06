# Kolokvium 1 - C++20 Industrial-Standard Application

**Belarusian State University, Faculty of Applied Mathematics and Computer Science, Department of Programming Technology**  
**Kolokvium 1**

## 🚀 Project Overview

Kolokvium 1 is a modern C++20 application showcasing industrial-grade software development practices. The project implements three core algorithms with comprehensive error handling, extensive unit testing, and a professional build system using CMake.

## 📁 Project Structure

```
kolokvium/
├── CMakeLists.txt              # Main CMake configuration
├── CMakePresets.json           # CMake build presets
├── compilation.txt             # Compilation notes
├── include/                    # Header files
│   ├── dedup.hpp               # Deduplication algorithm
│   ├── errors.hpp              # Custom exception classes
│   ├── factorial.hpp           # Big integer factorial computation
│   └── singly_list.hpp         # Linked list utilities
├── src/                        # Source code
│   ├── cli_main.cpp            # Command-line interface
│   └── factorial.cpp           # Big integer implementation
├── tests/                      # Unit tests
│   ├── test_cli.cpp            # CLI interface tests
│   ├── test_dedup.cpp          # Deduplication algorithm tests
│   ├── test_factorial.cpp      # Factorial computation tests
│   └── test_list.cpp           # Linked list tests
└── README.md                   # This documentation file
```

## ✨ Features

- **Modern C++20**: Leverages latest C++ standards and features
- **Industrial-Grade Code**: Production-ready with proper error handling
- **Comprehensive Testing**: Full unit test coverage with Google Test
- **Professional Build System**: Modern CMake with presets support
- **Safe Big Integer Arithmetic**: Custom `BigInt` class for large factorial computation
- **Memory Safety**: Smart pointers and RAII throughout
- **Stable Algorithms**: Preserve input order where required

## 🏗️ Building the Project

### Prerequisites
- **CMake 3.20+**
- **C++20 compatible compiler** (MSVC 2022+, GCC 11+, Clang 14+)
- **Git** (for cloning)

### Quick Start

```bash
# Configure using presets (recommended)
cmake --preset=default

# Build the project
cmake --build --preset=default

# Run tests
ctest --preset=default
```

### Manual Build

```bash
mkdir build && cd build
cmake -DCMAKE_CXX_STANDARD=20 -DCMAKE_CXX_STANDARD_REQUIRED=ON ..
cmake --build . --parallel 4
```

### Build Options
- **`ENABLE_COVERAGE`**: Enable code coverage instrumentation (GCC/Clang only)
- **`CMAKE_BUILD_TYPE`**: Debug/Release/RelWithDebInfo/MinSizeRel

## 🧪 Running Tests

The project includes comprehensive unit tests using Google Test:

```bash
# Run all tests
./build/unit_tests

# Run specific test suites
./build/unit_tests --gtest_filter="Factorial*"
./build/unit_tests --gtest_filter="Dedup*"
./build/unit_tests --gtest_filter="List*"
./build/unit_tests --gtest_filter="CLI*"
```

### Test Coverage Includes:
- **Factorial computation** with large numbers
- **Deduplication algorithm** stability and correctness
- **Linked list reversal** using recursion
- **CLI interface** error handling and validation
- **Edge cases** and exceptional situations

## 📖 Using the Application

### Command Line Interface

```bash
# Run the interactive CLI
./build/cli

# Example session:
# Enter n (number of factorials to compute): 5
# 1! = 1
# 2! = 2
# 3! = 6
# 4! = 24
# 5! = 120
# 
# Enter integers for deduplication (space-separated): 3 3 2 1 2
# Deduplicated: 3 2 1
# 
# Enter integers for linked list reversal (space-separated): 1 2 3 4
# Reversed: 4 3 2 1
```

### Program Features

1. **Factorial Computation** (`factorial.hpp`):
   - Computes first `n` factorials using custom `BigInt` class
   - Safe upper limit (100,000) to prevent excessive memory usage
   - Handles arbitrarily large numbers with base-10^9 representation

2. **Stable Deduplication** (`dedup.hpp`):
   - Removes duplicates while preserving original order
   - Works with custom types via hash and equality functors
   - Template-based implementation for type flexibility

3. **Recursive List Reversal** (`singly_list.hpp`):
   - Reverses singly-linked lists using recursion
   - Memory-safe with `std::unique_ptr`
   - Utility functions for vector-list conversion

## 🔧 Implementation Details

### Big Integer Arithmetic
The `BigInt` class implements efficient large integer arithmetic:
- **Base-10^9 representation** for memory efficiency
- **Optimized multiplication** for factorial computation
- **Safe string conversion** with proper zero-padding

### Error Handling
Custom exception hierarchy:
- `AppError` - Base application exception
- `InvalidArgumentError` - Invalid input parameters

### Memory Management
- **RAII principles** throughout
- **Smart pointers** for automatic memory management
- **Move semantics** for efficient resource transfer

## 📊 Algorithm Specifications

### 1. Factorial Computation
**Input**: Natural number `n` (1 ≤ n ≤ 100,000)  
**Output**: Vector containing first `n` factorials (1!, 2!, ..., n!)  
**Complexity**: O(n × m) where m is digits in result

### 2. Stable Deduplication
**Input**: Container of elements (preserves order of first occurrences)  
**Output**: Container with duplicates removed, stable order  
**Complexity**: O(n) average case with hash table

### 3. Recursive List Reversal
**Input**: Singly-linked list  
**Output**: Reversed list using recursion  
**Complexity**: O(n) time, O(n) stack space (recursive)

## 🎯 Design Principles

- **Type Safety**: Strong typing with templates where appropriate
- **Exception Safety**: Basic guarantee for all operations
- **Testability**: Dependency injection and pure functions
- **Modularity**: Separate compilation units with clear interfaces
- **Performance**: Efficient algorithms with reasonable constraints

## 🤝 Contributing

This is a colloquium project demonstrating industrial C++ practices. For educational purposes:

1. Follow existing code style and conventions
2. Add tests for new functionality
3. Maintain exception safety guarantees
4. Document public interfaces
5. Consider performance implications

## 📄 License

Educational project - Belarusian State University, Faculty of Applied Mathematics and Computer Science, Applied Mathematics.