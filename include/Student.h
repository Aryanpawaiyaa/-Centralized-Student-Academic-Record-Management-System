// =============================================================================
//  Student.h  —  Core entity class for the Academic Management System
//  Holds all per-student data and provides grade / percentage computation.
// =============================================================================
#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <algorithm>
// ---------------------------------------------------------------------------
//  Subject — lightweight value object for one subject's data
// ---------------------------------------------------------------------------
struct Subject {
    std::string code;   // e.g. "CS201"
    std::string name;   // e.g. "Data Structures"
    double      marks;  // raw marks out of 100

    Subject() : marks(0.0) {}
    Subject(const std::string& c, const std::string& n, double m)
        : code(c), name(n), marks(m) {}
};

// ---------------------------------------------------------------------------
//  Student — main entity class
// ---------------------------------------------------------------------------
class Student {
public:
    // ── Fixed subject catalogue (shared across all students) ─────────────
    static const std::vector<std::pair<std::string,std::string>> SUBJECT_CATALOGUE;
    // Each pair: { code, name }

    // ── Grade thresholds (institutional) ─────────────────────────────────
    static constexpr double GRADE_A = 85.0;
    static constexpr double GRADE_B = 70.0;
    static constexpr double GRADE_C = 55.0;
    static constexpr double GRADE_D = 40.0;
    // Below GRADE_D → F

    // ── Constructors ──────────────────────────────────────────────────────
    Student() : totalClasses_(0), classesAttended_(0) {}

    Student(const std::string& id,
            const std::string& name,
            int totalClasses,
            int classesAttended,
            const std::vector<double>& marks);

    // ── Accessors ─────────────────────────────────────────────────────────
    const std::string&         getId()             const { return id_; }
    const std::string&         getName()           const { return name_; }
    int                        getTotalClasses()   const { return totalClasses_; }
    int                        getClassesAttended()const { return classesAttended_; }
    const std::vector<Subject>& getSubjects()      const { return subjects_; }

    // ── Mutators ──────────────────────────────────────────────────────────
    void setName(const std::string& n)      { name_ = n; }
    void setTotalClasses(int t)             { totalClasses_ = t; }
    void setClassesAttended(int a)          { classesAttended_ = a; }
    void setSubjectMark(int idx, double m);

    // ── Computed properties ───────────────────────────────────────────────
    double      getAggregate()      const;  // mean of all subject marks
    double      getAttendancePct()  const;  // (attended / total) * 100
    char        getLetterGrade()    const;  // A B C D F
    std::string getGradeLabel()     const;  // "Excellent", "Good", …
    bool        isEligible()        const;  // attendance >= 75%

    // ── Visual / Output helpers ───────────────────────────────────────────
    static std::string renderProgressBar(double percentage, int width = 12);
    std::string formatMarksheet() const;

    // ── Serialisation ─────────────────────────────────────────────────────
    // Converts to a single CSV line (no newline appended)
    std::string toCSV() const;

    // Parses a CSV line produced by toCSV(); throws std::invalid_argument on failure
    static Student fromCSV(const std::string& line);

private:
    std::string         id_;
    std::string         name_;
    int                 totalClasses_;
    int                 classesAttended_;
    std::vector<Subject> subjects_;

    void initSubjects_(const std::vector<double>& marks);
};
