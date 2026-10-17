// =============================================================================
//  MenuController.cpp  —  Full CLI front-end implementation
// =============================================================================
#include "MenuController.h"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <vector>
#include <limits>
#include <cstdlib>   // system()

// ---------------------------------------------------------------------------
//  ANSI colour constants  (Windows 10+ Virtual Terminal support)
// ---------------------------------------------------------------------------
namespace Color {
    const char* RESET   = "\033[0m";
    const char* BOLD    = "\033[1m";
    const char* RED     = "\033[1;31m";
    const char* GREEN   = "\033[1;32m";
    const char* YELLOW  = "\033[1;33m";
    const char* CYAN    = "\033[1;36m";
    const char* MAGENTA = "\033[1;35m";
    const char* BLUE    = "\033[1;34m";
    const char* WHITE   = "\033[1;37m";
    const char* DIM     = "\033[2m";
}

// ---------------------------------------------------------------------------
//  Constructor  —  enables VT-100 on Windows and loads data
// ---------------------------------------------------------------------------
MenuController::MenuController() {
#ifdef _WIN32
    // Enable ANSI escape codes on Windows 10+
    system("chcp 65001 > nul");    // UTF-8 code page
#endif
    int n = fm_.loadAll(students_);
    std::cout << Color::GREEN << "\n  [OK] Loaded " << n
              << " record(s) from \"" << fm_.getFilePath() << "\".\n"
              << Color::RESET;
}

// ---------------------------------------------------------------------------
//  run  —  main event loop
// ---------------------------------------------------------------------------
void MenuController::run() {
    showBanner_();
    pauseForKey_();

    while (true) {
        clearScreen_();
        printHeader_();
        showMainMenu_();

        int choice = inputInt_("\n  Enter choice", 0, 7);

        switch (choice) {
            case 1: handleAddStudent_();    break;
            case 2: handleSearchStudent_(); break;
            case 3: handleUpdateStudent_(); break;
            case 4: handleDeleteStudent_(); break;
            case 5: handleReportCard_();    break;
            case 6: handleClassSummary_();  break;
            case 7: handleTopPerformers_(); break;
            case 0:
                clearScreen_();
                std::cout << Color::CYAN
                          << "\n  ╔══════════════════════════════════════╗\n"
                          << "  ║   Thank you for using AcademiCLI!   ║\n"
                          << "  ║         Goodbye — MITS 2024         ║\n"
                          << "  ╚══════════════════════════════════════╝\n\n"
                          << Color::RESET;
                return;
        }
        pauseForKey_();
    }
}

// ===========================================================================
//  SCREEN UTILITIES
// ===========================================================================

