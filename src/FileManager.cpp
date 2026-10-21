// =============================================================================
//  FileManager.cpp  —  Implementation of the back-end I/O engine
// =============================================================================
#include "FileManager.h"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <iomanip>
#include <filesystem>

// ---------------------------------------------------------------------------
//  Static members
// ---------------------------------------------------------------------------
const std::string FileManager::DEFAULT_FILE = "students.csv";

// ---------------------------------------------------------------------------
//  Constructor
// ---------------------------------------------------------------------------
FileManager::FileManager(const std::string& filePath)
    : filePath_(filePath), tmpFilePath_(filePath + ".tmp")
{}

// ---------------------------------------------------------------------------
//  loadAll
//  Opens the CSV, reads every data line (skips header), deserialises each one.
// ---------------------------------------------------------------------------
int FileManager::loadAll(std::map<std::string, Student>& dest) {
    // If file doesn't exist, create it with a header and return 0
    std::ifstream ifs(filePath_);
    if (!ifs.is_open()) {
        std::ofstream ofs(filePath_);
        if (!ofs.is_open())
            throw std::runtime_error("Cannot create storage file: " + filePath_);
        writeHeader_(ofs);
        return 0;
    }

    int count = 0;
    std::string line;
    bool firstLine = true;

    while (std::getline(ifs, line)) {
        // Trim carriage returns (Windows line endings)
        if (!line.empty() && line.back() == '\r')
            line.pop_back();

        if (line.empty()) continue;

        // Skip header
        if (firstLine) {
            firstLine = false;
            if (line.find("StudentID") != std::string::npos)
                continue;
        }

        // Skip comment lines
        if (line[0] == '#') continue;

        try {
            Student s = Student::fromCSV(line);
            dest[s.getId()] = s;
            ++count;
        } catch (const std::exception& e) {
            // Log bad lines to stderr but continue loading the rest
            std::cerr << "[WARN] Skipping malformed CSV line: " << e.what() << "\n";
        }
    }
    return count;
}

// ---------------------------------------------------------------------------
//  appendRecord  — fast path: just write one line at the end of the file
// ---------------------------------------------------------------------------
bool FileManager::appendRecord(const Student& s) {
    // If file doesn't have a header yet, write one first
    bool needHeader = !fileHasHeader_();

    std::ofstream ofs(filePath_, std::ios::app);
    if (!ofs.is_open()) return false;

    if (needHeader) writeHeader_(ofs);
    ofs << s.toCSV() << "\n";
    return ofs.good();
}

// ---------------------------------------------------------------------------
//  rewriteAll  — atomic overwrite via a temporary file using std::filesystem
// ---------------------------------------------------------------------------
bool FileManager::rewriteAll(const std::map<std::string, Student>& data) {
    namespace fs = std::filesystem;
    // Write to .tmp first
    {
        std::ofstream ofs(tmpFilePath_, std::ios::trunc);
        if (!ofs.is_open()) return false;
        writeHeader_(ofs);
        for (const auto& kv : data)
            ofs << kv.second.toCSV() << "\n";
        if (!ofs.good()) return false;
    } // ofs flushes and closes here

    std::error_code ec;
    fs::path target(filePath_);
    fs::path temp(tmpFilePath_);

    // Atomically replace original file with temp file
    fs::rename(temp, target, ec);
    if (ec) {
        // Fallback for Windows if destination is locked or across volumes
        ec.clear();
        fs::copy_file(temp, target, fs::copy_options::overwrite_existing, ec);
        fs::remove(temp, ec);
        if (ec) return false;
    }
    return true;
}

// ---------------------------------------------------------------------------
//  exportMarksheet — writes a student's marksheet to file
// ---------------------------------------------------------------------------
bool FileManager::exportMarksheet(const Student& s, const std::string& destinationFile) const {
    namespace fs = std::filesystem;
    std::error_code ec;
    fs::path p(destinationFile);
    if (p.has_parent_path()) {
        fs::create_directories(p.parent_path(), ec);
    }
    std::ofstream ofs(destinationFile, std::ios::trunc);
    if (!ofs.is_open()) return false;
    ofs << s.formatMarksheet();
    return ofs.good();
}

