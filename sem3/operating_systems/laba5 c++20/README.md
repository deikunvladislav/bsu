# Lab Work №5: Named Pipe Communication with Record Locking (C++20)

**Belarusian State University, Faculty of Applied Mathematics and Computer Science, Department of Programming Technology**  
**Topic: "Data Exchange via Named Pipes"**

## 🚀 Project Overview

This C++20 Windows project implements a client-server system for managing concurrent access to an employee database file using Windows named pipes. Building upon the C++17 version, this implementation leverages modern C++20 features while maintaining the core functionality of fine-grained record locking, concurrent client access, and robust error handling.

### **Key Differences from C++17 Version**
- **C++20 Standard**: Utilizes the latest C++ standard with improved language features
- **Enhanced CMake Configuration**: More robust build system with presets
- **Extended Testing Suite**: Additional unit tests for comprehensive coverage
- **Modern C++ Practices**: Updated code patterns and conventions
- **Improved Protocol Handling**: Enhanced request/response management

## 📁 Project Structure

```
laba5 c++20/
├── include/                      # Header files
│   ├── console_io.hpp            # Console input/output utilities
│   ├── employee.hpp              # Employee record structure
│   ├── file_storage.hpp          # Binary file storage operations
│   ├── handle.hpp                # RAII wrapper for Windows HANDLE
│   ├── pipe.hpp                  # Named pipe server and client
│   ├── protocol.hpp              # Communication protocol definition
│   ├── rw_lock.hpp               # Read-write lock implementation
│   └── server_session.hpp        # Server session management
├── server/                       # Server application
│   ├── server_main.cpp           # Server entry point
│   ├── server_app.cpp            # Server implementation
│   └── server_session.cpp        # Server session logic
├── client/                       # Client application
│   ├── client_main.cpp           # Client entry point
│   └── client_app.cpp            # Client implementation
├── tests/                        # Unit tests
│   ├── test_employee.cpp         # Employee record tests
│   ├── test_file_storage.cpp     # File storage operations tests
│   ├── test_console_io.cpp       # Console I/O tests
│   ├── test_rw_lock.cpp          # Read-write lock tests
│   ├── test_pipe.cpp             # Named pipe communication tests
│   └── test_write_commit.cpp     # Write commit flow tests
├── CMakeLists.txt                # Main CMake configuration (C++20)
├── CMakePresets.json             # CMake presets configuration
├── compilation.txt               # Build instructions
└── README.md                     # This documentation file
```

## 🏗️ Building the Project

### **Prerequisites**
- CMake 3.20+ (required for C++20 presets)
- C++20 compatible compiler (MSVC 2022+ or GCC 11+ with MinGW-w64)
- Windows OS (required for Windows named pipes)
- Ninja build system (recommended)

### **Build Instructions**
```bash
# Configure the project using CMake presets
cmake --preset default

# Build the project
cmake --build --preset default

# Run unit tests
ctest --preset default --output-on-failure
```

### **Manual Build (Alternative)**
```bash
mkdir build && cd build
cmake -DCMAKE_CXX_STANDARD=20 -DCMAKE_CXX_STANDARD_REQUIRED=ON ..
cmake --build . --config Release
```

## ✨ System Architecture

### **1. Server Process (`server/`)**
The central coordinator that manages employee records and client connections with enhanced C++20 patterns:

**Responsibilities:**
- Creates binary file for employee records with structure:
  ```cpp
  struct employee {
      int num;            // Employee ID
      char name[10];      // Employee name (max 9 chars + null)
      double hours;       // Worked hours
  };
  ```
- Initializes file with records provided by user
- Creates named pipe (`\\.\pipe\employee_pipe`) for client communication
- Implements per-record read-write locking with C++20 synchronization primitives
- Processes client requests via custom protocol with improved error handling
- Manages multiple concurrent client sessions
- Shows modified file contents after all clients disconnect

**Key Features (C++20 Enhanced):**
- Uses `std::atomic` with improved memory ordering
- Enhanced thread management with `std::jthread`-like patterns
- Better exception safety with modern C++ practices
- Support for up to 16 concurrent clients (configurable)
- 5-second timeout for pipe operations
- Automatic client index assignment and management
- Graceful shutdown handling

### **2. Client Process (`client/`)**
Individual processes that connect to server via named pipes with improved user interaction:

**Responsibilities:**
- Connects to server via named pipe using C++20 RAII patterns
- Provides interactive menu with enhanced input validation:
  1. Modify record (requires write lock)
  2. Read record (requires read lock)
  3. Exit
- Implements two-phase locking for modifications with confirmation prompts
- Implements shared locking for reads with proper lock release
- Handles connection failures gracefully

**User Interaction Enhancements:**
- ID-based record access with validation
- Confirmation prompts for modifications with rollback capability
- Real-time feedback from server operations
- Improved error messages and recovery

## 🔧 Implementation Details

### **Communication Protocol**
The system uses a custom binary protocol over named pipes with C++20 type safety:

