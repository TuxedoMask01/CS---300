//============================================================================
// Name        : ProjectTwo.cpp
// Author      : Rachael Thrasher
// Description : ABCU advising assistance program using a binary search tree.
//============================================================================

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cctype>

using namespace std;

// Store one course and its prerequisite course numbers.
struct Course {
    string courseNumber;
    string courseTitle;
    vector<string> prerequisites;
};

// Remove whitespace from both ends of a string.
string trim(const string& text) {
    size_t first = text.find_first_not_of(" \t\r\n");

    if (first == string::npos) {
        return "";
    }

    size_t last = text.find_last_not_of(" \t\r\n");
    return text.substr(first, last - first + 1);
}

// Normalize course numbers for consistent comparisons.
string uppercase(string text) {
    for (size_t i = 0; i < text.size(); ++i) {
        text[i] = static_cast<char>(
            toupper(static_cast<unsigned char>(text[i])));
    }

    return text;
}

// Separate comma-delimited fields while preserving empty fields.
vector<string> splitLine(const string& line) {
    vector<string> tokens;
    size_t start = 0;
    size_t comma = line.find(',', start);

    while (comma != string::npos) {
        tokens.push_back(trim(line.substr(start, comma - start)));
        start = comma + 1;
        comma = line.find(',', start);
    }

    tokens.push_back(trim(line.substr(start)));
    return tokens;
}

// Check the parsed collection for a prerequisite course.
bool existsCourse(const string& courseNumber,
                  const vector<Course>& courses) {
    for (const Course& course : courses) {
        if (course.courseNumber == courseNumber) {
            return true;
        }
    }

    return false;
}


bool readAndValidate(const string& fileName,
                     vector<Course>& parsedCourses) {
    parsedCourses.clear();
    ifstream file(fileName.c_str());

    if (!file.is_open()) {
        cout << "Error: File cannot be opened." << endl;
        return false;
    }

    string line;
    size_t lineNumber = 0;

    while (getline(file, line)) {
        ++lineNumber;

        // Remove a UTF-8 byte-order mark if the file contains one.
        if (lineNumber == 1 &&
            line.compare(0, 3, "\xEF\xBB\xBF") == 0) {
            line.erase(0, 3);
        }

        vector<string> tokens = splitLine(line);
        // Ignore unused prerequisite columns at the end of a record.
        while (tokens.size() > 2 && tokens.back().empty()) {
            tokens.pop_back();
        }

        if (tokens.size() < 2 ||
            tokens[0].empty() || tokens[1].empty()) {
            cout << "Error: Invalid course information on line "
                 << lineNumber << "." << endl;
            return false;
        }

        Course newCourse;
        newCourse.courseNumber = uppercase(tokens[0]);
        newCourse.courseTitle = tokens[1];

        for (size_t i = 2; i < tokens.size(); ++i) {
            if (tokens[i].empty()) {
                cout << "Error: Empty prerequisite on line "
                     << lineNumber << "." << endl;
                return false;
            }

            newCourse.prerequisites.push_back(uppercase(tokens[i]));
        }

        parsedCourses.push_back(newCourse);
    }

    if (file.bad()) {
        cout << "Error: Unable to finish reading the file." << endl;
        return false;
    }

    file.close();

    if (parsedCourses.empty()) {
        cout << "Error: The course file is empty." << endl;
        return false;
    }

    // Validate after reading so prerequisites may appear on later lines.
    for (const Course& course : parsedCourses) {
        for (const string& prerequisite : course.prerequisites) {
            if (!existsCourse(prerequisite, parsedCourses)) {
                cout << "Error: Prerequisite " << prerequisite
                     << " for " << course.courseNumber
                     << " does not exist." << endl;
                return false;
            }
        }
    }

    return true;
}

class BinarySearchTree {
private:
    struct Node {
        Course course;
        Node* left;
        Node* right;

        Node(const Course& aCourse)
            : course(aCourse), left(nullptr), right(nullptr) {
        }
    };

    Node* root;

    // Insert by comparing the complete course number.
    void addNode(Node* node, const Course& course) {
        if (course.courseNumber < node->course.courseNumber) {
            if (node->left == nullptr) {
                node->left = new Node(course);
            } else {
                addNode(node->left, course);
            }
        } else if (course.courseNumber > node->course.courseNumber) {
            if (node->right == nullptr) {
                node->right = new Node(course);
            } else {
                addNode(node->right, course);
            }
        } else {
            cout << "This course number is already stored: "
                 << course.courseNumber << endl;
        }
    }

