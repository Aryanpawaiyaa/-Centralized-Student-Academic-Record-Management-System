import os
import matplotlib.pyplot as plt
import matplotlib.patches as patches
import numpy as np

os.makedirs('assets', exist_ok=True)

# Set clean academic aesthetic styling
plt.rcParams['font.sans-serif'] = 'DejaVu Sans'
plt.rcParams['font.family'] = 'sans-serif'
plt.rcParams['figure.autolayout'] = True

# -------------------------------------------------------------
# 1. Architecture Diagram (Three-Tier System Architecture)
# -------------------------------------------------------------
def make_architecture_diagram():
    fig, ax = plt.subplots(figsize=(10, 6), dpi=300)
    ax.set_xlim(0, 100)
    ax.set_ylim(0, 100)
    ax.axis('off')

    # Background subtle card
    bg = patches.FancyBboxPatch((2, 2), 96, 96, boxstyle="round,pad=1.5,rounding_size=2",
                                facecolor="#F8FAFC", edgecolor="#CBD5E1", linewidth=1.5)
    ax.add_patch(bg)

    # Title
    ax.text(50, 93, "AcademiCLI — Three-Tier Architectural Decomposition", 
            ha='center', va='center', fontsize=14, fontweight='bold', color='#0F172A')
    ax.text(50, 89, "Unidirectional Separation of Presentation, Domain Logic, and File Persistence", 
            ha='center', va='center', fontsize=9, color='#64748B')

    # Tier 1: Presentation Layer
    tier1 = patches.FancyBboxPatch((8, 64), 84, 20, boxstyle="round,pad=1,rounding_size=1.5",
                                  facecolor="#EFF6FF", edgecolor="#3B82F6", linewidth=1.8)
    ax.add_patch(tier1)
    ax.text(12, 80, "PRESENTATION LAYER (MenuController)", fontsize=11, fontweight='bold', color="#1D4ED8")
    ax.text(12, 75, "• ANSI Escape Rendering (VT-100 Color Coded UI, Headers, Banners)\n• Defensive Input Validation (inputInt_, inputDouble_, inputString_ with loop contracts)\n• Action Dispatcher & Interactive Menus (Add, Search, Update, Delete, Reports)",
            fontsize=8.5, color="#1E293B", linespacing=1.4)

    # Arrow 1 down
    ax.annotate('', xy=(50, 57), xytext=(50, 64),
                arrowprops=dict(facecolor='#2563EB', edgecolor='#2563EB', width=2, headwidth=8))
    ax.text(52, 60.5, "Invokes Domain Actions / Passes Validated Entities", fontsize=8, color="#2563EB", fontweight='bold')

    # Tier 2: Business Logic Layer
    tier2 = patches.FancyBboxPatch((8, 35), 84, 21, boxstyle="round,pad=1,rounding_size=1.5",
                                  facecolor="#F0FDF4", edgecolor="#10B981", linewidth=1.8)
    ax.add_patch(tier2)
    ax.text(12, 51.5, "BUSINESS LOGIC LAYER (Student & Subject)", fontsize=11, fontweight='bold', color="#047857")
    ax.text(12, 43, "• Core Academic Entities: Student encapsulation & Subject lightweight value objects\n• Grading Policies: constexpr Thresholds (A >= 85%, B >= 70%, C >= 55%, D >= 40%)\n• Attendance Analytics: getAttendancePct() & 75% Mandatory Eligibility Flag\n• Active Record Serialization: toCSV() and fromCSV() self-serialization contracts",
            fontsize=8.5, color="#1E293B", linespacing=1.35)

    # Arrow 2 down
    ax.annotate('', xy=(50, 28), xytext=(50, 35),
                arrowprops=dict(facecolor='#059669', edgecolor='#059669', width=2, headwidth=8))
    ax.text(52, 31.5, "Persists / Queries Records via In-Memory Red-Black Tree", fontsize=8, color="#059669", fontweight='bold')

    # Tier 3: Data Access Layer
    tier3 = patches.FancyBboxPatch((8, 6), 84, 21, boxstyle="round,pad=1,rounding_size=1.5",
                                  facecolor="#FFFBEB", edgecolor="#F59E0B", linewidth=1.8)
    ax.add_patch(tier3)
    ax.text(12, 23, "DATA ACCESS LAYER (FileManager)", fontsize=11, fontweight='bold', color="#B45309")
    ax.text(12, 14.5, "• Single-Writer File Persistence Engine for students.csv\n• In-Memory Map Synchronization (std::map<std::string, Student> Red-Black Tree)\n• Two-Phase Atomic Commit Protocol (write-to-.tmp -> flush -> atomic rename)\n• Resilient Error Handling (skips malformed lines with warnings, graceful recovery)",
            fontsize=8.5, color="#1E293B", linespacing=1.35)

    plt.tight_layout()
    plt.savefig('assets/architecture_diagram.png', dpi=300, bbox_inches='tight')
    plt.close()
    print("Created assets/architecture_diagram.png")

