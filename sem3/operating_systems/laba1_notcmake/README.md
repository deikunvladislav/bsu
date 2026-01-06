# Lab Work №1: Process Creation and File Processing (Windows Implementation)

**Belarusian State University, Faculty of Applied Mathematics and Computer Science, Department of Programming Technology**  
**Topic: "Process Creation and Inter-Process Communication"**

## 🚀 Project Overview

This Windows-based project demonstrates process creation, binary file handling, and inter-process communication through three modular components managed by a main controller program. The system creates and processes employee records with salary calculations, showcasing fundamental operating system concepts including process spawning, synchronization, and file-based data exchange.

## 📁 Project Structure

```
laba1_notcmake/
├── Creator/                   # Binary file creation utility
│   └── Creator.cpp            # Creates employee records in binary format
├── Reporter/                  # Report generation utility
│   └── Reporter.cpp           # Generates formatted salary reports
├── Main/                      # Main controller program
│   └── Main.cpp               # Orchestrates Creator and Reporter processes
├── Laba1_tester/              # Unit test project (Visual Studio Test)
│   ├── Laba1_tester.cpp       # Test implementations
│   ├── pch.cpp                # Precompiled header implementation
│   └── pch.h                  # Common headers and includes
└── README.md                  # This documentation file
```

## 🏗️ Building the Project

### **Prerequisites**
- Visual Studio 2019+ (Windows development tools required)
- Windows SDK
- C++ compiler with Windows API support
- Basic understanding of Windows process management

### **Build Instructions (Visual Studio)**
Each component is a separate Visual Studio project. Build them in the following order:

1. **Build Creator:**
   - Open `Creator/Creator.sln`
   - Select appropriate configuration (Debug/Release)
   - Build Solution (Ctrl+Shift+B)

2. **Build Reporter:**
   - Open `Reporter/Reporter.sln`
   - Select appropriate configuration (Debug/Release)
   - Build Solution (Ctrl+Shift+B)

3. **Build Main:**
   - Open `Main/Main.sln`
   - Select appropriate configuration (Debug/Release)
   - Build Solution (Ctrl+Shift+B)

4. **Build Tests (Optional):**
   - Open `Laba1_tester/Laba1_tester.sln`
   - Build Solution (Ctrl+Shift+B)

**Note:** `Main.exe` expects `Creator.exe` and `Reporter.exe` to be in the same directory or accessible via PATH.

### **Alternative: Command Line Building**
```batch
:: Build individual projects
cl Creator.cpp /Fe:Creator.exe
cl Reporter.cpp /Fe:Reporter.exe
cl Main.cpp /Fe:Main.exe

:: Or use Visual Studio Developer Command Prompt
devenv Creator.sln /Build Release
devenv Reporter.sln /Build Release
devenv Main.sln /Build Release
```

## ✨ System Architecture

### **1. Creator Process (`Creator/`)**
The initial data entry component that creates binary employee records:

**Responsibilities:**
- Creates binary file for employee records with structure:
  ```cpp
  struct employee {
      int num;            // Employee ID
      char name[10];      // Employee name (max 9 chars + null)
      double hours;       // Worked hours
  };
  ```
- Prompts user for specified number of employee records
- Validates input data and ensures proper binary formatting
- Implements robust error handling for file operations

**Key Features:**
- Interactive console-based data entry
- Binary file creation with fixed-size records
- Input validation and error recovery
- Proper resource management (RAII patterns)

### **2. Reporter Process (`Reporter/`)**
The report generation component that processes binary data:

**Responsibilities:**
- Reads binary employee files created by Creator
- Calculates salaries based on hourly rate
- Generates formatted text reports with proper column alignment
- Implements currency formatting and data presentation

**Key Features:**
- Binary file parsing with error checking
- Salary calculation with proper floating-point handling
- Formatted text output with column alignment
- Support for customizable hourly rates

### **3. Main Controller (`Main/`)**
The orchestration component that manages the entire workflow:

**Responsibilities:**
- Creates and manages child processes using Windows API
- Implements process synchronization with `WaitForSingleObject`
- Handles inter-process communication via file system
- Provides user interface for entire workflow
- Manages error scenarios and graceful degradation

**Key Features:**
- Windows process creation (`CreateProcessA`)
- Process synchronization and waiting
- Resource cleanup and handle management
- Comprehensive error handling
- User-friendly console interface

## 📖 Usage Example

### **Direct Component Usage**
```bash
# Create employee records (3 employees)
Creator.exe employees.bin 3

# Generate salary report (rate: $15.50/hour)
Reporter.exe employees.bin report.txt 15.50
```

