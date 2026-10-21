# AcademiCLI — Academic Management System

AcademiCLI is a C++17 command-line application for managing student academic
records. It stores student data in a CSV file and can export individual
marksheets and class reports.

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
├── build/                     <- Generated build files (ignored by Git)
├── exports/                   <- Generated reports; example exports included
├── students.csv               <- Sample CSV database
├── CMakeLists.txt             <- CMake build configuration
├── Makefile                   <- GNU Make build (g++)
├── build.bat                  <- Windows build script (MSVC or MinGW)
└── REPORT.md                  <- Project design and evaluation report
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

Run build and application commands from the repository root. The application
uses relative paths for `students.csv` and `exports/`.

### Windows

The build script detects MSVC or MinGW and offers to launch the program:

```bat
build.bat
```

Or build with CMake:

```powershell
cmake -S . -B build
cmake --build build --config Release
```

With a Visual Studio generator, run
`.\build\bin\Release\AcademiCLI.exe`; single-configuration generators place the
executable at `.\build\bin\AcademiCLI.exe`.

### GNU Make (Linux, macOS, or MinGW)

Requires `g++` and `make`.

```sh
make
./build/bin/AcademiCLI
```

Use `make run` to build and launch, or `make clean` to remove the generated
`build/` directory.

## Local and Generated Files

- `students.csv` is a checked-in sample database. Running the application can
  change it, so preserve or copy the sample before experimenting with edits.
- The application writes report files under `exports/`. Existing example
  exports are tracked; newly generated `.txt` exports are ignored by Git.
- Build directories, compiler outputs, CMake intermediates, and IDE caches are
  ignored. The ignore rules do not hide changes to already tracked sample files.

See [REPORT.md](REPORT.md) for the project design and evaluation details.

---

## Features

| # | Feature | Description |
|---|---------|-------------|
| 1 | Bootstrap Load | Reads students.csv on startup; creates if missing |
| 2 | Add Student | Validates ID uniqueness, collects 5 subject marks |
| 3 | Search by ID | O(log n) lookup from in-memory std::map |
| 4 | Update Record | Modify name, attendance, or any subject mark |
| 5 | Delete Record | Removes from memory + atomically rewrites file |
| 6 | Report Card | Per-subject marks, progress bar, aggregate, eligibility & export to file |
| 7 | Class Summary | Comprehensive class roster, class average, and grade distribution histogram |
| 8 | Top Performers | Sorted leaderboard by aggregate percentage |
| 9 | Print & Export Center | Export individual marksheets, whole class roster ledger, or batch archive to `.txt` |
| 10 | Visual Meters | ASCII/Unicode progress bars for attendance, aggregate, and grade distribution |
| 11 | Input Validation | Loops on invalid type/range; never crashes |
| 12 | Instant ANSI Display | Fast VT-100 screen refresh without shell subprocess overhead |

---

## Subjects (Fixed Catalogue)

| Code  | Subject               |
| ----- | --------------------- |
| CS201 | Data Structures       |
| CS202 | Object-Oriented Prog. |
| CS203 | Database Management   |
| MA201 | Discrete Mathematics  |
| EC201 | Digital Electronics   |
