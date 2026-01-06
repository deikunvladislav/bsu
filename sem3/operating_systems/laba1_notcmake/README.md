# Lab Work №1: Process Creation (Lab1_Processes)

**Belarusian State University, Faculty of Applied Mathematics and Computer Science**

This multi-project Visual Studio solution demonstrates Windows process creation and management through three utilities (`Creator`, `Reporter`, `Main`) and an integrated test suite (`Laba1_tester`).

## 🚀 Project Structure

```
laba1_notcmake/
├── Creator/                    # Binary file creation utility
│   └── Creator.cpp
├── Reporter/                   # Report generation utility
│   └── Reporter.cpp
├── Main/                       # Main control program
│   └── Main.cpp
├── Laba1_tester/               # Unit test project
│   ├── Laba1_tester.cpp
│   ├── pch.cpp                 # Precompiled header implementation
│   └── pch.h                   # Precompiled header
└── README.md
```

## 🏗️ Building the Projects

Each component is a separate Visual Studio project. Build them in any order:

1. **Build Creator**: Open `Creator/Creator.sln` → Build Solution
2. **Build Reporter**: Open `Reporter/Reporter.sln` → Build Solution  
3. **Build Main**: Open `Main/Main.sln` → Build Solution
4. **Build Tests** (Optional): Open `Laba1_tester/Laba1_tester.sln` → Build Solution

**Note**: `Main.exe` expects `Creator.exe` and `Reporter.exe` in the same directory or PATH.

## ✨ Key Features

- **Process Management**: `CreateProcess` and `WaitForSingleObject` APIs
- **Modular Design**: Independent projects for each component
- **Binary Data Handling**: Structured employee record I/O
- **Formatted Reporting**: Column-aligned reports with salary calculations
- **Comprehensive Testing**: Unit tests for all components

## 📖 Usage

### Direct Component Usage:
```bash
Creator.exe employees.bin 3
Reporter.exe employees.bin report.txt 15.50
```

### Integrated Main Program:
```bash
Main.exe  # Follow interactive prompts
```

## 🧪 Testing

The `Laba1_tester` project validates:
- Binary file I/O operations
- Salary calculation logic  
- Process creation and synchronization
- Error handling scenarios
- Component integration

**Note**: `pch.h` and `pch.cpp` are located in the `Laba1_tester/` folder as they're primarily used for testing.

---

*Lab work completed at BSU Department of Programming Technology under A.P. Pobegailo.*