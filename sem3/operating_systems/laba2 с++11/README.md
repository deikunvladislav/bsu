# Lab Work №2: Thread Creation (C++11 Windows Threads)

**Belarusian State University, Faculty of Applied Mathematics and Computer Science, Department of Programming Technology**   
**Topic:** "Thread Creation"

## 🚀 Project Overview

This C++11 project demonstrates Windows thread creation and synchronization using the Windows Thread API. The program creates three threads (main, min_max, and average) that operate on a shared array of integers, implementing a concurrent processing workflow with proper synchronization and error handling.

## 📁 Project Structure

```
laba2_cxx11/
├── CMakeLists.txt          # Main CMake configuration file
├── CMakePresets.json       # CMake presets for build configuration
├── compilation.txt         # Compilation instructions
├── common/                 # Shared components
│   ├── shared_data.h       # Shared data structure for thread communication
│   ├── threads.h           # Thread function declarations
│   ├── utils.h             # Utility function declarations
│   └── utils.cpp           # Utility function implementations
├── src/                    # Main application source code
│   ├── main.cpp            # Main thread implementation
│   ├── thread_average.cpp  # Average calculation thread
│   └── thread_min_max.cpp  # Min/max calculation thread
├── tests/                  # Unit tests directory
│   ├── test_average.cpp    # Average calculation tests
│   ├── test_input.cpp      # Input validation tests
│   ├── test_min_max.cpp    # Min/max calculation tests
│   └── test_replace.cpp    # Array replacement tests
└── README.md               # This documentation file
```

## 🏗️ Building the Project

### Prerequisites
- **CMake 3.15+**
- **C++11 compatible compiler** (MSVC recommended for Windows)
- **Windows OS** (required for Windows Thread API)

### Build Instructions

```bash
# Configure the project
cmake --preset=default

# Build the project
cmake --build --preset=default

# Run tests
ctest --preset=default --output-on-failure
```

### Manual Build (Alternative)

```bash
mkdir build && cd build
cmake -DCMAKE_CXX_STANDARD=11 -DCMAKE_CXX_STANDARD_REQUIRED=ON ..
cmake --build . --config Release
```

## ✨ Thread Architecture

### 1. Main Thread (`src/main.cpp`)
Orchestrates the entire workflow:
- Reads integer array from console input
- Creates `min_max` and `average` threads using `CreateThread`
- Waits for thread completion using `WaitForSingleObject` with `INFINITE`
- Replaces min/max elements with average value
- Outputs modified array

### 2. Min_Max Thread (`src/thread_min_max.cpp`)
- Finds minimum and maximum values in the array
- Sleeps 7 milliseconds after each comparison (`Sleep(7)`)
- Outputs results to console

### 3. Average Thread (`src/thread_average.cpp`)
- Calculates arithmetic average of array elements
- Sleeps 12 milliseconds after each summation (`Sleep(12)`)
- Outputs result to console

## 🧪 Testing

The project includes comprehensive unit tests:

```bash
# Run all tests
./build/run_tests

# Run specific test suites
./build/run_tests --gtest_filter="AverageTest*"
./build/run_tests --gtest_filter="MinMaxTest*"
./build/run_tests --gtest_filter="InputTest*"
./build/run_tests --gtest_filter="ReplaceTest*"
```

### Test Coverage:
- **Average Calculation**: Correct computation and empty array handling
- **Min/Max Calculation**: Proper identification of extreme values
- **Input Validation**: Array size limits and integer parsing
- **Array Replacement**: Correct substitution of min/max with average

## 📖 Usage Example

```bash
# Run the main application
./build/lab2

# Example session:
# Enter array size: 5
# Enter 5 integers:
# 10 20 30 40 50
# 
# Output from threads (order may vary):
# Min: 10, Max: 50
# Average: 30
# 
# Modified array: 30 20 30 40 30
```

## 🔧 Implementation Details

### Thread Synchronization
- **`CreateThread`**: Windows API for thread creation
- **`WaitForSingleObject`**: Thread synchronization with `INFINITE` timeout
- **`Sleep`**: Thread suspension for specified milliseconds
- **Proper handle cleanup**: `CloseHandle` for thread handles

### Shared Data Structure
The `SharedData` structure enables safe data sharing between threads:
- Thread-safe design for concurrent access
- Contains array, min/max values, average, and thread handles
- Used by all three threads with proper synchronization

### Error Handling
- Exception-based error propagation
- Input validation for array size (1-1000 elements)
- Integer parsing with error detection
- Thread creation failure handling

## 📊 Key Features

- **Windows Thread API**: Demonstrates low-level thread management
- **Concurrent Processing**: Parallel min/max and average calculations
- **Thread Synchronization**: Proper waiting and resource cleanup
- **Safe Shared Memory**: Structured data sharing between threads
- **Comprehensive Testing**: Unit tests for all components
- **C++11 Compliance**: Modern C++ features with standard library

## 🎯 Design Principles

- **Separation of Concerns**: Each thread has a single responsibility
- **Resource Management**: RAII principles for Windows handles
- **Error Safety**: Exception safety and proper cleanup
- **Testability**: Modular design for unit testing
- **Performance**: Efficient algorithms with intentional delays for demonstration

## 🤝 Development Notes

This implementation specifically demonstrates Windows thread management APIs rather than standard C++ threads (`std::thread`) to meet the lab requirements. The project showcases:

1. **Windows-specific APIs**: `CreateThread`, `WaitForSingleObject`, `Sleep`, `CloseHandle`
2. **C++11 features**: Smart pointers, standard containers, type safety
3. **Modern CMake**: Cross-platform build configuration
4. **Professional testing**: Google Test integration

**Note**: The 7ms and 12ms sleep intervals in the threads are intentionally added to demonstrate thread timing and synchronization, not for performance optimization.

## 📄 License

Educational project - Belarusian State University, Faculty of Applied Mathematics and Computer Science, Applied Mathematics.