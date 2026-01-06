# Lab Work №4: Process Synchronization via File-Based Ring Queue (C++23)

Belarusian State University, Faculty of Applied Mathematics and Computer Science, Department of Programming Technology  
**Topic**: "Process Synchronization"

## 🚀 Project Overview
This C++23 Windows project implements a multi-process message passing system where multiple Sender processes communicate with a single Receiver process through a shared binary file organized as a ring queue (FIFO). The system uses Windows synchronization primitives (events and mutexes) for inter-process coordination and demonstrates modern C++23 features including `std::expected`, `std::format`, and ranges.

## 📁 Project Structure
```
laba4 c++23/
├── include/
│   ├── core/
│   │   ├── byte_io.h                # Low-level binary file I/O operations
│   │   └── ring_queue_file.h        # Ring queue file implementation
│   ├── ipc/
│   │   ├── constants.h              # Synchronization constants
│   │   ├── process_launcher.h       # Process creation and management
│   │   ├── win_handle.h             # RAII wrapper for Windows HANDLE
│   │   └── win_sync.h               # Windows synchronization primitives
│   └── util/
│       ├── console.h                # Console input/output utilities
│       └── string_trim.h            # String trimming utilities
├── src/
│   ├── core/
│   │   ├── byte_io.cpp              # ByteIO implementation
│   │   └── ring_queue_file.cpp      # RingQueueFile implementation
│   ├── ipc/
│   │   ├── process_launcher.cpp     # Process launcher implementation
│   │   └── win_sync.cpp             # Synchronization objects implementation
│   ├── util/
│   │   ├── console.cpp              # Console utilities implementation
│   │   └── string_trim.cpp          # String trimming implementation
│   ├── receiver.cpp                 # Receiver process main implementation
│   └── sender.cpp                   # Sender process main implementation
├── tests/
│   ├── test_byte_io.cpp             # ByteIO unit tests
│   ├── test_process_launcher.cpp    # ProcessLauncher unit tests
│   ├── test_ring_queue_file.cpp     # RingQueueFile unit tests
│   ├── test_sync.cpp                # SyncObjects unit tests
│   └── test_util.cpp                # Utility functions unit tests
├── CMakeLists.txt                   # Main CMake configuration file
├── CMakePresets.json                # CMake presets for build configuration
├── compilation.txt                  # Compilation instructions
└── README.md                        # This documentation file
```

## 🏗️ Building the Project

### Prerequisites
- CMake 3.20+
- C++23 compatible compiler (MSVC 2022 17.0+ recommended)
- Windows OS (required for Windows synchronization API)
- Ninja build system (optional but recommended)

### Build Instructions
```bash
# Configure the project using CMake presets
cmake --preset default

# Build the project
cmake --build --preset default

# Run unit tests
ctest --preset default --output-on-failure
```

### Manual Build (Alternative)
```bash
mkdir build && cd build
cmake -DCMAKE_CXX_STANDARD=23 -DCMAKE_CXX_STANDARD_REQUIRED=ON ..
cmake --build . --config Release
```

## ✨ Process Architecture

### 1. Receiver Process (`receiver.cpp`)
The main coordinating process that manages the message queue and launches Sender processes:

**Responsibilities:**
- Creates binary file for message storage with specified capacity
- Initializes ring queue structure in file with proper header and slots
- Launches multiple Sender processes using `CreateProcessA`
- Creates Windows synchronization objects (events and mutex) with derived names
- Waits for all Senders to signal readiness using `WaitForMultipleObjects`
- Processes console commands: `read` (read message) and `quit` (exit)

**Synchronization:**
- Creates named events for each Sender to signal readiness
- Uses file mutex for exclusive queue access (10-second timeout)
- Waits on message event when queue is empty
- Signals termination of all Sender processes on exit using `TerminateProcess`

### 2. Sender Process (`sender.cpp`)
Individual processes that send messages to Receiver through the shared file:

**Responsibilities:**
- Opens shared message file using file path from command line arguments
- Signals readiness to Receiver via named event specific to sender ID
- Processes console commands: `send` (send message) and `exit` (terminate)
- Validates message length (max 19 characters) before sending
- Handles queue full conditions with 3-second retry delay

