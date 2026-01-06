# Lab Work №3: Thread Synchronization (C++17 Modern Implementation)

**Belarusian State University, Faculty of Applied Mathematics and Computer Science, Department of Programming Technology**  
**Topic:** "Thread Synchronization Using Modern C++ Concurrency Primitives. Deadlock Handling"

## 🚀 Project Overview

This C++17 project demonstrates advanced thread synchronization using modern C++ standard library concurrency features. The program implements a multi-threaded system where marker threads concurrently access a shared array while being managed by a main thread using mutexes, condition variables, and atomic operations for coordination, deadlock prevention, and controlled termination.

## 📁 Project Structure

```
laba3 c++17/
├── CMakeLists.txt          # Main CMake configuration file
├── CMakePresets.json       # CMake presets for build configuration
├── compilation.txt         # Compilation instructions
├── include/                # Header files
│   ├── constants.hpp       # Timing constants (chrono durations)
│   ├── marker.hpp          # Marker thread class declaration
│   ├── print_utils.hpp     # Array printing utilities
│   └── shared_array.hpp    # Thread-aware array implementation
├── src/                    # Source code
│   ├── Main.cpp            # Main thread implementation
│   └── marker.cpp          # Marker thread implementation
├── tests/                  # Test suite
│   ├── test_array.cpp      # Shared array tests
│   ├── test_marker.cpp     # Marker thread tests
│   └── test_sync.cpp       # Synchronization primitive tests
└── README.md               # This documentation file
```

## 🏗️ Building the Project

### Prerequisites
- **CMake 3.16+**
- **C++17 compatible compiler** (MSVC 2017+, GCC 7+, Clang 5+)
- **Cross-platform**: Works on Windows, Linux, and macOS

### Build Instructions

```bash
# Configure the project with debug preset
cmake --preset=debug

# Build the project
cmake --build --preset=debug

# Run all tests
ctest --preset=debug --output-on-failure
```

### Alternative Manual Build

```bash
mkdir build && cd build
cmake -DCMAKE_CXX_STANDARD=17 -DCMAKE_BUILD_TYPE=Debug ..
cmake --build .
```

## ✨ Modern C++17 Synchronization Architecture

### 1. Main Thread (`src/Main.cpp`)
Orchestrates the synchronization workflow using modern C++:
- Creates and initializes a shared integer array
- Spawns multiple marker threads with sequential IDs using `std::thread`
- Coordinates thread execution using `std::condition_variable` and `std::mutex`
- Monitors thread states and handles controlled termination
- Implements deadlock detection and resolution

### 2. Marker Threads (`src/marker.cpp`)
Concurrently access the shared array using modern C++ concurrency:
- Generate random indices using C++11 random number generators
- Mark zero-valued elements with thread ID
- Detect conflicts when accessing non-zero elements
- Signal blocking states using condition variables
- Respond to termination or continuation signals
- Clean up marked elements upon termination

## 🧪 Testing Framework

The project includes comprehensive unit tests using Google Test:

```bash
# Run all tests via CTest
ctest --preset=debug

# Or run the test executable directly
./build/unit_tests.exe
```

### Test Components:
- **Shared Array Tests**: Array operations and boundary checks
- **Synchronization Tests**: Modern C++ thread synchronization primitives
- **Marker Thread Tests**: Complete marker lifecycle and conflict handling
- **Integration Tests**: Full system behavior through Google Test framework

## 📖 Usage Example

```bash
# Run the main application
./build/lab3.exe

# Example session:
# Enter array size: 10
# Enter number of markers: 3
#
# [Marker threads begin concurrent execution]
# Marker 2 blocked; marked=3; conflict_index=5
# Marker 1 blocked; marked=2; conflict_index=7
# Marker 3 blocked; marked=4; conflict_index=2
#
# All active markers blocked
# Array: 1 0 3 0 2 0 1 0 3 0
# ----------------------------------------
# Enter marker id to terminate: 2
#
# Marker 2 terminated
# Array: 1 0 3 0 0 0 1 0 3 0
# ----------------------------------------
# [Remaining threads continue...]
```

