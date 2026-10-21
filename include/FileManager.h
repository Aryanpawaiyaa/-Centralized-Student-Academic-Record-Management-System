// =============================================================================
//  FileManager.h  —  Back-end file I/O engine
//  All CSV read / write / atomic-rewrite operations live here.
// =============================================================================
#pragma once

#include "Student.h"
#include <vector>
#include <string>
#include <map>

class FileManager {
public:
    // Default storage file name (relative to CWD or configurable)
    static const std::string DEFAULT_FILE;

    // ── Constructor / destructor ───────────────────────────────────────────
    explicit FileManager(const std::string& filePath = DEFAULT_FILE);

    // ── Bootstrap ─────────────────────────────────────────────────────────
    // Loads all records from disk into the in-memory map.
    // Creates the file with a CSV header if it doesn't exist yet.
    // Returns number of records loaded.
    int loadAll(std::map<std::string, Student>& dest);

    // ── Write helpers ─────────────────────────────────────────────────────
    // Appends a single new student record to the CSV file.
    bool appendRecord(const Student& s);

    // Rewrites the entire file from the in-memory map
    // (used after update / delete to guarantee file consistency).
    bool rewriteAll(const std::map<std::string, Student>& data);

    // ── Export / Print helpers ─────────────────────────────────────────────
    bool exportMarksheet(const Student& s, const std::string& destinationFile) const;
    bool exportClassRoster(const std::map<std::string, Student>& data, const std::string& destinationFile) const;
    bool exportAllMarksheets(const std::map<std::string, Student>& data, const std::string& destinationFile) const;

    // ── Utility ───────────────────────────────────────────────────────────
    const std::string& getFilePath() const { return filePath_; }

private:
    std::string filePath_;
    std::string tmpFilePath_;   // temporary file used during atomic rewrite

    // Writes the CSV header row
    void writeHeader_(std::ofstream& ofs);

    // Checks whether the first line of the file is our header
    bool fileHasHeader_();
};