**Synchronization:**
- Opens synchronization objects created by Receiver using shared prefix
- Acquires file mutex before accessing queue (10-second timeout)
- Signals message event after successful message delivery using `SetEvent`

## 🧪 Testing

The project includes comprehensive unit tests using Google Test:

```bash
# Run all tests from build directory
./unit_tests.exe

# Run specific test suites
./unit_tests.exe --gtest_filter="ByteIOTest*"
./unit_tests.exe --gtest_filter="RingQueueFileTest*"
./unit_tests.exe --gtest_filter="SyncTest*"
./unit_tests.exe --gtest_filter="UtilTest*"
```

**Test Coverage:**
- **ByteIO Tests**: File I/O operations, error handling, and file resizing
- **RingQueueFile Tests**: Queue operations (push/pop), full/empty conditions, boundary cases, FIFO ordering across wrap-around
- **SyncObject Tests**: Windows synchronization primitives creation, signaling, and timeout handling
- **Utility Tests**: String trimming with various edge cases

## 📖 Usage Example

### Step 1: Start Receiver Process
```bash
# Build the project first, then run receiver from build directory
./receiver.exe

# Example session:
Receiver: enter binary file name:
messages.dat
Receiver: enter capacity (number of records):
10
Receiver: enter number of sender processes:
3
Path to sender.exe (leave empty for sender.exe in current directory):

Waiting for all senders to be ready...
Receiver ready. Commands: read | quit
```

### Step 2: Sender Consoles (automatically launched)
Each Sender process displays:
```
Sender 0 ready. Commands: send | exit
```

### Step 3: Send Messages from Sender
```
Commands: send | exit
send
Enter message (<20 chars):
Hello from Sender 0
Sent
```

### Step 4: Read Messages in Receiver
```
Commands: read | quit
read
Message: Hello from Sender 0
```

### Step 5: Handle Queue Full Condition
When a Sender tries to send to a full queue:
```
Queue full, waiting...
[waits 3 seconds and retries]
```

### Step 6: Handle Queue Empty Condition
When Receiver tries to read from empty queue:
```
Queue empty, waiting for message...
[waits for message event signal]
```

## 🔧 Implementation Details

### Modern C++23 Features
- **`std::expected`**: Modern error handling without exceptions for all operations
- **`std::format`**: Type-safe string formatting throughout the codebase
- **Ranges**: Used in string trimming with `std::ranges::find_if_not`
- **`std::string_view`**: Non-owning string references for efficiency
- **`std::filesystem`**: Modern file system operations
- **Attributes**: `[[nodiscard]]` and `noexcept` for better API design

### Ring Queue File Format
- **File Header**: Capacity, count, head, tail (32 bytes on 64-bit systems)
- **Message Records**: Fixed-size slots of 20 bytes each (max 19 chars + null terminator)
- **Memory Layout**: Header followed by capacity × 20 bytes of message slots
- **Circular Buffer**: Head and tail indices wrap around using modulo arithmetic

### Windows Synchronization Objects
- **Named Mutex**: Ensures exclusive file access for queue operations
- **Named Events**: Used for Sender readiness signaling and message notifications
- **RAII Wrapper**: `unique_handle` class provides automatic Windows handle management
- **Timeout Handling**: 10-second default timeout for all synchronization operations

### Error Handling Strategy
- **`std::expected<T, E>`**: Used for all operations that can fail
- **No Exceptions**: Error propagation through return values
- **Win32 Error Codes**: Converted to human-readable strings with `std::format`
- **Input Validation**: Range checking for all user inputs and command-line arguments

## 📊 Key Features
- **Multi-Process Architecture**: One Receiver with multiple concurrent Senders
- **File-Based Communication**: Persistent message storage in binary file
- **Ring Queue Implementation**: FIFO message ordering with fixed capacity
- **Windows Synchronization**: Named events and mutexes for process coordination
- **Modern C++23**: Uses latest C++ standard features including `std::expected` and `std::format`
- **RAII Resource Management**: Automatic cleanup of Windows handles and file streams
- **Console Interface**: Interactive command-line control for both processes
- **Comprehensive Testing**: Unit tests for all core components with Google Test
- **Error Handling**: Modern error propagation without exceptions

