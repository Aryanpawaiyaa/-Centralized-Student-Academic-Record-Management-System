// =============================================================================
//  Student.cpp  —  Implementation of the Student entity
// =============================================================================
#include "Student.h"
#include <numeric>
#include <sstream>
#include <stdexcept>
#include <iomanip>
#include <algorithm>

// ---------------------------------------------------------------------------
//  Static data
// ---------------------------------------------------------------------------
const std::vector<std::pair<std::string,std::string>> Student::SUBJECT_CATALOGUE = {
    { "CS201", "Data Structures"          },
    { "CS202", "Object-Oriented Prog."    },
    { "CS203", "Database Management"      },
    { "MA201", "Discrete Mathematics"     },
    { "EC201", "Digital Electronics"      }
};

// ---------------------------------------------------------------------------
//  Constructor
// ---------------------------------------------------------------------------
Student::Student(const std::string& id,
                 const std::string& name,
                 int totalClasses,
                 int classesAttended,
                 const std::vector<double>& marks)
    : id_(id), name_(name),
      totalClasses_(totalClasses), classesAttended_(classesAttended)
{
    if (classesAttended_ > totalClasses_)
        classesAttended_ = totalClasses_;
    initSubjects_(marks);
}

// ---------------------------------------------------------------------------
//  initSubjects_  — populates the subjects_ vector from raw marks
// ---------------------------------------------------------------------------
void Student::initSubjects_(const std::vector<double>& marks) {
    subjects_.clear();
    for (std::size_t i = 0; i < SUBJECT_CATALOGUE.size(); ++i) {
        double m = (i < marks.size()) ? marks[i] : 0.0;
        subjects_.emplace_back(SUBJECT_CATALOGUE[i].first,
                               SUBJECT_CATALOGUE[i].second,
                               m);
    }
}

// ---------------------------------------------------------------------------
//  setSubjectMark
// ---------------------------------------------------------------------------
void Student::setSubjectMark(int idx, double m) {
    if (idx < 0 || idx >= static_cast<int>(subjects_.size()))
        throw std::out_of_range("Subject index out of range");
    subjects_[idx].marks = m;
}

// ---------------------------------------------------------------------------
//  getAggregate  — arithmetic mean of all subjects
// ---------------------------------------------------------------------------
double Student::getAggregate() const {
    if (subjects_.empty()) return 0.0;
    double sum = 0.0;
    for (const auto& s : subjects_) sum += s.marks;
    return sum / static_cast<double>(subjects_.size());
}

// ---------------------------------------------------------------------------
//  getAttendancePct
// ---------------------------------------------------------------------------
double Student::getAttendancePct() const {
    if (totalClasses_ == 0) return 0.0;
    return (static_cast<double>(classesAttended_) / totalClasses_) * 100.0;
}

// ---------------------------------------------------------------------------
//  getLetterGrade
// ---------------------------------------------------------------------------
char Student::getLetterGrade() const {
    double agg = getAggregate();
    if (agg >= GRADE_A) return 'A';
    if (agg >= GRADE_B) return 'B';
    if (agg >= GRADE_C) return 'C';
    if (agg >= GRADE_D) return 'D';
    return 'F';
}

// ---------------------------------------------------------------------------
//  getGradeLabel
// ---------------------------------------------------------------------------
std::string Student::getGradeLabel() const {
    switch (getLetterGrade()) {
        case 'A': return "Excellent";
        case 'B': return "Good";
        case 'C': return "Average";
        case 'D': return "Below Average";
        default:  return "Fail";
    }
}

// ---------------------------------------------------------------------------
//  isEligible  — attendance threshold 75%
// ---------------------------------------------------------------------------
bool Student::isEligible() const {
    return getAttendancePct() >= 75.0;
}

// ---------------------------------------------------------------------------
//  renderProgressBar — visual block meter [██████░░░░]
// ---------------------------------------------------------------------------
std::string Student::renderProgressBar(double percentage, int width) {
    if (width <= 0) width = 10;
    double clamped = (percentage < 0.0) ? 0.0 : (percentage > 100.0 ? 100.0 : percentage);
    int filled = static_cast<int>((clamped / 100.0) * width);
    if (filled > width) filled = width;

    std::string bar = "[";
    for (int i = 0; i < filled; ++i) bar += "█";
    for (int i = filled; i < width; ++i) bar += "░";
    bar += "]";
    return bar;
}

