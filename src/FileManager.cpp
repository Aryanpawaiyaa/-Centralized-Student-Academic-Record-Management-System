// =============================================================================
//  FileManager.cpp  —  Implementation of the back-end I/O engine
// =============================================================================
#include "FileManager.h"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <cstdio>        // std::rename, std::remove

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
//  rewriteAll  — atomic overwrite via a temporary file
// ---------------------------------------------------------------------------
bool FileManager::rewriteAll(const std::map<std::string, Student>& data) {
    // Write to .tmp first
    {
        std::ofstream ofs(tmpFilePath_);
        if (!ofs.is_open()) return false;
        writeHeader_(ofs);
        for (const auto& kv : data)
            ofs << kv.second.toCSV() << "\n";
        if (!ofs.good()) return false;
    } // ofs flushes and closes here

    // Atomically replace original file
    std::remove(filePath_.c_str());
    if (std::rename(tmpFilePath_.c_str(), filePath_.c_str()) != 0) {
        // Fallback: copy contents manually
        std::ifstream src(tmpFilePath_);
        std::ofstream dst(filePath_);
        if (!src || !dst) return false;
        dst << src.rdbuf();
        std::remove(tmpFilePath_.c_str());
    }
    return true;
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