## 🆚 Comparison: C++11 vs C++23 Implementations

| Feature | C++11 Implementation | C++23 Implementation |
|---------|--------------------------------|--------------------------------|
| **Error Handling** | Boolean return values + output error strings | `std::expected<T, E>` with type-safe error propagation |
| **String Formatting** | `std::ostringstream` or manual concatenation | `std::format` with compile-time format checking |
| **String Processing** | Manual loops with `std::isspace` | Ranges with `std::ranges::find_if_not` |
| **File System Operations** | `std::fstream` with manual path handling | `std::filesystem` with modern path operations |
| **String Views** | `const std::string&` parameters | `std::string_view` for non-owning string references |
| **Attributes** | Limited or no use of attributes | `[[nodiscard]]`, `noexcept` for better API contracts |
| **Type Safety** | Manual error codes and nullptr checks | Compile-time type checking with modern return types |
| **Code Clarity** | Mixed error handling approaches | Consistent error handling pattern throughout |
| **Resource Management** | Manual handle management or custom RAII | `unique_handle` with move semantics and automatic cleanup |
| **Compile-time Checks** | Basic syntax checking | Enhanced type safety with modern C++ features |
| **Standard Library Usage** | C++11 standard library features | C++17/C++20/C++23 features integrated throughout |

### Key Improvements in C++23 Version:

1. **Modern Error Handling**: `std::expected` provides a type-safe alternative to exceptions and error codes
2. **Type-Safe Formatting**: `std::format` eliminates format string vulnerabilities and improves readability
3. **Expressive Algorithms**: Ranges make algorithms like string trimming more readable and efficient
4. **Better Resource Management**: Improved RAII patterns with move-only types
5. **Enhanced API Design**: `[[nodiscard]]` ensures errors aren't ignored accidentally
6. **File System Modernization**: `std::filesystem` provides portable, type-safe file operations
7. **Performance Optimizations**: `std::string_view` reduces unnecessary string copies

### Backward Compatibility Considerations:
- The C++23 version requires a compiler with C++23 support (MSVC 2022 17.0+, GCC 13+, Clang 16+)
- Some C++23 features (`std::expected`, `std::format`) may not be available in older compilers
- Windows API usage remains similar, but wrapped in modern C++ interfaces
- The core architecture (Receiver-Sender with file-based queue) remains identical

## 🎯 Design Principles
- **Modern C++ Best Practices**: Leverage C++23 features for cleaner, safer code
- **Resource Safety**: RAII patterns for all system resources (handles, files, memory)
- **Thread-Safe Operations**: Proper synchronization for concurrent file access
- **Error Resilience**: Graceful handling of invalid inputs and system errors
- **Testability**: Modular design enabling comprehensive unit testing
- **Performance**: Efficient algorithms with minimal copying and allocations
- **Maintainability**: Clear separation of concerns and consistent coding style

## 🤝 Development Notes
This implementation demonstrates modern Windows process synchronization techniques using C++23 features. The project showcases:

- **Modern C++23**: `std::expected` for error handling, `std::format` for string formatting, ranges for algorithms
- **Windows API Integration**: `CreateProcessA`, `CreateEventA`, `CreateMutexA`, `WaitForMultipleObjects`
- **Inter-Process Communication**: Shared file as communication medium with proper synchronization
- **Process Management**: Launching and terminating child processes with proper cleanup
- **File-Based Data Structures**: Implementing efficient ring queue in binary file
- **RAII Patterns**: Custom `unique_handle` class for Windows handle management

**Note**: The system uses 10-second timeouts for synchronization operations to prevent deadlocks. The message length limit of 19 characters ensures fixed-size records for efficient queue implementation. The project requires Windows due to the use of Windows synchronization primitives.

## 📄 License
Educational project - Belarusian State University, Faculty of Applied Mathematics and Computer Science, Apllied Mathematics.