# Lab Work №4: Process Synchronization via File-Based Ring Queue (C++11 Windows Threads)

Belarusian State University, Faculty of Applied Mathematics and Computer Science, Department of Programming Technology  
**Topic**: "Process Synchronization"

## 🚀 Project Overview
This C++11 Windows project implements a multi-process message passing system where multiple Sender processes communicate with a single Receiver process through a shared binary file organized as a ring queue (FIFO). The system uses Windows synchronization primitives (events and mutexes) for inter-process coordination and demonstrates file-based communication with concurrent access control.

## 📁 Project Structure
```
laba4 c++11/
├── CMakeLists.txt                   # Main CMake configuration file
├── CMakePresets.json                # CMake presets for build configuration
├── compilation.txt                  # Compilation instructions
├── include/                         # Header files
│   ├── core/
│   │   ├── byte_io.h                # Low-level binary file I/O operations
│   │   └── ring_queue_file.h        # Ring queue file implementation
│   ├── ipc/
│   │   ├── process_launcher.h       # Process creation and management
│   │   ├── win_handle.h             # RAII wrapper for Windows HANDLE
│   │   └── win_sync.h               # Windows synchronization primitives
│   └── util/
│       ├── console.h                # Console input/output utilities
│       └── string_trim.h            # String trimming utilities
├── src/                             # Source code implementation
│   ├── core/
│   │   ├── byte_io.cpp              # ByteIO implementation
│   │   └── ring_queue_file.cpp      # RingQueueFile implementation
│   ├── ipc/
│   │   ├── process_launcher.cpp     # Process launcher implementation
│   │   └── win_sync.cpp             # Synchronization objects implementation
│   ├── receiver.cpp                 # Receiver process main implementation
│   └── sender.cpp                   # Sender process main implementation
├── tests/                           # Unit tests directory
│   ├── test_byte_io.cpp             # ByteIO unit tests
│   ├── test_process_launcher.cpp    # ProcessLauncher unit tests
│   ├── test_ring_queue_file.cpp     # RingQueueFile unit tests
│   ├── test_sync.cpp                # SyncObjects unit tests
│   └── test_util.cpp                # Utility functions unit tests
└── README.md                        # This documentation file
```

## 🏗️ Building the Project

### Prerequisites
- CMake 3.16+
- C++11 compatible compiler (MSVC recommended for Windows)
- Windows OS (required for Windows synchronization API)

### Build Instructions
```bash
# Configure the project using CMake presets
cmake --preset default

# Build the project
cmake --build --preset default

# Run unit tests
ctest --preset default
```

### Manual Build (Alternative)
```bash
mkdir build && cd build
cmake -DCMAKE_CXX_STANDARD=11 -DCMAKE_CXX_STANDARD_REQUIRED=ON ..
cmake --build . --config Release
```

## ✨ Process Architecture

### 1. Receiver Process (`receiver.cpp`)
The main coordinating process that manages the message queue:

**Responsibilities:**
- Creates binary file for message storage with specified capacity
- Initializes ring queue structure in file
- Launches multiple Sender processes using `CreateProcessA`
- Creates Windows synchronization objects (events and mutex)
- Waits for all Senders to signal readiness using `WaitForMultipleObjects`
- Processes console commands: `read` (read message) and `quit` (exit)

**Synchronization:**
- Creates named events for each Sender to signal readiness
- Uses file mutex for exclusive queue access
- Waits on message event when queue is empty
- Signals termination of all Sender processes on exit

### 2. Sender Process (`sender.cpp`)
Individual processes that send messages to Receiver:

**Responsibilities:**
- Opens shared message file using provided file path
- Signals readiness to Receiver via named event
- Processes console commands: `send` (send message) and `stop` (exit)
- Validates message length (max 19 characters)
- Handles queue full conditions

**Synchronization:**
- Opens synchronization objects created by Receiver
- Acquires file mutex before accessing queue
- Signals message event after successful message delivery

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
- **RingQueueFile Tests**: Queue operations (push/pop), full/empty conditions, message length validation
- **SyncObject Tests**: Windows synchronization primitives creation and signaling
- **Utility Tests**: String trimming and console operations

## 📖 Usage Example

### Starting the System:
```bash
# Build the project first, then run receiver from build directory
./receiver.exe

# Example session:
Receiver: enter binary file name:
messages.dat
Receiver: enter number of records:
10
Receiver: enter number of sender processes:
3
Queue file created.
Senders launched.
Waiting for all senders to be ready...
All senders are ready.
Receiver command: [read | quit]
```

### Sender Console (automatically launched):
```
Sender ready.
Sender command: [send | stop]
send
Enter message (< 20 chars):
Hello from Sender 1
Message sent.
```

### Reading Messages in Receiver:
```
Receiver command: [read | quit]
read
Received: Hello from Sender 1
```

## 🔧 Implementation Details

### Ring Queue File Format
- **File Header**: 16 bytes containing capacity, head, tail, and count
- **Message Records**: Fixed-size slots of 20 bytes each (max 19 chars + null terminator)
- **Memory Layout**: Header followed by capacity × 20 bytes of message slots

### Windows Synchronization Objects
- **Named Events**: Used for Sender readiness signaling and message notifications
- **Named Mutex**: Ensures exclusive file access for queue operations
- **RAII Wrapper**: `WinHandle` class provides automatic handle management

### Process Communication
- **Command Line Arguments**: Receiver passes file path and synchronization prefix to Senders
- **Shared File**: Binary file serves as shared memory between processes
- **Named Objects**: Synchronization objects accessible by all processes via names

### Error Handling
- **Input Validation**: Message length limits, queue capacity validation
- **File Operations**: Error checking for all file I/O operations
- **Process Management**: Proper cleanup of child processes on exit
- **Synchronization**: Timeout handling for mutex acquisition

## 📊 Key Features
- **Multi-Process Architecture**: One Receiver with multiple concurrent Senders
- **File-Based Communication**: Persistent message storage in binary file
- **Ring Queue Implementation**: FIFO message ordering with fixed capacity
- **Windows Synchronization**: Named events and mutexes for process coordination
- **RAII Resource Management**: Automatic cleanup of Windows handles and file streams
- **Console Interface**: Interactive command-line control for both processes
- **Comprehensive Testing**: Unit tests for all core components

## 🎯 Design Principles
- **Separation of Concerns**: Clear division between file I/O, queue logic, and synchronization
- **Resource Safety**: RAII patterns for all system resources (handles, files)
- **Thread-Safe Operations**: Proper synchronization for concurrent file access
- **Error Resilience**: Graceful handling of invalid inputs and system errors
- **Testability**: Modular design enabling comprehensive unit testing
- **C++11 Compliance**: Leverages modern C++ features while maintaining compatibility

## 🤝 Development Notes
This implementation demonstrates Windows-specific process synchronization techniques using named synchronization objects. The project showcases:

- **Windows API Integration**: `CreateProcessA`, `CreateEventA`, `CreateMutexA`, `WaitForMultipleObjects`
- **Inter-Process Communication**: Shared file as communication medium with proper synchronization
- **Process Management**: Launching and terminating child processes
- **File-Based Data Structures**: Implementing ring queue in binary file with fixed-size records
- **RAII Patterns**: Custom `WinHandle` class for Windows handle management

**Note**: The system uses 15000ms timeouts for synchronization operations to prevent deadlocks. The message length limit of 19 characters (20 bytes with null terminator) ensures fixed-size records for efficient queue implementation.

## 📄 License
Educational project - Belarusian State University, Faculty of Applied Mathematics and Computer Science, Applied Mathematics.