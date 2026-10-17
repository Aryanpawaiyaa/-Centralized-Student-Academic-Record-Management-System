// =============================================================================
//  Student.cpp  —  Implementation of the Student entity
// =============================================================================
#include "Student.h"
#include <numeric>
#include <sstream>
#include <stdexcept>

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