// ---------------------------------------------------------------------------
//  exportClassRoster — writes comprehensive tabular ledger of all students
// ---------------------------------------------------------------------------
bool FileManager::exportClassRoster(const std::map<std::string, Student>& data, const std::string& destinationFile) const {
    namespace fs = std::filesystem;
    std::error_code ec;
    fs::path p(destinationFile);
    if (p.has_parent_path()) {
        fs::create_directories(p.parent_path(), ec);
    }
    std::ofstream ofs(destinationFile, std::ios::trunc);
    if (!ofs.is_open()) return false;

    ofs << "========================================================================================================\n";
    ofs << "                               MADHAV INSTITUTE OF TECHNOLOGY AND SCIENCE                               \n";
    ofs << "                                  COMPLETE ACADEMIC CLASS ROSTER                                       \n";
    ofs << "========================================================================================================\n\n";
    ofs << "  Total Registered Students: " << data.size() << "\n\n";

    ofs << "  " << std::left
        << std::setw(16) << "Student ID"
        << std::setw(24) << "Full Name"
        << std::setw(12) << "Attend%"
        << std::setw(12) << "Aggr%"
        << std::setw(8)  << "Grade"
        << std::setw(14) << "Attendance"
        << std::setw(10) << "Result"
        << "\n";
    ofs << "  ------------------------------------------------------------------------------------------------------\n";

    double totalAgg = 0.0;
    int eligibleCount = 0;
    int passCount = 0;

    for (const auto& kv : data) {
        const auto& s = kv.second;
        totalAgg += s.getAggregate();
        if (s.isEligible()) ++eligibleCount;
        if (s.getLetterGrade() != 'F') ++passCount;

        ofs << "  " << std::left
            << std::setw(16) << s.getId()
            << std::setw(24) << s.getName()
            << std::setw(12) << (std::to_string(static_cast<int>(s.getAttendancePct())) + "%")
            << std::setw(12) << std::fixed << std::setprecision(2) << s.getAggregate()
            << std::setw(8)  << s.getLetterGrade()
            << std::setw(14) << (s.isEligible() ? "Eligible" : "Detained")
            << std::setw(10) << (s.getLetterGrade() != 'F' ? "PASS" : "FAIL")
            << "\n";
    }

    int n = static_cast<int>(data.size());
    ofs << "  ------------------------------------------------------------------------------------------------------\n\n";
    ofs << "  CLASS SUMMARY METRICS\n";
    ofs << "  Total Enrolled   : " << n << "\n";
    if (n > 0) {
        ofs << "  Class Average    : " << std::fixed << std::setprecision(2) << (totalAgg / n) << "%\n";
        ofs << "  Eligible Count   : " << eligibleCount << " (" << std::fixed << std::setprecision(1) << (100.0 * eligibleCount / n) << "%)\n";
        ofs << "  Passing Count    : " << passCount << " (" << std::fixed << std::setprecision(1) << (100.0 * passCount / n) << "%)\n";
    }
    ofs << "========================================================================================================\n";
    return ofs.good();
}

// ---------------------------------------------------------------------------
//  exportAllMarksheets — concatenates all student marksheets into one file
// ---------------------------------------------------------------------------
bool FileManager::exportAllMarksheets(const std::map<std::string, Student>& data, const std::string& destinationFile) const {
    namespace fs = std::filesystem;
    std::error_code ec;
    fs::path p(destinationFile);
    if (p.has_parent_path()) {
        fs::create_directories(p.parent_path(), ec);
    }
    std::ofstream ofs(destinationFile, std::ios::trunc);
    if (!ofs.is_open()) return false;

    for (const auto& kv : data) {
        ofs << kv.second.formatMarksheet();
        ofs << "\n\n\f\n\n"; // Form feed for print pagination
    }
    return ofs.good();
}

// ---------------------------------------------------------------------------
//  writeHeader_
// ---------------------------------------------------------------------------
void FileManager::writeHeader_(std::ofstream& ofs) {
    ofs << "StudentID,FullName,TotalClasses,ClassesAttended";
    for (const auto& sub : Student::SUBJECT_CATALOGUE)
        ofs << "," << sub.first;
    ofs << "\n";
}

// ---------------------------------------------------------------------------
//  fileHasHeader_
// ---------------------------------------------------------------------------
bool FileManager::fileHasHeader_() {
    std::ifstream ifs(filePath_);
    if (!ifs.is_open()) return false;
    std::string line;
    std::getline(ifs, line);
    return (line.find("StudentID") != std::string::npos);
}