// ---------------------------------------------------------------------------
//  formatMarksheet — formal printable marksheet layout
// ---------------------------------------------------------------------------
std::string Student::formatMarksheet() const {
    std::ostringstream oss;
    oss << "================================================================================\n";
    oss << "                   MADHAV INSTITUTE OF TECHNOLOGY AND SCIENCE                   \n";
    oss << "                          OFFICIAL ACADEMIC MARKSHEET                           \n";
    oss << "================================================================================\n\n";
    oss << "  Student ID       : " << id_ << "\n";
    oss << "  Student Name     : " << name_ << "\n";
    oss << "  Academic Session : Semester 3 (2024-2025)\n";
    oss << "  Department       : Computer Science & Academic Management\n";
    oss << "--------------------------------------------------------------------------------\n";
    oss << "  ATTENDANCE RECORD\n";
    oss << "  Total Classes    : " << totalClasses_ << "\n";
    oss << "  Classes Attended : " << classesAttended_ << "\n";
    oss << "  Attendance Pct   : " << std::fixed << std::setprecision(1) << getAttendancePct() << "%\n";
    oss << "  Eligibility      : " << (isEligible() ? "ELIGIBLE FOR SEMESTER EXAMINATIONS" : "DETAINED (Attendance < 75%)") << "\n";
    oss << "--------------------------------------------------------------------------------\n";
    oss << "  COURSE PERFORMANCE & MARKS EVALUATION\n\n";
    oss << "  " << std::left
        << std::setw(10) << "Course"
        << std::setw(30) << "Subject Name"
        << std::setw(12) << "Max Marks"
        << std::setw(12) << "Scored"
        << std::setw(10) << "Grade"
        << "\n";
    oss << "  ----------------------------------------------------------------------------\n";
    for (const auto& s : subjects_) {
        char g;
        if      (s.marks >= GRADE_A) g = 'A';
        else if (s.marks >= GRADE_B) g = 'B';
        else if (s.marks >= GRADE_C) g = 'C';
        else if (s.marks >= GRADE_D) g = 'D';
        else                         g = 'F';

        oss << "  " << std::left
            << std::setw(10) << s.code
            << std::setw(30) << s.name
            << std::setw(12) << "100.00"
            << std::setw(12) << std::fixed << std::setprecision(2) << s.marks
            << std::setw(10) << g
            << "\n";
    }
    oss << "  ----------------------------------------------------------------------------\n";
    oss << "  Aggregate Percentage : " << std::fixed << std::setprecision(2) << getAggregate() << "%\n";
    oss << "  Final Letter Grade   : " << getLetterGrade() << " (" << getGradeLabel() << ")\n";
    oss << "  Result Status        : " << (getLetterGrade() != 'F' ? "PASS" : "FAIL") << "\n";
    oss << "================================================================================\n\n";
    oss << "  __________________________                    __________________________\n";
    oss << "  Faculty Mentor Signature                      Controller of Examinations\n";
    oss << "================================================================================\n";
    return oss.str();
}

// ---------------------------------------------------------------------------
//  toCSV
//  Format: ID,Name,TotalClasses,Attended,M1,M2,M3,M4,M5
// ---------------------------------------------------------------------------
std::string Student::toCSV() const {
    std::ostringstream oss;
    oss << id_ << ","
        << name_ << ","
        << totalClasses_ << ","
        << classesAttended_;
    for (const auto& sub : subjects_)
        oss << "," << std::fixed << std::setprecision(2) << sub.marks;
    return oss.str();
}

// ---------------------------------------------------------------------------
//  fromCSV  — static factory
// ---------------------------------------------------------------------------
Student Student::fromCSV(const std::string& line) {
    // Skip header lines
    if (line.empty() || line[0] == '#' || line.find("StudentID") != std::string::npos)
        throw std::invalid_argument("Header or comment line");

    std::vector<std::string> tokens;
    std::stringstream ss(line);
    std::string tok;
    while (std::getline(ss, tok, ',')) {
        tokens.push_back(tok);
    }

    // Minimum expected columns: 4 + n_subjects
    const std::size_t MIN_COLS = 4 + SUBJECT_CATALOGUE.size();
    if (tokens.size() < MIN_COLS)
        throw std::invalid_argument("Insufficient columns in CSV line: " + line);

    std::string id   = tokens[0];
    std::string name = tokens[1];
    int total    = std::stoi(tokens[2]);
    int attended = std::stoi(tokens[3]);

    std::vector<double> marks;
    for (std::size_t i = 4; i < tokens.size(); ++i)
        marks.push_back(std::stod(tokens[i]));

    return Student(id, name, total, attended, marks);
}
