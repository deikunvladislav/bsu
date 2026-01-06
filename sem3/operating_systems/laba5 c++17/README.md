# Lab Work №5: Named Pipe Communication with Record Locking (C++17)

Belarusian State University, Faculty of Applied Mathematics and Computer Science, Department of Programming Technology  
**Topic**: "Data Exchange via Named Pipes"

## 🚀 Project Overview
This C++17 Windows project implements a client-server system for managing concurrent access to an employee database file using Windows named pipes. The system features a server that manages employee records with fine-grained read/write locking and multiple client processes that can concurrently read and modify records with proper synchronization.

## 📁 Project Structure
```
laba5 c++17/
├── include/                      # Header files
│   ├── console_io.hpp            # Console input/output utilities
│   ├── employee.hpp              # Employee record structure
│   ├── file_storage.hpp          # Binary file storage operations
│   ├── handle.hpp                # RAII wrapper for Windows HANDLE
│   ├── pipe.hpp                  # Named pipe server and client
│   ├── protocol.hpp              # Communication protocol definition
│   └── rw_lock.hpp               # Read-write lock implementation
├── server/                       # Server application
│   ├── server_main.cpp           # Server entry point
│   └── server_app.cpp            # Server implementation
├── client/                       # Client application
│   ├── client_main.cpp           # Client entry point
│   └── client_app.cpp            # Client implementation
├── tests/                        # Unit tests
│   ├── test_employee.cpp         # Employee record tests
│   ├── test_file_storage.cpp     # File storage operations tests
│   ├── test_protocol.cpp         # Protocol structure tests
│   └── test_rw_lock.cpp          # Read-write lock tests
├── CMakeLists.txt                # Main CMake configuration
├── CMakePresets.json             # CMake presets configuration
├── compilation.txt               # Build instructions
└── README.md                     # This documentation file
```

## 🏗️ Building the Project

### Prerequisites
- CMake 3.16+
- C++17 compatible compiler (MSVC 2017+ recommended)
- Windows OS (required for Windows named pipes)
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
cmake -DCMAKE_CXX_STANDARD=17 -DCMAKE_CXX_STANDARD_REQUIRED=ON ..
cmake --build . --config Release
```

## ✨ System Architecture

### 1. Server Process (`server/`)
The central coordinator that manages employee records and client connections:

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
- Implements per-record read-write locking:
  - Multiple readers allowed concurrently
  - Writers get exclusive access to records
  - Read requests block writers, write requests block readers
- Processes client requests via custom protocol
- Shows modified file contents after all clients disconnect

**Key Features:**
- Supports up to 16 concurrent clients (configurable)
- 5-second timeout for pipe operations
- Automatic client index assignment and management
- Graceful shutdown handling
- Concurrent client sessions with thread per client

### 2. Client Process (`client/`)
Individual processes that connect to server via named pipes:

**Responsibilities:**
- Connects to server via named pipe
- Provides interactive menu:
  - Modify record (requires write lock)
  - Read record (requires read lock)
  - Exit
- Implements two-phase locking for modifications:
  - Request write lock → get current record → modify → commit → release
- Implements shared locking for reads:
  - Request read lock → read record → display → release

**User Interaction:**
- ID-based record access
- Confirmation prompts for modifications
- Real-time feedback from server operations
- Graceful handling of connection failures

## 🔧 Implementation Details

### Communication Protocol
The system uses a custom binary protocol over named pipes:

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
    Op op;            // Operation type
    int32_t id;       // Employee ID
    Employee payload; // Data (for WriteCommit)
};
```

**Response Structure:**
```cpp
struct Response {
    bool ok;           // Success flag
    Employee payload;  // Requested data
    char message[64];  // Status message
};
```

### Synchronization Strategy
- **Per-record RW Locks**: Each employee record has its own read-write lock
- **Reader Priority**: Multiple concurrent readers allowed
- **Writer Exclusion**: Writers get exclusive access to records
- **Deadlock Prevention**: Timeouts on pipe operations
- **Resource Cleanup**: Automatic lock release on client disconnect

### File Storage
- **Binary Format**: Fixed-size records for direct access
- **In-place Updates**: Record modifications done without file recreation
- **Error Handling**: Proper exception handling for file operations
- **Data Integrity**: Atomic write operations via file streams