# -------------------------------------------------------------
# 2. OOP Class & Relationship Diagram
# -------------------------------------------------------------
def make_class_diagram():
    fig, ax = plt.subplots(figsize=(11, 7), dpi=300)
    ax.set_xlim(0, 110)
    ax.set_ylim(0, 100)
    ax.axis('off')

    # Outer container
    bg = patches.FancyBboxPatch((2, 2), 106, 96, boxstyle="round,pad=1.5,rounding_size=2",
                                facecolor="#F8FAFC", edgecolor="#CBD5E1", linewidth=1.5)
    ax.add_patch(bg)

    ax.text(55, 94, "AcademiCLI — Unified Object-Oriented Class & Entity Architecture", 
            ha='center', va='center', fontsize=14, fontweight='bold', color='#0F172A')

    # Class: Subject
    sub_box = patches.FancyBboxPatch((72, 54), 32, 32, boxstyle="round,pad=0.5,rounding_size=1",
                                    facecolor="#FFFFFF", edgecolor="#0284C7", linewidth=1.5)
    ax.add_patch(sub_box)
    ax.fill_between([72, 104], [84, 84], [86, 86], color="#E0F2FE")
    ax.text(88, 82.5, "struct Subject", ha='center', fontsize=10.5, fontweight='bold', color="#0369A1")
    ax.plot([72, 104], [80, 80], color="#0284C7", lw=1)
    ax.text(74, 76, "+ code : std::string\n+ name : std::string\n+ marks : double\n+ maxMarks : double", fontsize=8, color="#334155")
    ax.plot([72, 104], [64, 64], color="#0284C7", lw=1)
    ax.text(74, 59, "+ getGrade() : char\n+ isPassed() : bool", fontsize=8, color="#047857")

    # Class: Student
    stu_box = patches.FancyBboxPatch((38, 48), 28, 38, boxstyle="round,pad=0.5,rounding_size=1",
                                    facecolor="#FFFFFF", edgecolor="#10B981", linewidth=1.5)
    ax.add_patch(stu_box)
    ax.text(52, 82.5, "class Student", ha='center', fontsize=10.5, fontweight='bold', color="#047857")
    ax.plot([38, 66], [80, 80], color="#10B981", lw=1)
    ax.text(39.5, 73, "- id_ : std::string\n- name_ : std::string\n- totalClasses_ : int\n- attended_ : int\n- subjects_ : vector<Subject>", fontsize=7.5, color="#334155")
    ax.plot([38, 66], [62, 62], color="#10B981", lw=1)
    ax.text(39.5, 53, "+ getAggregate() : double\n+ getAttendancePct() : double\n+ getLetterGrade() : char\n+ isEligible() : bool\n+ toCSV() : string\n+ fromCSV() : Student [static]", fontsize=7.5, color="#047857")

    # Composition: Student 1 --- 5 Subject
    ax.annotate('', xy=(72, 70), xytext=(66, 70),
                arrowprops=dict(arrowstyle='<|-', color='#0284C7', lw=1.8))
    ax.text(67.5, 71.5, "1..5", fontsize=8, color="#0369A1", fontweight='bold')
    ax.text(67, 66.5, "owns", fontsize=7.5, color="#64748B")

    # Class: FileManager
    fm_box = patches.FancyBboxPatch((38, 8), 28, 32, boxstyle="round,pad=0.5,rounding_size=1",
                                   facecolor="#FFFFFF", edgecolor="#D97706", linewidth=1.5)
    ax.add_patch(fm_box)
    ax.text(52, 36.5, "class FileManager", ha='center', fontsize=10.5, fontweight='bold', color="#B45309")
    ax.plot([38, 66], [34, 34], color="#D97706", lw=1)
    ax.text(39.5, 29, "- filename_ : std::string\n- header_ : std::string", fontsize=8, color="#334155")
    ax.plot([38, 66], [24, 24], color="#D97706", lw=1)
    ax.text(39.5, 14, "+ loadAll() : map<string,Student>\n+ appendRecord(Student) : bool\n+ rewriteAll(map) : bool\n+ getFilename() : string", fontsize=7.5, color="#047857")

    # Dependency: FileManager uses Student
    ax.annotate('', xy=(52, 48), xytext=(52, 40),
                arrowprops=dict(arrowstyle='->', color='#B45309', lw=1.6, ls='--'))
    ax.text(53, 44, "serializes", fontsize=7.5, color="#B45309")

    # Class: MenuController
    mc_box = patches.FancyBboxPatch((6, 20), 26, 62, boxstyle="round,pad=0.5,rounding_size=1",
                                   facecolor="#FFFFFF", edgecolor="#6366F1", linewidth=1.5)
    ax.add_patch(mc_box)
    ax.text(19, 78.5, "class MenuController", ha='center', fontsize=10.5, fontweight='bold', color="#4F46E5")
    ax.plot([6, 32], [76, 76], color="#6366F1", lw=1)
    ax.text(7.5, 68, "- fileMgr_ : FileManager\n- students_ : map<str,Student>\n- running_ : bool", fontsize=7.8, color="#334155")
    ax.plot([6, 32], [63, 63], color="#6366F1", lw=1)
    ax.text(7.5, 26, "+ run() : void\n- handleAddStudent_()\n- handleSearchStudent_()\n- handleUpdateStudent_()\n- handleDeleteStudent_()\n- handleReportCard_()\n- handleClassSummary_()\n- handleTopPerformers_()\n- inputInt_() : int\n- inputDouble_() : double\n- inputString_() : string", fontsize=7.5, color="#047857")

    # Dependencies from MenuController
    ax.annotate('', xy=(38, 66), xytext=(32, 66),
                arrowprops=dict(arrowstyle='->', color='#4F46E5', lw=1.8))
    ax.text(33.5, 67.5, "uses", fontsize=7.5, color="#4F46E5")

    ax.annotate('', xy=(38, 26), xytext=(32, 26),
                arrowprops=dict(arrowstyle='->', color='#4F46E5', lw=1.8))
    ax.text(33.5, 27.5, "owns", fontsize=7.5, color="#4F46E5")

    # Legend / Key patterns box
    leg_box = patches.FancyBboxPatch((72, 8), 32, 36, boxstyle="round,pad=0.5,rounding_size=1",
                                     facecolor="#F1F5F9", edgecolor="#94A3B8", linewidth=1)
    ax.add_patch(leg_box)
    ax.text(88, 40, "Core OOP Patterns Applied", ha='center', fontsize=9.5, fontweight='bold', color="#1E293B")
    ax.text(74, 13, "• Encapsulation: Strict private fields\n  with public validated getters/setters\n• Composition: Student owns Subject items\n• Active Record: Self-deserialization\n• RAII Discipline: Zero manual allocations\n• Single Responsibility Principle:\n  UI, Logic, and File I/O strictly decoupled",
            fontsize=7.5, color="#334155", linespacing=1.35)

    plt.tight_layout()
    plt.savefig('assets/class_diagram.png', dpi=300, bbox_inches='tight')
    plt.close()
    print("Created assets/class_diagram.png")