### **Integrated Main Program**
```bash
# Run the complete workflow
Main.exe

# Example session:
Enter binary file name: employees.bin
Enter number of records: 3
... (Creator process runs, prompts for employee data)
... (Binary file created and displayed)
Enter report file name: salary_report.txt
Enter rate per hour: 15.50
... (Reporter process runs, generates report)
... (Report displayed)
```

### **Component Execution Flow**
1. **Main** starts and prompts for configuration
2. **Main** launches **Creator** as child process with parameters
3. **Creator** interacts with user to collect employee data
4. **Creator** saves data to binary file and exits
5. **Main** displays binary file contents
6. **Main** launches **Reporter** as child process with parameters
7. **Reporter** reads binary file, calculates salaries, generates report
8. **Main** displays the generated report
9. **Main** cleans up resources and exits

## 🔧 Implementation Details

### **Windows Process Management**
- **Process Creation**: `CreateProcessA` with proper security attributes
- **Process Synchronization**: `WaitForSingleObject` for child process completion
- **Resource Management**: `CloseHandle` for process and thread handles
- **Error Handling**: Windows error code translation to human-readable messages

### **File I/O Operations**
- **Binary Files**: Fixed-size records for direct access and consistency
- **Text Reports**: Formatted output with proper alignment and precision
- **Error Checking**: Comprehensive file operation validation
- **Resource Safety**: RAII patterns for file stream management

### **Data Structures**
```cpp
struct employee {
    int num;            // 4 bytes
    char name[10];      // 10 bytes
    double hours;       // 8 bytes
    // Total: 22 bytes per record
};
```

### **Synchronization Strategy**
- **Parent-Child Coordination**: Main waits for each child process to complete
- **File-Based Communication**: Sequential access ensures data consistency
- **Error Propagation**: Exit codes and error messages communicated between processes
- **Resource Cleanup**: Proper handle closure to prevent resource leaks

## 🧪 Testing

### **Test Project (`Laba1_tester/`)**
The Visual Studio Test project provides comprehensive unit testing:

```cpp
// Example test cases included:
TEST_METHOD(TestBinaryWriteRead)      // Binary file I/O validation
TEST_METHOD(TestSalaryCalculation)    // Salary computation accuracy
TEST_METHOD(TestReporterOutput)       // Report generation correctness
TEST_METHOD(TestMainExecution)        // Full workflow integration
```

### **Running Tests**
1. Open `Laba1_tester/Laba1_tester.sln` in Visual Studio
2. Build the test project
3. Open Test Explorer (Test → Test Explorer)
4. Run all tests or specific test categories

### **Test Coverage**
- **Binary File Operations**: Read/write consistency, error handling
- **Salary Calculations**: Floating-point accuracy, edge cases
- **Process Management**: Child process creation, synchronization
- **Integration Testing**: End-to-end workflow validation
- **Error Scenarios**: Invalid inputs, missing files, permission issues

## 📊 Key Features

### **Windows-Specific Features**
- **Process Creation**: Demonstrates `CreateProcessA` API usage
- **Handle Management**: Proper Windows handle lifecycle management
- **Synchronization**: Inter-process coordination techniques
- **Console I/O**: Windows console interaction patterns

### **Data Processing Features**
- **Binary File Handling**: Efficient storage of structured data
- **Formatted Reporting**: Professional text output generation
- **Input Validation**: Robust error checking and user feedback
- **Modular Design**: Independent, reusable components

### **Development Features**
- **Visual Studio Integration**: Native project and solution files
- **Precompiled Headers**: Performance optimization for large projects
- **Unit Testing**: Comprehensive test suite with Visual Studio Test
- **Debugging Support**: Integrated debugging tools and error reporting

## 🎯 Design Principles

### **Process Architecture**
- **Separation of Concerns**: Each component has distinct responsibility
- **Modularity**: Independent executables enable flexible deployment
- **Reusability**: Components can be used independently or together
- **Maintainability**: Clear interfaces and documentation

### **Error Handling Strategy**
- **Defensive Programming**: Validate all inputs and operations
- **Graceful Degradation**: Handle errors without crashing
- **Informative Messages**: User-friendly error descriptions
- **Resource Safety**: Automatic cleanup in error scenarios

### **User Experience**
- **Interactive Interface**: Step-by-step guidance for users
- **Clear Feedback**: Immediate confirmation of operations
- **Data Verification**: Display intermediate results
- **Error Recovery**: Suggestions for correcting issues