    // Inorder traversal prints all courses in alphanumeric order.
    void inOrder(const Node* node) const {
        if (node == nullptr) {
            return;
        }

        inOrder(node->left);

        cout << node->course.courseNumber << ", "
             << node->course.courseTitle << endl;

        inOrder(node->right);
    }

    // Free children before freeing their parent node.
    void destroyTree(Node* node) {
        if (node == nullptr) {
            return;
        }

        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }

public:
    BinarySearchTree() : root(nullptr) {
    }

    ~BinarySearchTree() {
        clear();
    }

    // Prevent accidental copying of owned node pointers.
    BinarySearchTree(const BinarySearchTree&) = delete;
    BinarySearchTree& operator=(const BinarySearchTree&) = delete;

    void clear() {
        destroyTree(root);
        root = nullptr;
    }

    void Insert(const Course& course) {
        if (root == nullptr) {
            root = new Node(course);
        } else {
            addNode(root, course);
        }
    }

    const Course* Search(const string& courseNumber) const {
        const Node* current = root;

        while (current != nullptr) {
            if (courseNumber == current->course.courseNumber) {
                return &current->course;
            }

            if (courseNumber < current->course.courseNumber) {
                current = current->left;
            } else {
                current = current->right;
            }
        }

        return nullptr;
    }

    void InOrder() const {
        inOrder(root);
    }
};

// Validate records before inserting them into the tree.
bool loadCourses(const string& csvPath, BinarySearchTree& bst) {
    vector<Course> parsedCourses;

    if (!readAndValidate(csvPath, parsedCourses)) {
        return false;
    }

    for (const Course& course : parsedCourses) {
        bst.Insert(course);
    }

    cout << "Course data loaded successfully." << endl;
    return true;
}

void printSorted(const BinarySearchTree& bst) {
    cout << "\nHere is a sample schedule:\n" << endl;
    bst.InOrder();
}

// Look up the selected course and each prerequisite in the same tree.
void searchCourse(const BinarySearchTree& bst, string courseNumber) {
    courseNumber = uppercase(trim(courseNumber));
    const Course* course = bst.Search(courseNumber);

    if (course == nullptr) {
        cout << "Course not found." << endl;
        return;
    }

    cout << "\n" << course->courseNumber << ", "
         << course->courseTitle << endl;

    if (course->prerequisites.empty()) {
        cout << "Prerequisites: None" << endl;
        return;
    }

    cout << "Prerequisites:" << endl;

    for (const string& prerequisite : course->prerequisites) {
        const Course* prerequisiteCourse = bst.Search(prerequisite);

        if (prerequisiteCourse != nullptr) {
            cout << prerequisiteCourse->courseNumber << ", "
                 << prerequisiteCourse->courseTitle << endl;
        } else {
            cout << "Error: Prerequisite " << prerequisite
                 << " could not be found." << endl;
        }
    }
}

int main() {
    BinarySearchTree courses;
    bool fileLoaded = false;
    string choice;

    cout << "Welcome to the course planner." << endl;

    while (true) {
        cout << "\n1. Load Course File" << endl;
        cout << "2. Print Course List" << endl;
        cout << "3. Print Individual Course" << endl;
        cout << "9. Exit" << endl;
        cout << "\nWhat would you like to do? ";

        // Reading a whole line safely handles nonnumeric menu entries.
        if (!getline(cin, choice)) {
            break;
        }

        choice = trim(choice);

        if (choice == "1") {
            string csvPath;
            cout << "Enter the course data filename: ";

            if (!getline(cin, csvPath)) {
                break;
            }

            // Match the menu pseudocode: clear old data before loading.
            courses.clear();
            fileLoaded = false;
            fileLoaded = loadCourses(trim(csvPath), courses);

        } else if (choice == "2") {
            if (fileLoaded) {
                printSorted(courses);
            } else {
                cout << "Load course data first." << endl;
            }

        } else if (choice == "3") {
            if (!fileLoaded) {
                cout << "Load course data first." << endl;
                continue;
            }

            string courseNumber;
            cout << "What course do you want to know about? ";

            if (!getline(cin, courseNumber)) {
                break;
            }

            searchCourse(courses, courseNumber);

        } else if (choice == "9") {
            cout << "Thank you for using the course planner!" << endl;
            break;

        } else {
            cout << "Invalid selection. Please enter 1, 2, 3, or 9."
                 << endl;
        }
    }

    return 0;
}