# -------------------------------------------------------------
# 3. Atomic Write / Persistence Workflow Diagram
# -------------------------------------------------------------
def make_atomic_write_workflow():
    fig, ax = plt.subplots(figsize=(10, 5.5), dpi=300)
    ax.set_xlim(0, 100)
    ax.set_ylim(0, 100)
    ax.axis('off')

    bg = patches.FancyBboxPatch((2, 2), 96, 96, boxstyle="round,pad=1.5,rounding_size=2",
                                facecolor="#F8FAFC", edgecolor="#CBD5E1", linewidth=1.5)
    ax.add_patch(bg)

    ax.text(50, 93, "Two-Phase Commit Atomic File Persistence Protocol", 
            ha='center', va='center', fontsize=13.5, fontweight='bold', color='#0F172A')
    ax.text(50, 89, "Guarantees Zero Data Corruption During Power Outages or Program Interruption", 
            ha='center', va='center', fontsize=9, color='#64748B')

    # Step boxes
    steps = [
        ("Step 1: In-Memory Mutation", "Record updated or deleted in std::map.\nPrimary state remains valid in RAM.", "#EFF6FF", "#3B82F6", 6, 48),
        ("Step 2: Stream to Temporary File", "Open students.csv.tmp for writing.\nStream header and all map records.\nRAII flush & close stream cleanly.", "#FEF3C7", "#D97706", 38, 48),
        ("Step 3: Atomic File Swap", "1. Delete original students.csv\n2. Rename students.csv.tmp -> students.csv\n3. Fallback to stream copy if OS locks.", "#ECFDF5", "#059669", 70, 48)
    ]

    for title, desc, fc, ec, x, y in steps:
        box = patches.FancyBboxPatch((x, y), 24, 32, boxstyle="round,pad=0.8,rounding_size=1",
                                     facecolor=fc, edgecolor=ec, linewidth=1.6)
        ax.add_patch(box)
        ax.text(x + 12, y + 27, title, ha='center', va='center', fontsize=8.5, fontweight='bold', color=ec)
        ax.text(x + 1.5, y + 14, desc, fontsize=7.5, color="#1E293B", linespacing=1.3)

    # Arrows between steps
    ax.annotate('', xy=(38, 64), xytext=(30, 64),
                arrowprops=dict(facecolor='#2563EB', edgecolor='#2563EB', width=2, headwidth=7))
    ax.annotate('', xy=(70, 64), xytext=(62, 64),
                arrowprops=dict(facecolor='#059669', edgecolor='#059669', width=2, headwidth=7))

    # Resilience banner at bottom
    res_box = patches.FancyBboxPatch((6, 8), 88, 32, boxstyle="round,pad=1,rounding_size=1.2",
                                     facecolor="#F1F5F9", edgecolor="#64748B", linewidth=1.2)
    ax.add_patch(res_box)
    ax.text(50, 34, "Failure Mode Analysis & Data Integrity Guarantees", ha='center', fontsize=10, fontweight='bold', color="#0F172A")

    ax.text(9, 21, "• Failure during Step 2 (Power loss or crash): students.csv.tmp is incomplete, but original students.csv is untouched.\n  Result: ZERO data loss; last known good state is 100% preserved.",
            fontsize=8, color="#0369A1", linespacing=1.2)
    ax.text(9, 11, "• Failure during Step 3 (Rename interrupted): Either original or temporary file exists containing intact data.\n  Result: Safe recovery possible via startup check; no truncated records ever enter the database.",
            fontsize=8, color="#047857", linespacing=1.2)

    plt.tight_layout()
    plt.savefig('assets/atomic_write_workflow.png', dpi=300, bbox_inches='tight')
    plt.close()
    print("Created assets/atomic_write_workflow.png")