**Request Structure:**
```cpp
enum class Op : uint8_t {
    ReadBegin = 1,    // Request read lock and record
    ReadEnd = 2,      // Release read lock
    WriteStart = 3,   // Request write lock and record
    WriteCommit = 4,  // Commit modified record
    Release = 5,      // Release write lock
    Exit = 6,         // Client exit
    Shutdown = 7      // Server shutdown
};

struct Request {
    Op op = Op::ReadBegin;
    int32_t id = 0;
    Employee payload{};
};
```

**Response Structure:**
```cpp
struct Response {
    bool ok = false;
    Employee payload{};
    char message[64]{};
};
```

### **Synchronization Strategy (C++20 Enhanced)**
- **Per-record RW Locks**: Each employee record has its own read-write lock using `std::shared_mutex`
- **Reader Priority**: Multiple concurrent readers allowed with improved fairness
- **Writer Exclusion**: Writers get exclusive access with C++20 lock management
- **Deadlock Prevention**: Timeouts and proper lock ordering
- **Resource Cleanup**: RAII patterns for automatic lock release

### **File Storage**
- **Binary Format**: Fixed-size records for direct access
- **In-place Updates**: Record modifications done without file recreation
- **Error Handling**: Enhanced exception safety with C++20 features
- **Data Integrity**: Atomic write operations with proper flushing

### **Named Pipe Configuration**
- **Pipe Name**: `\\.\pipe\employee_pipe`
- **Buffer Size**: 1024 bytes for both directions
- **Timeout**: 5000ms for connection and operations
- **Access Mode**: Duplex message-mode pipes
- **Client Limit**: Configurable maximum (default 16)

## 🧪 Testing

The project includes comprehensive unit tests with Google Test framework:

```bash
# Run all tests from build directory
./UnitTests.exe

# Run specific test suites
./UnitTests.exe --gtest_filter="test_employee*"
./UnitTests.exe --gtest_filter="test_file_storage*"
./UnitTests.exe --gtest_filter="test_rw_lock*"
./UnitTests.exe --gtest_filter="test_pipe*"
./UnitTests.exe --gtest_filter="test_write_commit*"
```

**Test Coverage:**
- **Employee Tests**: Record structure, name truncation, equality operations
- **FileStorage Tests**: Create, read, update operations, error handling
- **RWLock Tests**: Reader-writer synchronization, concurrent access patterns
- **Pipe Tests**: Named pipe communication, protocol handling
- **Console IO Tests**: Input/output formatting and validation
- **Write Commit Tests**: Full modification flow validation

## 📖 Usage Example

### **Step 1: Start Server Process**
```bash
./Server.exe

# Example session:
=== Server ===
Enter binary file name: employees.dat
Enter number of records: 3
Record #1
ID: 101
Name (up to 10 characters): Alice
Hours: 160.5
... (additional records)
File contents:
ID=101 name='Alice' hours=160.5
ID=102 name='Bob' hours=145.0
ID=103 name='Charlie' hours=180.0
Enter number of client processes: 3
```

### **Step 2: Start Client Processes**
```bash
# In separate terminals (3 times):
./Client.exe

# Client will connect and show:
=== Client ===
[Client] Connected. Client #1. Active clients: 1
```

### **Step 3: Client Menu Interaction**
```
=== Client Menu ===
1) Modify record
2) Read record
3) Exit
Choose option: 2
Enter ID to read: 101
[Client] Read OK
ID=101 name='Alice' hours=160.5
Press Enter to end reading process...
[Client] Read ended
```

### **Step 4: Modify Record**
```
Choose option: 1
Enter ID to modify: 102
[Client] Write lock acquired
ID=102 name='Bob' hours=145.0
New name (up to 10 chars): Robert
New hours: 155.5
Press 'y' to commit changes, any other key to cancel: y
[Client] Commit OK
[Client] Write lock released
```

### **Step 5: Server After All Clients Exit**
```
=== All clients disconnected ===
Modified file:
ID=101 name='Alice' hours=160.5
ID=102 name='Robert' hours=155.5
ID=103 name='Charlie' hours=180.0
Type 'quit' to terminate server: quit
Server is shutting down...
Press Enter to close the window...
```

## 🔒 Concurrency Scenarios

### **Reader-Writer Problem Solved**
- **Multiple Readers**: Clients 1 and 2 can simultaneously read record 101
- **Writer Exclusive**: Client 3 writing to record 102 blocks other readers/writers
- **Reader Priority**: New readers can join existing readers
- **Writer Priority**: Waiting writers get priority after current readers finish

### **Locking Protocol**
- **Client Read**: `LockRead → Read → UnlockRead`
- **Client Write**: `LockWrite → Read → Modify → Write → UnlockWrite`
- **Server**: Per-record `std::shared_mutex` with `try_lock` support

## 📊 Key Features

### **C++20 Specific Enhancements**
- **Modern CMake**: Presets and improved dependency management
- **Enhanced Type Safety**: Stronger type checking and conversions
- **Improved Template Usage**: Better template argument deduction
- **Standard Library Updates**: Latest STL features and bug fixes