## 🆚 Comparison: CMake vs Non-CMake Implementations

| Feature | Non-CMake (Visual Studio) Implementation | CMake Implementation |
|---------|------------------------------------------|----------------------|
| **Build System** | Visual Studio Solution (.sln) files | CMakeLists.txt with cross-platform support |
| **Platform Support** | Windows-only (Visual Studio dependent) | Cross-platform (Windows, Linux, macOS) |
| **Project Structure** | Separate .sln files for each component | Single CMakeLists.txt managing all targets |
| **Dependency Management** | Manual project references | CMake target dependencies and linking |
| **Compiler Compatibility** | MSVC only (Visual Studio compiler) | Multiple compilers (GCC, Clang, MSVC) |
| **Build Configuration** | Visual Studio configurations (Debug/Release) | CMake build types with presets |
| **Testing Framework** | Microsoft Unit Testing Framework | Google Test (GTest) integration |
| **Code Organization** | Per-project separation with duplication | Centralized include directories |
| **Portability** | Limited to Windows/Visual Studio | Portable across different systems |
| **Development Environment** | Visual Studio IDE integration | Any IDE supporting CMake (VS Code, CLion, etc.) |
| **Precompiled Headers** | Native Visual Studio support (`pch.h`) | CMake-based PCH configuration |
| **Error Handling** | Windows-specific error codes | Cross-platform error handling |
| **Process Creation** | Windows API (`CreateProcessA`) | Platform-agnostic process spawning |
| **File Operations** | Windows file paths and APIs | `std::filesystem` for cross-platform paths |

### **Key Differences**

1. **Build System Philosophy:**
   - **Non-CMake**: IDE-centric development with Visual Studio integration
   - **CMake**: Build-system agnostic with generator-based approach

2. **Platform Compatibility:**
   - **Non-CMake**: Tightly coupled with Windows ecosystem
   - **CMake**: Designed for cross-platform development from the start

3. **Project Management:**
   - **Non-CMake**: Multiple solution files require separate management
   - **CMake**: Single configuration file for entire project hierarchy

4. **Testing Approach:**
   - **Non-CMake**: Microsoft's proprietary testing framework
   - **CMake**: Industry-standard Google Test integration

5. **Development Workflow:**
   - **Non-CMake**: Integrated Visual Studio experience
   - **CMake**: Flexible IDE/editor choice with consistent build process

### **Advantages of Non-CMake Version:**
- **Native Windows Integration**: Better Windows API and toolchain support
- **Visual Studio Features**: IntelliSense, debugger integration, profiler
- **Simpler Setup**: No CMake learning curve for Windows developers
- **MSBuild Integration**: Seamless with other Visual Studio projects

### **Advantages of CMake Version:**
- **Cross-Platform**: Build on Windows, Linux, and macOS
- **Build System Flexibility**: Generate projects for various IDEs/tools
- **Modern CMake Practices**: Better dependency management
- **Community Standards**: Widely adopted in open-source projects

### **Code Compatibility**
Both versions share identical:
- Core algorithm implementations
- Data structures and file formats
- Business logic (salary calculations)
- User interface flow
- Error handling strategies

Differences are primarily in:
- Build system configuration
- Platform-specific API calls
- Test framework implementation
- Project organization

### **Migration Considerations**
- **Non-CMake → CMake**: Requires learning CMake syntax and patterns
- **CMake → Non-CMake**: Loss of cross-platform compatibility
- **Shared Code**: Core business logic remains transferable
- **Build Configuration**: Different approaches to dependencies and linking

## 🤝 Development Notes

This implementation demonstrates fundamental Windows system programming concepts:

- **Process Management**: Creating, managing, and synchronizing child processes
- **Inter-Process Communication**: File-based data exchange between processes
- **Windows API Usage**: `CreateProcessA`, `WaitForSingleObject`, `CloseHandle`
- **Binary File Operations**: Structured data storage and retrieval
- **Resource Management**: Proper cleanup of Windows handles and file streams

**Educational Value:**
- Illustrates parent-child process relationships
- Demonstrates process synchronization techniques
- Shows file-based IPC patterns
- Teaches Windows handle management
- Provides practical binary file I/O examples

**Note:** The system uses fixed-size records for simplicity and performance. Employee names are limited to 10 characters (including null terminator) for consistent binary formatting. All currency calculations use double-precision floating-point for accuracy.

## 📄 License

Educational project - Belarusian State University, Faculty of Applied Mathematics and Computer Science, Applied Mathematics.