# -------------------------------------------------------------
# 4. Complexity & Performance Analysis Graph
# -------------------------------------------------------------
def make_complexity_graph():
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(11, 4.8), dpi=300)

    # Subplot 1: Theoretical curves
    n = np.linspace(1, 150, 300)
    o_1 = np.ones_like(n) * 2
    o_log_n = np.log2(n) * 1.5 + 2
    o_n = n * 0.15 + 2
    o_n_log_n = (n * np.log2(n)) * 0.035 + 2

    ax1.plot(n, o_1, label='O(1) [Direct Array/Hash avg]', color='#0284C7', lw=2.2, ls='--')
    ax1.plot(n, o_log_n, label='O(log n) [std::map Red-Black Tree]', color='#059669', lw=2.5)
    ax1.plot(n, o_n, label='O(n) [Linear CSV Scan / Traversals]', color='#D97706', lw=2.2)
    ax1.plot(n, o_n_log_n, label='O(n log n) [Introsort Top Performers]', color='#DC2626', lw=2.2)

    ax1.axvspan(60, 120, color='#10B981', alpha=0.15, label='Target Section Scale (60-120 students)')
    ax1.set_title("Theoretical Scaling of System Operations", fontsize=11, fontweight='bold', color='#0F172A')
    ax1.set_xlabel("Number of Student Records (n)", fontsize=9.5)
    ax1.set_ylabel("Normalized Computational Effort", fontsize=9.5)
    ax1.legend(fontsize=8, loc='upper left')
    ax1.grid(True, linestyle=':', alpha=0.6)

    # Subplot 2: Operation Bar Chart
    operations = ['Search by ID\n(In-Memory)', 'Add Student\n(Tree Insert)', 'Disk Append\n(Per Record)', 'Sort Rank\n(Top 5)', 'Full Rewrite\n(Update/Del)']
    complexities = [3.5, 3.5, 2.0, 7.5, 8.0]
    colors = ['#059669', '#10B981', '#0284C7', '#6366F1', '#D97706']

    bars = ax2.bar(operations, complexities, color=colors, width=0.55, edgecolor='#334155', linewidth=0.8)
    ax2.set_title("Operational Latency Class in AcademiCLI", fontsize=11, fontweight='bold', color='#0F172A')
    ax2.set_ylabel("Complexity Hierarchy Score", fontsize=9.5)
    ax2.set_ylim(0, 10)
    ax2.grid(axis='y', linestyle=':', alpha=0.6)

    labels = ['O(log n)', 'O(log n)', 'O(1) amort.', 'O(n log n)', 'O(n)']
    for bar, lbl in zip(bars, labels):
        yval = bar.get_height()
        ax2.text(bar.get_x() + bar.get_width()/2.0, yval + 0.3, lbl, ha='center', va='bottom', fontsize=8.5, fontweight='bold')

    plt.suptitle("Figure 4: Algorithmic Efficiency & Scalability Profile", fontsize=13, fontweight='bold', color='#0F172A', y=1.02)
    plt.tight_layout()
    plt.savefig('assets/complexity_performance_graph.png', dpi=300, bbox_inches='tight')
    plt.close()
    print("Created assets/complexity_performance_graph.png")