### Named Pipe Configuration
- **Pipe Name**: `\\.\pipe\employee_pipe`
- **Buffer Size**: 1024 bytes for both directions
- **Timeout**: 5000ms for connection and operations
- **Access Mode**: Duplex message-mode pipes
- **Client Limit**: Configurable maximum (default 16)

## 🧪 Testing

The project includes comprehensive unit tests:

```bash
# Run all tests from build directory
./Tests.exe

# Run specific test suites
./Tests.exe --gtest_filter="Employee*"
./Tests.exe --gtest_filter="FileStorage*"
./Tests.exe --gtest_filter="RWLock*"
./Tests.exe --gtest_filter="Protocol*"
```

**Test Coverage:**
- **Employee Tests**: Record structure, name clipping, serialization size
- **FileStorage Tests**: Create, read, update operations, error handling
- **RWLock Tests**: Reader-writer synchronization, concurrent access patterns
- **Protocol Tests**: Structure sizes, constant values

## 📖 Usage Example

### Step 1: Start Server Process
```bash
# Build the project first, then run server
./Server.exe

# Example session:
=== Server ===
Enter binary file name: employees.dat
Enter number of records: 3
Record #1
ID: 101
Name (up to 10 characters): Alice
Hours: 160.5
Record #2
ID: 102
Name (up to 10 characters): Bob
Hours: 145.0
Record #3
ID: 103
Name (up to 10 characters): Charlie
Hours: 180.0

File contents:
ID=101 name='Alice' hours=160.5
ID=102 name='Bob' hours=145.0
ID=103 name='Charlie' hours=180.0

Enter number of client processes: 3
```

### Step 2: Start Client Processes
```bash
# In separate terminals (3 times):
./Client.exe

# Client will connect and show:
=== Client ===
[Client] Connected. Client #1. Active clients: 1
```

### Step 3: Client Menu Interaction
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

### Step 4: Modify Record
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

### Step 5: Server After All Clients Exit
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

### Reader-Writer Problem Solved
1. **Multiple Readers**: Clients 1 and 2 can simultaneously read record 101
2. **Writer Exclusive**: Client 3 writing to record 102 blocks other readers/writers
3. **Reader Priority**: New readers can join existing readers
4. **Writer Priority**: Waiting writers get priority after current readers finish

### Locking Protocol
```
Client Read:    LockRead → Read → UnlockRead
Client Write:   LockWrite → Read → Modify → Write → UnlockWrite
Server:         Per-record std::shared_mutex with try_lock support
```

## 📊 Key Features
- **Named Pipe Communication**: Windows named pipes for inter-process communication
- **Fine-grained Locking**: Per-record read-write locks with proper synchronization
- **Concurrent Access**: Multiple clients can work simultaneously on different records
- **Two-phase Commit**: Safe record modification with confirmation
- **Robust Error Handling**: Graceful handling of disconnections and failures
- **C++17 Features**: `std::shared_mutex`, `std::filesystem`, `std::optional`
- **RAII Resource Management**: Automatic cleanup of handles and locks
- **Thread-safe Design**: Server handles multiple clients with thread-per-client model
- **Comprehensive Testing**: Unit tests for all core components

## 🎯 Design Principles
- **Separation of Concerns**: Clear division between communication, storage, and synchronization
- **Resource Safety**: RAII for all Windows handles and synchronization primitives
- **Thread Safety**: Proper locking for shared data structures
- **Error Resilience**: Graceful handling of network and file system errors
- **Testability**: Modular design enabling comprehensive unit testing
- **User Experience**: Interactive console interfaces for both server and client
- **Performance**: Efficient algorithms with minimal blocking

## 🤝 Development Notes
This implementation demonstrates Windows named pipes and advanced synchronization patterns. The project showcases:

- **Windows Named Pipes**: `CreateNamedPipeW`, `ConnectNamedPipe`, `WaitNamedPipeW`
- **Read-Write Locks**: Per-record `std::shared_mutex` with custom management
- **Concurrent File Access**: Multiple clients accessing same file with proper locking
- **Structured IPC**: Custom binary protocol for reliable communication
- **Client-Server Architecture**: Classic distributed system pattern
- **RAII Patterns**: Custom `Handle` class for Windows handle management

**Note**: The system uses 5-second timeouts for pipe operations to prevent deadlocks. The server supports up to 16 concurrent clients by default, matching typical Windows named pipe limitations. Employee names are limited to 10 characters including null terminator for fixed-size record storage.

## 📄 License
Educational project - Belarusian State University, Faculty of Applied Mathematics and Computer Science, Applied Mathematics.