void MenuController::clearScreen_() const {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void MenuController::printHeader_() const {
    std::cout << Color::CYAN;
    printSeparator_('=', 80);
    std::cout << "  " << Color::BOLD << Color::WHITE
              << "  AcademiCLI  \xE2\x80\x94  Academic Management System  |  MITS Semester 3"
              << Color::RESET << Color::CYAN << "\n";
    printSeparator_('=', 80);
    std::cout << Color::RESET;
}

void MenuController::printFooter_() const {
    std::cout << Color::DIM;
    printSeparator_('-', 80);
    std::cout << Color::RESET;
}

void MenuController::pauseForKey_() const {
    std::cout << Color::DIM << "\n  Press ENTER to continue..." << Color::RESET;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void MenuController::printSuccess_(const std::string& msg) const {
    std::cout << Color::GREEN << "\n  [SUCCESS] " << msg << Color::RESET << "\n";
}

void MenuController::printError_(const std::string& msg) const {
    std::cout << Color::RED << "\n  [ERROR]   " << msg << Color::RESET << "\n";
}

void MenuController::printInfo_(const std::string& msg) const {
    std::cout << Color::CYAN << "\n  [INFO]    " << msg << Color::RESET << "\n";
}

void MenuController::printWarning_(const std::string& msg) const {
    std::cout << Color::YELLOW << "\n  [WARN]    " << msg << Color::RESET << "\n";
}


void MenuController::printSeparator_(const char* pattern, int repeat) const {
    std::cout << "  ";
    for (int i = 0; i < repeat; ++i) std::cout << pattern;
    std::cout << "\n";
}

void MenuController::printSeparator_(char c, int w) const {
    std::cout << "  " << std::string(w - 2, c) << "\n";
}

// ===========================================================================
//  BANNER
// ===========================================================================

void MenuController::showBanner_() const {
    clearScreen_();
    std::cout << Color::CYAN << R"(
  ╔══════════════════════════════════════════════════════════════════════════════╗
  ║                                                                              ║
  ║        █████╗  ██████╗ █████╗ ██████╗ ███████╗███╗   ███╗██╗               ║
  ║       ██╔══██╗██╔════╝██╔══██╗██╔══██╗██╔════╝████╗ ████║██║               ║
  ║       ███████║██║     ███████║██║  ██║█████╗  ██╔████╔██║██║               ║
  ║       ██╔══██║██║     ██╔══██║██║  ██║██╔══╝  ██║╚██╔╝██║██║               ║
  ║       ██║  ██║╚██████╗██║  ██║██████╔╝███████╗██║ ╚═╝ ██║██║               ║
  ║       ╚═╝  ╚═╝ ╚═════╝╚═╝  ╚═╝╚═════╝ ╚══════╝╚═╝     ╚═╝╚═╝               ║
  ║                                                                              ║
  ║              C L I   —   Academic Management System  v1.0                   ║
  ║                     Manipal Institute of Technology                          ║
  ║                            Semester 3  |  2024                               ║
  ╚══════════════════════════════════════════════════════════════════════════════╝
)" << Color::RESET;
}

// ===========================================================================
//  MAIN MENU
// ===========================================================================

void MenuController::showMainMenu_() const {
    int total = static_cast<int>(students_.size());
    std::cout << "\n"
              << Color::BOLD << Color::WHITE
              << "  ┌─────────────────────────────────────┐\n"
              << "  │           M A I N   M E N U         │\n"
              << "  └─────────────────────────────────────┘\n"
              << Color::RESET;

    std::cout << Color::CYAN
              << "  Students in database: " << Color::YELLOW << total
              << Color::CYAN << "  |  File: " << Color::DIM << fm_.getFilePath()
              << Color::RESET << "\n\n";

    auto opt = [&](const char* num, const char* label, const char* col){
        std::cout << "  " << col << "[" << num << "]" << Color::WHITE
                  << "  " << label << Color::RESET << "\n";
    };

    opt("1", "Add New Student          ", Color::GREEN);
    opt("2", "Search Student by ID     ", Color::CYAN);
    opt("3", "Update Student Record    ", Color::YELLOW);
    opt("4", "Delete Student Record    ", Color::RED);
    opt("5", "Print Student Report Card", Color::MAGENTA);
    opt("6", "Class Summary Report     ", Color::BLUE);
    opt("7", "Top Performers           ", Color::CYAN);
    std::cout << "\n";
    opt("0", "Exit Application         ", Color::DIM);
}

// ===========================================================================
//  INPUT VALIDATORS
// ===========================================================================

std::string MenuController::inputString_(const std::string& prompt) const {
    std::string val;
    while (true) {
        std::cout << Color::WHITE << "  " << prompt << ": " << Color::RESET;
        std::getline(std::cin, val);
        // Trim leading/trailing whitespace
        val.erase(0, val.find_first_not_of(" \t\r\n"));
        val.erase(val.find_last_not_of(" \t\r\n") + 1);
        if (!val.empty()) return val;
        printError_("Input cannot be empty. Please try again.");
    }
}

int MenuController::inputInt_(const std::string& prompt, int lo, int hi) const {
    while (true) {
        std::cout << Color::WHITE << "  " << prompt
                  << " [" << lo << "-" << hi << "]: " << Color::RESET;
        std::string line;
        std::getline(std::cin, line);
        try {
            std::size_t pos;
            int val = std::stoi(line, &pos);
            // Ensure entire string was consumed
            if (pos == line.size() && val >= lo && val <= hi) return val;
        } catch (...) {}
        printError_("Invalid input. Please enter an integer between "
                    + std::to_string(lo) + " and " + std::to_string(hi) + ".");
    }
}

double MenuController::inputDouble_(const std::string& prompt, double lo, double hi) const {
    while (true) {
        std::cout << Color::WHITE << "  " << prompt
                  << " [" << lo << " - " << hi << "]: " << Color::RESET;
        std::string line;
        std::getline(std::cin, line);
        try {
            std::size_t pos;
            double val = std::stod(line, &pos);
            if (pos == line.size() && val >= lo && val <= hi) return val;
        } catch (...) {}
        printError_("Invalid input. Enter a number between "
                    + std::to_string(lo) + " and " + std::to_string(hi) + ".");
    }
}

std::string MenuController::inputStudentId_(bool mustBeNew) const {
    while (true) {
        std::string id = inputString_("Enter Student ID (e.g. 0901CS241050)");
        // Basic format check: non-empty, no commas (CSV delimiter)
        if (id.find(',') != std::string::npos) {
            printError_("ID must not contain commas.");
            continue;
        }
        if (mustBeNew && students_.count(id)) {
            printError_("ID \"" + id + "\" already exists. Choose a unique ID.");
            continue;
        }
        if (!mustBeNew && !students_.count(id)) {
            printError_("ID \"" + id + "\" not found in the database.");
            continue;
        }
        return id;
    }
}

bool MenuController::confirm_(const std::string& msg) const {
    std::cout << Color::YELLOW << "\n  " << msg << " (y/N): " << Color::RESET;
    std::string line;
    std::getline(std::cin, line);
    return (!line.empty() && (line[0] == 'y' || line[0] == 'Y'));
}

// ===========================================================================
//  CRUD HANDLERS
// ===========================================================================

// ── 1. Add Student ──────────────────────────────────────────────────────────
void MenuController::handleAddStudent_() {
    clearScreen_();
    printHeader_();
    std::cout << Color::GREEN << "\n  ── Add New Student ──────────────────────────────\n"
              << Color::RESET;

    std::string id   = inputStudentId_(true);
    std::string name = inputString_("Enter Full Name");

    int total    = inputInt_("Total Classes Conducted", 1, 500);
    int attended = inputInt_("Classes Attended by Student", 0, total);

    std::cout << Color::CYAN << "\n  Enter marks for each subject (0.0 – 100.0):\n"
              << Color::RESET;

    std::vector<double> marks;
    for (const auto& sub : Student::SUBJECT_CATALOGUE) {
        double m = inputDouble_("  " + sub.first + " — " + sub.second, 0.0, 100.0);
        marks.push_back(m);
    }

    Student s(id, name, total, attended, marks);

    // Preview before saving
    std::cout << "\n";
    printStudentCard_(s);

    if (!confirm_("Save this record?")) {
        printWarning_("Operation cancelled. No data was saved.");
        return;
    }

    students_[id] = s;
    if (fm_.appendRecord(s)) {
        printSuccess_("Student \"" + name + "\" [" + id + "] added successfully.");
    } else {
        printError_("Record added to memory but could not write to file.");
    }
}

// ── 2. Search Student ───────────────────────────────────────────────────────
void MenuController::handleSearchStudent_() {
    clearScreen_();
    printHeader_();
    std::cout << Color::CYAN << "\n  ── Search Student by ID ─────────────────────────\n"
              << Color::RESET;

    std::string id = inputStudentId_(false);
    printStudentCard_(students_.at(id));
}

// ── 3. Update Student ───────────────────────────────────────────────────────
void MenuController::handleUpdateStudent_() {
    clearScreen_();
    printHeader_();
    std::cout << Color::YELLOW << "\n  ── Update Student Record ────────────────────────\n"
              << Color::RESET;

    std::string id = inputStudentId_(false);
    Student& s = students_.at(id);

    std::cout << "\n  Current record:\n";
    printStudentCard_(s);

    std::cout << Color::WHITE << "\n  What would you like to update?\n" << Color::RESET
              << Color::YELLOW << "  [1]" << Color::WHITE << " Full Name\n"
              << Color::YELLOW << "  [2]" << Color::WHITE << " Attendance Data\n"
              << Color::YELLOW << "  [3]" << Color::WHITE << " Subject Mark\n"
              << Color::YELLOW << "  [0]" << Color::DIM   << " Cancel\n"
              << Color::RESET;

    int choice = inputInt_("\n  Select field to update", 0, 3);

    switch (choice) {
        case 0:
            printWarning_("Update cancelled.");
            return;

        case 1: {
            std::string newName = inputString_("Enter new Full Name");
            s.setName(newName);
            printInfo_("Name updated to \"" + newName + "\".");
            break;
        }

        case 2: {
            int total    = inputInt_("New Total Classes Conducted", 1, 500);
            int attended = inputInt_("New Classes Attended", 0, total);
            s.setTotalClasses(total);
            s.setClassesAttended(attended);
            printInfo_("Attendance updated.");
            break;
        }

        case 3: {
            std::cout << "\n  Subjects:\n";
            const auto& subs = s.getSubjects();
            for (int i = 0; i < static_cast<int>(subs.size()); ++i) {
                std::cout << Color::CYAN << "  [" << (i+1) << "] "
                          << Color::WHITE << subs[i].code << " — " << subs[i].name
                          << Color::DIM << "  (current: " << subs[i].marks << ")\n"
                          << Color::RESET;
            }
            int subIdx = inputInt_("Select subject number", 1, static_cast<int>(subs.size())) - 1;
            double newMark = inputDouble_("Enter new mark for " + subs[subIdx].code, 0.0, 100.0);
            s.setSubjectMark(subIdx, newMark);
            printInfo_("Mark updated.");
            break;
        }
    }

    if (!confirm_("Save changes to file?")) {
        // Reload from file to discard in-memory change
        students_.clear();
        fm_.loadAll(students_);
        printWarning_("Changes discarded.");
        return;
    }

    if (fm_.rewriteAll(students_)) {
        printSuccess_("Record [" + id + "] updated and saved successfully.");
    } else {
        printError_("In-memory update done, but file write failed.");
    }
}

// ── 4. Delete Student ───────────────────────────────────────────────────────
void MenuController::handleDeleteStudent_() {
    clearScreen_();
    printHeader_();
    std::cout << Color::RED << "\n  ── Delete Student Record ────────────────────────\n"
              << Color::RESET;

    std::string id = inputStudentId_(false);
    printStudentCard_(students_.at(id));

    if (!confirm_("Are you SURE you want to permanently delete this record?")) {
        printWarning_("Deletion cancelled.");
        return;
    }

    std::string name = students_[id].getName();
    students_.erase(id);

    if (fm_.rewriteAll(students_)) {
        printSuccess_("Record [" + id + "] \"" + name + "\" permanently deleted.");
    } else {
        printError_("Record removed from memory but file rewrite failed.");
    }
}

// ===========================================================================
//  REPORT HANDLERS
// ===========================================================================

// ── 5. Individual Report Card ───────────────────────────────────────────────
void MenuController::handleReportCard_() {
    clearScreen_();
    printHeader_();
    std::cout << Color::MAGENTA << "\n  ── Student Report Card ──────────────────────────\n"
              << Color::RESET;

    std::string id = inputStudentId_(false);
    const Student& s = students_.at(id);

    printSeparator_('=', 80);
    std::cout << Color::BOLD << Color::WHITE
              << "\n              ACADEMIC REPORT CARD\n"
              << "         Manipal Institute of Technology\n\n"
              << Color::RESET;

    std::cout << std::left
              << Color::CYAN   << "  Student ID  : " << Color::WHITE << s.getId()   << "\n"
              << Color::CYAN   << "  Name        : " << Color::WHITE << s.getName() << "\n\n";

    // Attendance
    double attPct = s.getAttendancePct();
    std::cout << Color::CYAN << "  Attendance  : "
              << (attPct >= 75.0 ? Color::GREEN : Color::RED)
              << std::fixed << std::setprecision(1) << attPct << "% "
              << "(" << s.getClassesAttended() << "/" << s.getTotalClasses() << " classes)";
    if (attPct < 75.0)
        std::cout << Color::RED << "  ← NOT ELIGIBLE";
    std::cout << Color::RESET << "\n\n";

    // Marks table
    std::cout << Color::CYAN;
    printSeparator_('-', 60);
    std::cout << "  " << std::left
              << std::setw(8)  << "Code"
              << std::setw(28) << "Subject"
              << std::setw(10) << "Marks"
              << std::setw(10) << "Grade"
              << "\n";
    printSeparator_('-', 60);
    std::cout << Color::RESET;

    for (const auto& sub : s.getSubjects()) {
        char g;
        if      (sub.marks >= Student::GRADE_A) g = 'A';
        else if (sub.marks >= Student::GRADE_B) g = 'B';
        else if (sub.marks >= Student::GRADE_C) g = 'C';
        else if (sub.marks >= Student::GRADE_D) g = 'D';
        else                                     g = 'F';

        const char* col = (g == 'A') ? Color::GREEN  :
                          (g == 'B') ? Color::CYAN   :
                          (g == 'C') ? Color::YELLOW :
                          (g == 'D') ? Color::MAGENTA: Color::RED;

        std::cout << "  " << std::left
                  << Color::WHITE << std::setw(8)  << sub.code
                  << Color::WHITE << std::setw(28) << sub.name
                  << col          << std::setw(10) << std::fixed << std::setprecision(2) << sub.marks
                  << col          << std::setw(10) << g
                  << Color::RESET << "\n";
    }

    std::cout << Color::CYAN;
    printSeparator_('-', 60);

    double agg = s.getAggregate();
    char   lg  = s.getLetterGrade();
    const char* gradeCol = (lg == 'A') ? Color::GREEN  :
                           (lg == 'B') ? Color::CYAN   :
                           (lg == 'C') ? Color::YELLOW :
                           (lg == 'D') ? Color::MAGENTA: Color::RED;

    std::cout << "\n  " << Color::WHITE << std::left << std::setw(36) << "Aggregate Percentage"
              << gradeCol << std::setw(10) << std::fixed << std::setprecision(2) << agg << "%"
              << Color::RESET << "\n"
              << "  " << Color::WHITE << std::setw(36) << "Final Grade"
              << gradeCol << lg << "  (" << s.getGradeLabel() << ")\n"
              << Color::RESET;

    printSeparator_('=', 80);
}

// ── 6. Class Summary ────────────────────────────────────────────────────────
void MenuController::handleClassSummary_() {
    clearScreen_();
    printHeader_();
    std::cout << Color::BLUE << "\n  ── Class Summary Report ─────────────────────────\n"
              << Color::RESET;

    if (students_.empty()) {
        printWarning_("No student records found in the database.");
        return;
    }

    printTableHeader_();
    for (const auto& kv : students_)
        printStudentRow_(kv.second);
    printTableFooter_();

    // Statistics
    double totalAgg = 0.0;
    int    eligible = 0, passCount = 0;
    for (const auto& kv : students_) {
        totalAgg += kv.second.getAggregate();
        if (kv.second.isEligible()) ++eligible;
        if (kv.second.getLetterGrade() != 'F') ++passCount;
    }
    int n = static_cast<int>(students_.size());

    std::cout << "\n  " << Color::BOLD << Color::WHITE << "── Statistics ─────────────────────────────────────────\n"
              << Color::RESET;
    std::cout << Color::CYAN
              << "  Total Students    : " << Color::WHITE << n << "\n"
              << Color::CYAN
              << "  Class Average     : " << Color::WHITE
              << std::fixed << std::setprecision(2) << (totalAgg / n) << "%\n"
              << Color::CYAN
              << "  Eligible Students : " << Color::GREEN << eligible
              << Color::DIM << " (" << std::setprecision(1) << (100.0*eligible/n) << "% attendance ≥ 75%)\n"
              << Color::CYAN
              << "  Passed Students   : " << Color::GREEN << passCount
              << Color::DIM << " (" << std::setprecision(1) << (100.0*passCount/n) << "% pass rate)\n"
              << Color::RESET;
}

// ── 7. Top Performers ───────────────────────────────────────────────────────
void MenuController::handleTopPerformers_() {
    clearScreen_();
    printHeader_();
    std::cout << Color::CYAN << "\n  ── Top Performers ───────────────────────────────\n"
              << Color::RESET;

    if (students_.empty()) {
        printWarning_("No student records found.");
        return;
    }

    // Sort descending by aggregate
    std::vector<const Student*> sorted;
    for (const auto& kv : students_)
        sorted.push_back(&kv.second);
    std::sort(sorted.begin(), sorted.end(), [](const Student* a, const Student* b){
        return a->getAggregate() > b->getAggregate();
    });

    int show = std::min(static_cast<int>(sorted.size()), 5);
    std::cout << Color::YELLOW << "  Top " << show << " students by aggregate percentage:\n\n"
              << Color::RESET;

    for (int i = 0; i < show; ++i) {
        const Student* s = sorted[i];
        char lg = s->getLetterGrade();
        const char* col = (lg == 'A') ? Color::GREEN  :
                          (lg == 'B') ? Color::CYAN   :
                          (lg == 'C') ? Color::YELLOW : Color::WHITE;

        std::cout << Color::BOLD << "  #" << (i+1) << " " << Color::RESET
                  << col << std::left << std::setw(20) << s->getName()
                  << Color::WHITE
                  << "  ID: " << std::setw(15) << s->getId()
                  << "  Agg: " << std::fixed << std::setprecision(2) << s->getAggregate() << "%"
                  << "  Grade: " << col << lg
                  << Color::RESET << "\n";
    }
}

// ===========================================================================
//  DISPLAY HELPERS
// ===========================================================================

void MenuController::printStudentCard_(const Student& s) const {
    printSeparator_('-', 70);
    std::cout << Color::CYAN << "  ID    : " << Color::WHITE << s.getId()   << "\n"
              << Color::CYAN << "  Name  : " << Color::WHITE << s.getName() << "\n";

    double att = s.getAttendancePct();
    std::cout << Color::CYAN << "  Attend: "
              << (att >= 75.0 ? Color::GREEN : Color::RED)
              << std::fixed << std::setprecision(1) << att << "%  "
              << "(" << s.getClassesAttended() << "/" << s.getTotalClasses() << ")"
              << Color::RESET << "\n";

    std::cout << Color::CYAN << "  Marks : " << Color::RESET;
    for (const auto& sub : s.getSubjects())
        std::cout << Color::WHITE << sub.code << "="
                  << std::fixed << std::setprecision(1) << sub.marks << "  ";
    std::cout << "\n";

    double agg = s.getAggregate();
    char   lg  = s.getLetterGrade();
    const char* gc = (lg == 'A') ? Color::GREEN  :
                     (lg == 'B') ? Color::CYAN   :
                     (lg == 'C') ? Color::YELLOW :
                     (lg == 'D') ? Color::MAGENTA: Color::RED;

    std::cout << Color::CYAN << "  Aggr  : "
              << gc << std::fixed << std::setprecision(2) << agg << "%"
              << "  Grade: " << lg << " (" << s.getGradeLabel() << ")"
              << Color::RESET << "\n";
    printSeparator_('-', 70);
}

void MenuController::printTableHeader_() const {
    std::cout << "\n  " << Color::CYAN
              << std::left
              << std::setw(15) << "Student ID"
              << std::setw(22) << "Name"
              << std::setw(10) << "Attend%"
              << std::setw(10) << "Aggr%"
              << std::setw(8)  << "Grade"
              << std::setw(10) << "Status"
              << Color::RESET  << "\n";
    printSeparator_('-', 80);
}

void MenuController::printStudentRow_(const Student& s) const {
    double agg = s.getAggregate();
    char   lg  = s.getLetterGrade();
    const char* col = (lg == 'A') ? Color::GREEN  :
                      (lg == 'B') ? Color::CYAN   :
                      (lg == 'C') ? Color::YELLOW :
                      (lg == 'D') ? Color::MAGENTA: Color::RED;

    std::cout << "  " << std::left
              << Color::WHITE   << std::setw(15) << s.getId()
              << Color::WHITE   << std::setw(22) << s.getName().substr(0, 21)
              << (s.isEligible() ? Color::GREEN : Color::RED)
              << std::setw(10) << (std::to_string(static_cast<int>(s.getAttendancePct())) + "%")
              << col << std::fixed << std::setprecision(1)
              << std::setw(10) << agg
              << col << std::setw(8) << lg
              << (s.isEligible() ? Color::GREEN : Color::RED)
              << std::setw(10) << (s.isEligible() ? "Eligible" : "Detained")
              << Color::RESET  << "\n";
}

void MenuController::printTableFooter_() const {
    printSeparator_('-', 80);
}
