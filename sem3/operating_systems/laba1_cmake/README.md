# Lab Work №1: Process Creation (CMake Version)

**Belarusian State University, Faculty of Applied Mathematics and Computer Science, Department of Programming Technology**   
**Topic:** "Process Creation"

## 🚀 Project Overview

This is a **CMake-based implementation** of Lab Work #1, demonstrating modern C++ project organization and build system practices while solving the same core problem as the traditional Visual Studio solution. The project implements a console program Main and two utilities (Creator and Reporter) for managing employee records through Windows process creation mechanisms.

## 📁 Project Structure

```
laba1_cmake/
├── CMakeLists.txt          # Main CMake configuration file
├── CMakePresets.json       # CMake presets for build configuration
├── compilation.txt         # Compilation notes and instructions
├── common/                 # Shared library components
│   ├── employee.h          # Employee structure and operations
│   ├── utils.h             # Utility functions declarations
│   └── utils.cpp           # Windows process management implementation
├── src/                    # Main application source code
│   ├── Creator.cpp         # Binary file creation utility
│   ├── Reporter.cpp        # Report generation utility
│   └── Main.cpp           # Main orchestration program
├── tests/                  # Unit tests directory
│   └── Tests.cpp          # Google Test test cases
└── README.md              # This documentation file
```

## 🏗️ Building the Project

### Prerequisites
- **CMake 3.21+** (modern CMake features required)
- **C++17 compatible compiler** (MSVC, GCC, or Clang)
- **Windows OS** (for `CreateProcess` and `WaitForSingleObject` APIs)

### Quick Build Instructions

```bash
# Using CMake Presets (recommended)
cmake --preset=default
cmake --build --preset=default

# Traditional CMake approach
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=ON ..
cmake --build . --parallel 4
```

### Key CMake Features
- **Automatic dependency management** (Google Test via FetchContent)
- **Modern CMake practices** (target-based, proper dependencies)
- **Preset support** for reproducible builds
- **Integrated testing** with CTest
- **Shared library** (`common`) for reusable components

## ✨ Project Components

### 1. Shared Library (`common/`)
Contains the core data structures and utilities:
- **`employee` structure** with serialization methods
- **Process management utilities** (`utils::runProcess`) using Windows API
- **Report generation logic** (`employee::writeReport`)

### 2. Creator Utility
Creates binary files containing employee records.

**Usage:**
```bash
Creator <binary_file> <record_count>
```

**Example:**
```bash
Creator employees.bin 3
```
Prompts for employee data (ID, name, hours) for each record.

### 3. Reporter Utility
Generates formatted salary reports from binary files.

**Usage:**
```bash
Reporter <binary_file> <report_file> <hourly_rate>
```

**Example:**
```bash
Reporter employees.bin report.txt 15.5
```
Creates a tabular report with calculated salaries.

### 4. Main Program
Orchestrates the complete workflow:

1. Prompts for binary filename and record count
2. Launches Creator utility via `CreateProcess`
3. Waits for completion using `WaitForSingleObject`
4. Prompts for report filename and hourly rate
5. Launches Reporter utility
6. Displays the generated report

## 🧪 Testing

The project includes comprehensive unit tests using Google Test:

```bash
# Build and run tests
cmake --build build --target Tests
ctest --test-dir build --output-on-failure

# Or using presets
ctest --preset=default
```

**Test Coverage:**
- Employee data serialization (binary/text)
- I/O stream operators
- Report generation logic
- Error handling scenarios
- Edge cases (empty files, missing files)

## 🔧 Implementation Details

### Process Management
The project demonstrates Windows process creation and synchronization:
- **`CreateProcessA`** for launching child processes
- **`WaitForSingleObject`** with `INFINITE` timeout for synchronization
- **Proper handle cleanup** with `CloseHandle`

### Data Serialization
- **Binary I/O**: Direct memory writing for performance
- **Text formatting**: Tab-separated values for reports
- **Error handling**: Exception-based error propagation

### Modern C++ Features
- **C++17 standard** with type safety
- **RAII principles** for resource management
- **Modular design** with separate compilation
- **Comprehensive testing** with Google Test

## 📝 Usage Example

```bash
# Interactive workflow through Main
./Main
# Enter: employees.dat
# Enter: 2
# Enter data for 2 employees...
# Enter: report.txt
# Enter: 12.5

# Direct utility usage
./Creator employees.bin 2
./Reporter employees.bin report.txt 12.5
```

## 🤝 Development Notes

This CMake version demonstrates professional software engineering practices:
- **Modular architecture** with shared components
- **Modern build system** with CMake
- **Automated testing** integration
- **Cross-platform readiness** (except Windows-specific APIs)
- **Dependency management** via FetchContent

**Note**: While the build system is cross-platform, the process management code uses Windows-specific APIs (`CreateProcess`, `WaitForSingleObject`). For portability to other platforms, these would need to be replaced with platform-specific implementations.
