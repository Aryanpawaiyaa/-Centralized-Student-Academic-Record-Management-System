# AcademiCLI — Academic Management System

> **Course**: Object-Oriented Programming | MITS Semester 3  
> **Language**: C++17  
> **Paradigm**: OOP with File I/O (CSV)  
> **Build**: MSVC (VS 2022 Build Tools) / MinGW g++

---

run this command - .\build\bin\AcademiCLI.exe

## Project Structure

```
Academic CLI codebase/
│
├── include/                   <- Header files (class declarations)
│   ├── Student.h              <- Core entity: data + grade/percentage logic
│   ├── FileManager.h          <- Back-end: CSV read/write engine
│   └── MenuController.h       <- Front-end: CLI menus & input validation
│
├── src/                       <- Source files (implementations)
│   ├── Student.cpp
│   ├── FileManager.cpp
│   ├── MenuController.cpp
│   └── main.cpp               <- Entry point
│
├── build/
│   ├── bin/AcademiCLI.exe     <- Compiled executable (after build)
│   └── obj/                   <- Intermediate object files
│
├── students.csv               <- Auto-created data file (CSV database)
├── Makefile                   <- Build with g++ (Linux/MinGW)
├── build.bat                  <- One-click build script (Windows + g++)
└── README.md
```

---

## Architecture (OOP Design)

### Student Class

- Holds: id, name, totalClasses, classesAttended, subjects[]
- Computes: getAggregate(), getAttendancePct(), getLetterGrade(), isEligible()
- Serialises/deserialises itself to/from CSV via toCSV() / fromCSV()

### FileManager Class

- Owns ALL disk I/O; no other class touches the file
- loadAll() -- bootstrap reads entire CSV into an in-memory map
- appendRecord() -- fast path for adding a single new student
- rewriteAll() -- atomic overwrite via .tmp swap to prevent corruption

### MenuController Class

- Owns the event loop, screen rendering, and input validation
- inputInt*() / inputDouble*() helpers loop until valid data is entered
- The program never crashes on bad input
- Dispatches to CRUD handlers and report generators

---

## Grade Scale

| Grade | Range    | Label         |
| ----- | -------- | ------------- |
| A     | >= 85.0% | Excellent     |
| B     | >= 70.0% | Good          |
| C     | >= 55.0% | Average       |
| D     | >= 40.0% | Below Average |
| F     | < 40.0%  | Fail          |

Attendance eligibility threshold: >= 75%

---

## Data Format (CSV)

```
StudentID,FullName,TotalClasses,ClassesAttended,CS201,CS202,CS203,MA201,EC201
0901CS241050,Rohan Gupta,60,54,88.50,79.00,84.50,91.00,76.00
0901CS241051,Priya Sharma,60,42,72.00,65.50,58.00,80.00,69.00
```

- Grade and percentage are computed at runtime -- not stored
- File is created automatically on first run
- Atomic rewrite via .tmp file prevents data loss during updates/deletes

---

## Build and Run

### Option A -- VS Build Tools (Windows, recommended)

call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
cl /std:c++17 /EHsc /O2 /Iinclude src\*.cpp /Fe:build\bin\AcademiCLI.exe
.\build\bin\AcademiCLI.exe

### Option B -- MinGW g++ (Windows)

build.bat (one-click script)
-- OR --
g++ -std=c++17 -O2 -Iinclude src/\*.cpp -o AcademiCLI.exe

### Option C -- GNU Make (Linux / WSL)

make (builds to build/bin/AcademiCLI)
make run (build + launch)
make clean (remove build/ directory)

---

## Features

# Feature Description

1 Bootstrap Load Reads students.csv on startup; creates if missing
2 Add Student Validates ID uniqueness, collects 5 subject marks
3 Search by ID O(log n) lookup from in-memory std::map
4 Update Record Modify name, attendance, or any subject mark
5 Delete Record Removes from memory + atomically rewrites file
6 Report Card Full per-subject grade table + aggregate + eligibility
7 Class Summary Tabular view of all students + statistics
8 Top Performers Sorted top-5 by aggregate percentage
9 Input Validation Loops on invalid type/range; never crashes
10 ANSI Colour Coding Green/Red/Cyan feedback for pass/fail/info states

---

## Subjects (Fixed Catalogue)

| Code  | Subject               |
| ----- | --------------------- |
| CS201 | Data Structures       |
| CS202 | Object-Oriented Prog. |
| CS203 | Database Management   |
| MA201 | Discrete Mathematics  |
| EC201 | Digital Electronics   |