# -------------------------------------------------------------
# 5. Grading Policy & Attendance Model Graph
# -------------------------------------------------------------
def make_grading_graph():
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(11, 4.6), dpi=300)

    # Subplot 1: Grading distribution bands
    grades = ['F (Fail)', 'D (Below Avg)', 'C (Average)', 'B (Good)', 'A (Excellent)']
    thresholds = [40, 15, 15, 15, 15] # cumulative intervals: 0-40, 40-55, 55-70, 70-85, 85-100
    bottoms = [0, 40, 55, 70, 85]
    colors = ['#EF4444', '#F97316', '#FBBF24', '#06B6D4', '#10B981']

    for i in range(len(grades)):
        ax1.barh(grades[i], thresholds[i], left=bottoms[i], color=colors[i], edgecolor='#1E293B', height=0.55)
        ax1.text(bottoms[i] + thresholds[i]/2.0, i, f"{bottoms[i]}% - {bottoms[i]+thresholds[i]}%",
                 ha='center', va='center', color='white' if i in [0, 4] else '#0F172A', fontweight='bold', fontsize=8.5)

    ax1.set_xlim(0, 100)
    ax1.set_xlabel("Aggregate Score Percentage (%)", fontsize=9.5)
    ax1.set_title("MITS Five-Tier Academic Grading Scale", fontsize=11, fontweight='bold', color='#0F172A')
    ax1.grid(axis='x', linestyle=':', alpha=0.6)

    # Subplot 2: Attendance threshold
    attendance_cats = ['Detained / Ineligible\n(< 75% Attendance)', 'Exam Eligible\n(>= 75% Attendance)']
    att_thresholds = [75, 25]
    att_colors = ['#DC2626', '#16A34A']

    wedges, texts, autotexts = ax2.pie([75, 25], labels=attendance_cats, autopct='%1.0f%%',
                                       startangle=90, colors=att_colors, explode=(0.05, 0),
                                       textprops=dict(fontsize=8.5, fontweight='bold'))
    for at in autotexts:
        at.set_color('white')

    ax2.set_title("Institutional Attendance Policy Rule", fontsize=11, fontweight='bold', color='#0F172A')

    plt.suptitle("Figure 5: Evaluation & Statutory Business Rules in AcademiCLI", fontsize=13, fontweight='bold', color='#0F172A', y=1.02)
    plt.tight_layout()
    plt.savefig('assets/grade_distribution_graph.png', dpi=300, bbox_inches='tight')
    plt.close()
    print("Created assets/grade_distribution_graph.png")

