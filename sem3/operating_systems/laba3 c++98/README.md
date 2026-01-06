# Lab Work №3: Thread Synchronization with Critical Sections and Events

**Belarusian State University, Faculty of Applied Mathematics and Computer Science, Department of Programming Technology**  
**Topic:** "Thread Synchronization Using Critical Sections and Events. Deadlock Handling"

## 🚀 Project Overview

This C++98 project demonstrates advanced thread synchronization techniques using Windows synchronization primitives. The program implements a multi-threaded system where marker threads concurrently access a shared array while being managed by a main thread using critical sections and event objects for coordination, deadlock prevention, and controlled termination.

## 📁 Project Structure

```
laba3 c++98/
├── CMakeLists.txt          # Main CMake configuration file
├── CMakePresets.json       # CMake presets for build configuration
├── compilation.txt         # Compilation instructions
├── boost/                  # Boost headers (header-only, used for shared_ptr)
├── include/                # Header files
│   ├── constants.hpp       # Timing constants
│   ├── marker.hpp          # Marker thread class declaration
│   ├── print_utils.hpp     # Array printing utilities
│   └── shared_array.hpp    # Thread-safe array implementation
├── src/                    # Source code
│   ├── main.cpp            # Main thread implementation
│   └── marker.cpp          # Marker thread implementation
├── tests/                  # Test suite
│   ├── runner.cpp          # Test runner utility
│   ├── test_array.cpp      # Shared array tests
│   ├── test_marker.cpp     # Marker thread tests
│   └── test_sync.cpp       # Synchronization primitive tests
└── README.md               # This documentation file
```

## 🏗️ Building the Project

### Prerequisites
- **CMake 3.20+**
- **C++98 compatible compiler** (MSVC recommended for Windows API compatibility)
- **Windows OS** (required for Windows synchronization primitives)
- **Boost headers** (included in project, header-only usage)

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
cmake -DCMAKE_CXX_STANDARD=98 -DCMAKE_BUILD_TYPE=Debug ..
cmake --build .
```

## ✨ Synchronization Architecture

### 1. Main Thread (`src/main.cpp`)
Manages the entire synchronization workflow:
- Creates and initializes a shared integer array
- Spawns multiple marker threads with sequential IDs
- Coordinates thread execution using event objects
- Monitors thread states and handles controlled termination
- Implements deadlock detection and resolution

### 2. Marker Threads (`src/marker.cpp`)
Concurrently access the shared array:
- Generate random indices to access array elements
- Mark zero-valued elements with thread ID
- Detect conflicts when accessing non-zero elements
- Signal blocking states to main thread
- Respond to termination or continuation signals
- Clean up marked elements upon termination

## 🧪 Testing Framework

The project includes a comprehensive test suite with custom test runner:

```bash
# Run individual test executables
./build/test_array.exe
./build/test_sync.exe  
./build/test_marker.exe

# Run all tests via the test runner
./build/test_runner.exe
```

### Test Components:
- **Shared Array Tests**: Thread-safe array operations and boundary checks
- **Synchronization Tests**: Windows API thread synchronization primitives
- **Marker Thread Tests**: Complete marker lifecycle and conflict handling
- **Integration Tests**: Full system behavior through custom test runner

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

### Synchronization Primitives
- **Critical Sections**: `CRITICAL_SECTION` for protecting shared array access
- **Event Objects**: `CreateEvent`, `SetEvent`, `ResetEvent`, `WaitForSingleObject`
- **Thread Management**: `CreateThread`, `WaitForMultipleObjects`, `CloseHandle`
- **Atomic Operations**: `InterlockedIncrement` for thread-safe counters

### Thread Coordination
- **Start Synchronization**: All marker threads wait on a single start event
- **Block Detection**: Marker threads signal when they cannot proceed
- **Controlled Termination**: Main thread selects which marker to terminate
- **Deadlock Prevention**: Timeouts and careful event signaling

### C++98 Specifics
- **Manual Memory Management**: Raw pointers and manual cleanup
- **No Standard Smart Pointers**: Using Boost `shared_ptr` for reference counting
- **Exception Safety**: Basic exception handling without modern features
- **Traditional Iteration**: Index-based loops instead of range-based for

## 📊 Key Features

- **Thread-Safe Shared Array**: Protected by critical sections with bounds checking
- **Event-Based Communication**: Signal-based thread coordination
- **Deadlock Handling**: Detection and resolution of thread conflicts
- **Controlled Termination**: Graceful shutdown of selected threads
- **Comprehensive Testing**: Custom test framework with runner utility
- **C++98 Compliance**: Compatible with legacy codebases and systems

## 🎯 Design Principles

- **Thread Safety**: All shared data protected by synchronization primitives
- **Resource Management**: Proper cleanup of Windows handles and memory
- **Error Handling**: Windows API error checking and reporting
- **Predictable Behavior**: Deterministic thread coordination
- **Testability**: Modular design enabling comprehensive testing

## 🚫 Deadlock Prevention Strategies

The implementation incorporates several deadlock prevention techniques:

1. **Timeout Mechanisms**: Limited waits on synchronization objects
2. **Ordered Resource Acquisition**: Consistent locking patterns
3. **Signal Hierarchy**: Clear signaling protocol between threads
4. **Conflict Detection**: Early detection of unresolvable conflicts
5. **Controlled Backoff**: Randomized delays to break contention patterns

## 🤝 Development Notes

This implementation demonstrates classic Windows threading and synchronization patterns, showcasing:

1. **Low-Level Synchronization**: Direct Windows API usage for maximum control
2. **Legacy C++ Compatibility**: C++98 compliance for educational and legacy systems
3. **Robust Error Handling**: Comprehensive Windows error code checking
4. **Resource Safety**: Manual but thorough resource cleanup
5. **Complex Coordination**: Multi-threaded system with sophisticated state management

**Note**: The 5ms sleep intervals in the marker threads simulate processing time as specified in the lab requirements. The implementation focuses on demonstrating synchronization concepts rather than performance optimization.

## 📄 License

Educational project - Belarusian State University, Faculty of Applied Mathematics and Computer Science, Applied Mathematics.