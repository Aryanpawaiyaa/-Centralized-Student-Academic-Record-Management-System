// =============================================================================
//  MenuController.h  —  Front-end CLI controller
//  Owns: screen rendering, input capture & validation, action dispatch.
// =============================================================================
#pragma once

#include "Student.h"
#include "FileManager.h"
#include <map>
#include <string>

// ANSI colour helpers (no-op on platforms that don't support them)
namespace Color {
    extern const char* RESET;
    extern const char* BOLD;
    extern const char* RED;
    extern const char* GREEN;
    extern const char* YELLOW;
    extern const char* CYAN;
    extern const char* MAGENTA;
    extern const char* BLUE;
    extern const char* WHITE;
    extern const char* DIM;
}

class MenuController {
public:
    MenuController();

    // Entry point — starts the main event loop
    void run();

private:
    // ── In-memory data store ───────────────────────────────────────────────
    std::map<std::string, Student> students_;
    FileManager                    fm_;

    // ── Screen utilities ──────────────────────────────────────────────────
    void clearScreen_()  const;
    void printHeader_()  const;
    void printFooter_()  const;
    void pauseForKey_()  const;
    void printSuccess_(const std::string& msg) const;
    void printError_  (const std::string& msg) const;
    void printInfo_   (const std::string& msg) const;
    void printWarning_(const std::string& msg) const;

    // ── Menu renderers ────────────────────────────────────────────────────
    void showMainMenu_()   const;
    void showBanner_()     const;

    // ── Input validators ──────────────────────────────────────────────────
    // Reads a non-empty string from stdin
    std::string inputString_(const std::string& prompt) const;

    // Reads an integer in [lo, hi]; loops on bad input
    int inputInt_(const std::string& prompt, int lo, int hi) const;

    // Reads a double in [lo, hi]; loops on bad input
    double inputDouble_(const std::string& prompt, double lo, double hi) const;

    // Reads a student ID and validates format (non-empty, unique check optional)
    std::string inputStudentId_(bool mustBeNew) const;

    // Confirm prompt — returns true if user types y/Y
    bool confirm_(const std::string& msg) const;

    // ── CRUD action handlers ───────────────────────────────────────────────
    void handleAddStudent_();
    void handleSearchStudent_();
    void handleUpdateStudent_();
    void handleDeleteStudent_();

    // ── Report handlers ───────────────────────────────────────────────────
    void handleReportCard_();
    void handleClassSummary_();
    void handleTopPerformers_();
    void handlePrintAndExport_();

    // ── Display helpers ───────────────────────────────────────────────────
    void printStudentCard_(const Student& s) const;
    void printStudentRow_ (const Student& s) const;
    void printGradeHistogram_()              const;
    void printTableHeader_()                 const;
    void printTableFooter_()                 const;
    void printSeparator_(const char* pattern, int repeat = 78) const;
    void printSeparator_(char c = '-', int w = 80) const;
};