# -------------------------------------------------------------
# 6. Total Cost of Ownership (TCO) Comparison Graph
# -------------------------------------------------------------
def make_tco_graph():
    fig, ax = plt.subplots(figsize=(9, 4.6), dpi=300)

    categories = ['Setup & Dev', 'Hosting (Yr 1)', 'Domain & Auth', 'Security/Audit', 'Total Year 1']
    local_cost = [0, 0, 0, 0, 0]
    cloud_min = [50000, 24000, 7000, 10000, 91000]
    cloud_max = [150000, 60000, 25000, 25000, 260000]

    x = np.arange(len(categories))
    width = 0.35

    ax.bar(x - width/2, [c/1000 for c in local_cost], width, label='AcademiCLI Local C++ Deployment', color='#10B981', edgecolor='#065F46')
    ax.bar(x + width/2, [c/1000 for c in cloud_min], width, label='Cloud SaaS Alternative (Min Est. ₹k)', color='#3B82F6', edgecolor='#1E40AF')
    ax.plot(x + width/2, [c/1000 for c in cloud_max], 'r--o', label='Cloud SaaS Upper Bound (₹k)', lw=1.5, markersize=5)

    ax.set_ylabel("Expenditure in ₹ Thousands (INR)", fontsize=9.5)
    ax.set_title("Figure 6: Economic Cost Comparison — Local Zero-Dependency vs. Cloud SaaS", fontsize=12, fontweight='bold', color='#0F172A')
    ax.set_xticks(x)
    ax.set_xticklabels(categories, fontsize=9)
    ax.legend(fontsize=8.5, loc='upper left')
    ax.grid(axis='y', linestyle=':', alpha=0.6)

    # Highlight total
    ax.annotate('Zero Cost Footprint\n(100% Free & Open Source)', xy=(4 - width/2, 2), xytext=(3, 80),
                arrowprops=dict(facecolor='#059669', edgecolor='#059669', width=1.5, headwidth=6),
                fontsize=8.5, fontweight='bold', color='#065F46')

    plt.tight_layout()
    plt.savefig('assets/tco_comparison_graph.png', dpi=300, bbox_inches='tight')
    plt.close()
    print("Created assets/tco_comparison_graph.png")

if __name__ == '__main__':
    make_architecture_diagram()
    make_class_diagram()
    make_atomic_write_workflow()
    make_complexity_graph()
    make_grading_graph()
    make_tco_graph()
    print("All 6 figures generated successfully!")