### **Core Features**
- **Named Pipe Communication**: Windows named pipes for inter-process communication
- **Fine-grained Locking**: Per-record read-write locks with proper synchronization
- **Concurrent Access**: Multiple clients can work simultaneously on different records
- **Two-phase Commit**: Safe record modification with confirmation
- **Robust Error Handling**: Graceful handling of disconnections and failures
- **RAII Resource Management**: Automatic cleanup of handles and locks
- **Thread-safe Design**: Server handles multiple clients with thread-per-client model
- **Comprehensive Testing**: Unit tests for all core components

## 🎯 Design Principles

### **C++20 Best Practices**
- **Explicit Ownership**: Clear resource management with RAII
- **Type Safety**: Minimize implicit conversions, use strong types
- **Exception Safety**: Strong exception guarantees where possible
- **Move Semantics**: Efficient resource transfer
- **Concurrency Safety**: Proper synchronization for shared data

### **Architectural Principles**
- **Separation of Concerns**: Clear division between communication, storage, and synchronization
- **Resource Safety**: RAII for all Windows handles and synchronization primitives
- **Thread Safety**: Proper locking for shared data structures
- **Error Resilience**: Graceful handling of network and file system errors
- **Testability**: Modular design enabling comprehensive unit testing
- **User Experience**: Interactive console interfaces for both server and client
- **Performance**: Efficient algorithms with minimal blocking

## 🤝 Development Notes

This implementation demonstrates advanced Windows system programming with C++20 features. The project showcases:

- **Windows Named Pipes**: `CreateNamedPipeW`, `ConnectNamedPipe`, `WaitNamedPipeW`
- **Read-Write Locks**: Per-record synchronization with C++20 primitives
- **Concurrent File Access**: Multiple clients accessing same file with proper locking
- **Structured IPC**: Custom binary protocol for reliable communication
- **Client-Server Architecture**: Classic distributed system pattern
- **RAII Patterns**: Custom `Handle` class for Windows handle management
- **Modern C++**: C++20 features and best practices

**Note**: The system uses 5-second timeouts for pipe operations to prevent deadlocks. The server supports up to 16 concurrent clients by default, matching typical Windows named pipe limitations. Employee names are limited to 10 characters including null terminator for fixed-size record storage.

## 🆚 Comparison: C++17 vs C++20 Implementations

| Feature | C++17 Implementation | C++20 Implementation |
|---------|----------------------|----------------------|
| **CMake Configuration** | CMake 3.16+ required, simpler structure | CMake 3.20+ required, more modular with library targets |
| **Project Structure** | Flat structure with direct executables | Layered structure with `core` library and `serverlib` |
| **Code Organization** | Global state in `server_app.cpp` | Encapsulated in `ServerSession` and `ServerState` classes |
| **Error Handling** | Boolean returns and error codes | Enhanced exception safety with modern patterns |
| **Transport Interface** | Direct named pipe classes | Abstract `IServerTransport` and `IClientTransport` interfaces |
| **Testing Coverage** | Basic tests for core components | Extended tests including console I/O and full flow scenarios |
| **Build System** | Simple executables linking | Library-based with better dependency management |
| **Thread Management** | Raw `std::thread` with detach | More controlled thread lifecycle |
| **Client Management** | Manual index tracking | Structured `ServerState` with atomic counters |
| **Protocol Handling** | Switch-case in server loop | Separate `handle_request` method for better separation |
| **Code Reusability** | Limited reuse opportunities | Library-based design enables component reuse |
| **Modern C++ Features** | Basic C++17 features | Enhanced with C++20 practices and patterns |

### **Key Improvements in C++20 Version:**

1. **Better Architecture**:
   - Abstract transport interfaces allow for easier testing and mocking
   - Encapsulated server session management improves code organization
   - Library-based design promotes code reuse

2. **Enhanced Testing**:
   - More comprehensive test coverage with additional test scenarios
   - Mock classes for protocol testing
   - Full flow tests for write-commit operations

3. **Improved Error Handling**:
   - Better exception safety throughout the codebase
   - More informative error messages
   - Structured error propagation

4. **Modern C++ Practices**:
   - Better use of RAII patterns
   - Improved const-correctness
   - Enhanced type safety with modern patterns

5. **Build System Improvements**:
   - More granular library targets
   - Better dependency management
   - Modern CMake practices

### **Backward Compatibility Considerations**:
- The C++20 version requires a compiler with C++20 support (MSVC 2022+, GCC 11+, Clang 12+)
- Windows API usage remains identical in both versions
- The core protocol and file format are completely compatible
- Both versions solve the same lab requirements with identical functionality
- The C++20 version offers better maintainability and testability

### **Performance Considerations**:
Both versions have similar performance characteristics since:
- The same Windows named pipe APIs are used
- Identical synchronization primitives (`std::shared_mutex`)
- Same file I/O patterns and binary format
- Similar client-server interaction patterns

The C++20 version may offer slightly better compile-time safety and error checking but maintains the same runtime performance profile.

## 📄 License

Educational project - Belarusian State University, Faculty of Applied Mathematics and Computer Science, Applied Mathematics.