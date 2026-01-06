# Lab Work №2: Thread Creation (C++20 Modern Threads)

**Belarusian State University, Faculty of Applied Mathematics and Computer Science, Department of Programming Technology**  
**Topic:** "Thread Creation"

## 🚀 Project Overview

This C++20 project demonstrates modern thread creation and synchronization using the C++ Standard Library. The program creates three threads (main, min_max, and average) that operate on a shared array of integers, implementing a concurrent processing workflow with modern C++20 features including `std::jthread`, ranges, and chrono utilities.

## 📁 Project Structure

```
laba2 c++20/
├── CMakeLists.txt          # Main CMake configuration file
├── CMakePresets.json       # CMake presets for build configuration
├── compilation.txt         # Compilation instructions
├── common/                 # Shared components
│   ├── shared_data.h       # Shared data structure for thread communication
│   ├── threads.h           # Thread function declarations
│   ├── utils.h             # Utility function declarations
│   └── utils.cpp           # Utility function implementations
├── src/                    # Main application source code
│   ├── main.cpp            # Main thread implementation using std::jthread
│   ├── thread_average.cpp  # Average calculation thread
│   └── thread_min_max.cpp  # Min/max calculation thread
├── tests/                  # Unit tests directory
│   ├── test_average.cpp    # Average calculation tests
│   ├── test_input.cpp      # Input/output tests
│   ├── test_min_max.cpp    # Min/max calculation tests
│   └── test_replace.cpp    # Array replacement tests
└── README.md               # This documentation file
```

## 🏗️ Building the Project

### Prerequisites
- **CMake 3.15+**
- **C++20 compatible compiler** (MSVC 2019+, GCC 10+, Clang 10+)
- **Ninja build system** (recommended)

### Build Instructions

```bash
# Configure the project using CMake presets
cmake --preset=default

# Build the project
cmake --build --preset=default

# Run tests
ctest --preset=default --output-on-failure
```

### Alternative Manual Build

```bash
mkdir build && cd build
cmake -DCMAKE_CXX_STANDARD=20 -DCMAKE_CXX_STANDARD_REQUIRED=ON ..
cmake --build . --config Release
```

## ✨ Modern C++20 Thread Architecture

### 1. Main Thread (`src/main.cpp`)
Uses modern C++20 threading:
- Reads integer array from console input
- Creates `min_max` and `average` threads using `std::jthread` (automatic resource management)
- Automatically joins threads on destruction (RAII)
- Replaces min/max elements with average value
- Outputs modified array

### 2. Min_Max Thread (`src/thread_min_max.cpp`)
- Finds minimum and maximum values using `std::ranges::minmax_element`
- Uses `std::this_thread::sleep_for` for delays
- Outputs results to console

### 3. Average Thread (`src/thread_average.cpp`)
- Calculates arithmetic average using `std::accumulate`
- Uses `std::this_thread::sleep_for` for delays
- Outputs result to console

## 🧪 Testing

The project includes comprehensive unit tests using Google Test:

```bash
# Run all tests
./build/run_tests

# Run specific test suites
./build/run_tests --gtest_filter="AverageTest*"
./build/run_tests --gtest_filter="MinMaxTest*"
./build/run_tests --gtest_filter="InputOutputTest*"
./build/run_tests --gtest_filter="ReplaceTest*"
```

### Test Coverage:
- **Average Calculation**: Correct computation and edge cases
- **Min/Max Calculation**: Proper identification using modern algorithms
- **Input/Output**: Console I/O validation
- **Array Replacement**: Correct substitution of min/max with average

## 📖 Usage Example

```bash
# Run the main application
./build/lab2

# Example session:
# Enter the number of elements: 5
# Enter the elements:
# 10 20 30 40 50
# 
# Thread MinMax: min = 10, max = 50
# Thread Average: average = 30
# Result: 30 20 30 40 30
# 
# Press any key to exit...
```

## 🔧 Implementation Details

### Modern Thread Management
- **`std::jthread`**: C++20 joining thread with automatic resource management
- **RAII Principles**: Automatic thread joining on destruction
- **`std::this_thread::sleep_for`**: Standard chrono-based timing
- **No manual cleanup**: Eliminates `CloseHandle` requirements

### C++20 Features
- **Ranges**: `std::ranges::minmax_element` for cleaner algorithms
- **Chrono Utilities**: Standard timing facilities
- **Standard Algorithms**: `std::accumulate`, clean iteration patterns
- **Uniform Initialization**: Modern initialization syntax

### Shared Data Structure
Simplified `SharedData` structure:
- Thread-safe design for concurrent access
- Uses standard C++ types only
- No platform-specific handle types
- Clean initialization with default member initializers

## 📊 Key Features

- **Modern C++20**: Leverages latest C++ standard features
- **Standard Library Threading**: Cross-platform thread management
- **RAII Resource Management**: Automatic cleanup with `std::jthread`
- **Ranges and Algorithms**: Clean, expressive operations
- **Comprehensive Testing**: Full unit test coverage
- **Cross-Platform Design**: Portable code (except for `_getch`)

## 🆚 Comparison with Windows API Version

| Feature | Windows API (C++11) | Standard C++20 |
|---------|---------------------|----------------|
| **Thread Creation** | `CreateThread` | `std::jthread` |
| **Synchronization** | `WaitForSingleObject` | Automatic join |
| **Sleep** | `Sleep()` | `std::this_thread::sleep_for` |
| **Resource Management** | Manual `CloseHandle` | RAII automatic |
| **Portability** | Windows only | Cross-platform |
| **Error Handling** | Return codes | Exceptions |

## 🎯 Design Principles

- **Modern C++ Best Practices**: Use latest language features
- **RAII Everywhere**: Automatic resource management
- **Exception Safety**: Strong exception guarantees
- **Expressive Code**: Clear intent through standard algorithms
- **Testability**: Modular design for comprehensive testing

## 🤝 Development Notes

This implementation demonstrates modern C++20 threading as an alternative to the Windows API approach, showcasing:

1. **Standard Library Concurrency**: Portable thread management
2. **C++20 Features**: Ranges, chrono, jthread
3. **Resource Safety**: Automatic cleanup with RAII
4. **Expressive Algorithms**: Clear intent through standard functions

**Note**: The 50ms sleep intervals in the threads simulate the required delays (7ms and 12ms from the lab specification) but use modern `std::chrono` facilities. The implementation focuses on demonstrating modern C++ practices rather than exact timing requirements.

## 📄 License

Educational project - Belarusian State University, Faculty of Applied Mathematics and Computer Science, Applied Mathematics.