## 🔧 Implementation Details

### Modern Synchronization Primitives
- **Mutexes**: `std::mutex` for protecting shared array access
- **Condition Variables**: `std::condition_variable` for thread signaling and waiting
- **Atomic Operations**: `std::atomic` for lock-free counters and flags
- **Thread Management**: `std::thread`, `std::jthread`-like RAII wrapper
- **Random Number Generation**: `std::mt19937` and `std::uniform_int_distribution`

### Thread Coordination
- **Start Synchronization**: All marker threads wait on a condition variable
- **Block Detection**: Marker threads signal when they cannot proceed
- **Controlled Termination**: Main thread selects which marker to terminate
- **Deadlock Prevention**: Timeouts and careful signaling patterns

### C++17 Features
- **RAII Resource Management**: Automatic cleanup with destructors
- **Smart Pointers**: `std::unique_ptr` for exclusive ownership
- **Atomic Types**: `std::atomic` for thread-safe shared variables
- **Chrono Library**: `std::chrono` for precise timing
- **Optional Values**: `std::optional` for nullable returns
- **Random Number Facilities**: Modern random number generators

## 📊 Key Features

- **Modern Thread-Safe Array**: Protected by mutexes with bounds checking
- **Condition Variable Communication**: Signal-based thread coordination
- **Deadlock Handling**: Detection and resolution of thread conflicts
- **Controlled Termination**: Graceful shutdown of selected threads
- **Comprehensive Testing**: Google Test integration with modern framework
- **Cross-Platform Compatibility**: Standard C++17 without platform-specific code
- **RAII Resource Management**: Automatic cleanup of all resources

## 🆚 Comparison with Windows API Version

| Feature | Windows API (C++98) | Standard C++17 |
|---------|---------------------|----------------|
| **Synchronization** | Critical Sections, Events | Mutexes, Condition Variables |
| **Thread Creation** | `CreateThread` | `std::thread` |
| **Timing** | `Sleep()` | `std::this_thread::sleep_for` |
| **Random Numbers** | `rand()`, `srand()` | `std::mt19937` |
| **Atomic Operations** | `InterlockedIncrement` | `std::atomic` |
| **Portability** | Windows only | Cross-platform |
| **Resource Management** | Manual cleanup | RAII automatic |

## 🎯 Design Principles

- **Thread Safety**: All shared data protected by synchronization primitives
- **RAII Everywhere**: Automatic resource management with destructors
- **Exception Safety**: Strong exception guarantees with proper cleanup
- **Modern C++ Best Practices**: Use of latest language features
- **Testability**: Modular design enabling comprehensive testing
- **Cross-Platform Design**: No platform-specific code in core logic

## 🚫 Deadlock Prevention Strategies

The implementation incorporates several modern deadlock prevention techniques:

1. **Lock Hierarchies**: Consistent mutex acquisition order
2. **Timeout Mechanisms**: Limited waits on condition variables
3. **Atomic Flags**: Lock-free communication where possible
4. **Conflict Detection**: Early detection of unresolvable conflicts
5. **Controlled Backoff**: Randomized delays to break contention patterns

## 🤝 Development Notes

This implementation demonstrates modern C++ threading and synchronization patterns, showcasing:

1. **Standard Library Concurrency**: Portable thread management across platforms
2. **Modern C++ Features**: C++17 compliance with best practices
3. **RAII Resource Management**: Automatic cleanup with destructors
4. **Exception Safety**: Proper exception handling with resource cleanup
5. **Complex Coordination**: Multi-threaded system with sophisticated state management

**Note**: The 5ms sleep intervals in the marker threads use `std::chrono::milliseconds` as specified in the lab requirements. The implementation focuses on demonstrating modern synchronization concepts with cross-platform compatibility.

## 📄 License

Educational project - Belarusian State University, Faculty of Applied Mathematics and Computer Science, Applied Mathematics.