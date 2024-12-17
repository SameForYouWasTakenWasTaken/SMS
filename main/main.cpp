#include <iostream>
#include <string>
#include <map>
#include <algorithm>
#include <chrono>
#include <thread>

#include "../include/helper.h"

// In case anyone asks, I got ChatGPT to do some explaining for my code cause im too lazy.

using namespace ezc;

// Structure to hold student data
struct student_data {
	std::string name; // Student's name
	long id; // Unique student ID
	std::string date_of_birth; // Date of birth in string format
	std::string status; // Enrollment status (e.g., active, graduated)
	std::map<std::string, int> grades; // Map for subject-grade pairs
	float average_grade = 0; // Calculated average grade
};

// Converts a string to lowercase
std::string tolower(const std::string& input) {
	std::string result = input;
	for (char& c : result) {
		c = (c >= 'A' && c <= 'Z') ? c + ('a' - 'A') : c; // Convert uppercase letters to lowercase
	}
	return result;
}


class student {
private:
	int count_ = 0; // Number of students
public:
	std::map<long, student_data> students_; // Map for student data, keyed by ID

	// Adds a new student to the system
	void add_student(const std::string& name, const long& id, const std::string& date_of_birth,
		const std::string& status, const std::map<std::string, int>& grades) {
		student_data data;
		data.name = name;
		data.date_of_birth = date_of_birth;
		data.status = status;
		data.grades = grades;
		data.id = id;

		// Calculate average grade
		int total_grade = 0;
		for (const auto& grade : data.grades) {
			total_grade += grade.second; // Sum up all grades
		}
		data.average_grade = (data.grades.empty()) ? 0 : static_cast<float>(total_grade) / data.grades.size();

		students_[id] = data; // Add data to map
		count_++; // Increment student count
	}

	// Removes a student from the system
	void remove_student(const long& student_id) {
		if (students_.erase(student_id) > 0) {
			count_--; // Decrement student count if removal was successful
		}
		else {
			std::cout << "Student with ID " << student_id << " not found.\n";
		}
	}

	// Edits an existing student's details
	void edit_student(const long& student_id) {
		auto it = students_.find(student_id);
		if (it == students_.end()) {
			std::cout << "Student with ID " << student_id << " not found.\n";
			return;
		}

		student_data& item = it->second;
		// Display editable fields
		logger.BlueLog("1. Name\n");
		logger.BlueLog("2. Date of Birth\n");
		logger.BlueLog("3. Status\n");
		logger.BlueLog("4. Grades\n");

		int choice;
		betterCin(&choice, "\nInvalid choice! Please select a valid option.\n", true, [&]() {
			return choice >= 1 && choice <= 4; // Ensure choice is valid
			});

		switch (choice) {
		case 1: {
			logger.BlueLog("\nEnter new name: ");
			std::string input;
			betterCin(&input, "Sorry, you can't do that!\n", true, [&]() {
				item.name = input; // Update name
				});
			break;
		}
		case 2: {
			logger.BlueLog("\nEnter new date of birth: ");
			std::string input;
			betterCin(&input, "Invalid date format!\n", true, [&]() {
				item.date_of_birth = input; // Update date of birth
				});
			break;
		}
		case 3: {
			logger.BlueLog("\nEnter new status: ");
			std::string input;
			betterCin(&input, "Invalid status!\n", true, [&]() {
				item.status = input; // Update status
				});
			break;
		}
		case 4: {
			logger.BlueLog("\nEnter new grades (subject:grade). Type 'done' to finish: ");
			std::map<std::string, int> new_grades = item.grades; // Start with existing grades
			std::string line;

			while (true) {
				logger.CyanLog("\nEnter subject and grade: ");
				betterCin(&line, "Invalid format! Please enter 'subject:grade' or 'done'.\n", true, [&]() {
					if (line == "done") return false; // Stop if 'done'

					size_t colon_pos = line.find(':');
					if (colon_pos == std::string::npos) return false; // Ensure colon exists

					std::string subject = line.substr(0, colon_pos);
					std::string grade_str = line.substr(colon_pos + 1);

					if (subject.empty() || subject.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 ") != std::string::npos) {
						logger.RedLog("Invalid subject! Only letters, numbers, and spaces are allowed.\n");
						return false;
					}

					bool valid_grade = true;
					for (char c : grade_str) {
						if (!isdigit(c)) {
							valid_grade = false;
							break;
						}
					}

					if (!valid_grade) {
						logger.RedLog("Invalid grade! Please enter a valid grade.\n");
						return false;
					}

					int grade = std::stoi(grade_str);

					if (grade == 0) {
						logger.GreenLog("Grade is 0. Deleting grade for subject: " + subject);
						new_grades.erase(subject); // Remove subject if grade is 0
					}
					else {
						new_grades[subject] = grade; // Add or update subject-grade pair
					}

					return true;
					});

				if (line == "done") break; // Exit loop if done
			}

			item.grades = new_grades; // Update the grades map
			break;
		}
		}
	}

	// Displays details of a specific student
	void view_student_data(const long& student_id) {
		auto it = students_.find(student_id);
		if (it == students_.end()) {
			std::cout << "Student with ID " << student_id << " not found.\n";
			return;
		}

		const student_data& student = it->second;
		// Display student details
		std::cout << "\n\n--- Student Information ---\n";
		std::cout << "Name: " << student.name << "\n";
		std::cout << "ID: " << student.id << "\n";
		std::cout << "Date of Birth: " << student.date_of_birth << "\n";
		std::cout << "Status: " << student.status << "\n";
		std::cout << "Average Grade: " << student.average_grade << "\n";

		// Display grades
		if (!student.grades.empty()) {
			std::cout << "Grades:\n";
			for (const auto& grade : student.grades) {
				std::cout << " - " << grade.first << ": " << grade.second << "\n";
			}
		}
		else {
			std::cout << "No grades available.\n";
		}
	}
};

int main() {
	student student_management_system;

	while (true) {
		// Display menu options
		logger.CyanLog("\nHello, USER!\n");
		logger.CyanLog("Here are the operations:\n");
		logger.BlueLog("1. Add student\n");
		logger.BlueLog("2. Remove student\n");
		logger.BlueLog("3. Edit student\n");
		logger.BlueLog("4. View student data\n");
		logger.BlueLog("5. View all students\n");
		logger.BlueLog("6. Exit\n");

		int input;
		betterCin(&input, "", true, [&]() {
			switch (input) {
			case 1: {
				std::string name;
				long id;
				std::string date_of_birth;
				std::string status;
				std::map<std::string, int> grades;

				logger.MagentaLog("Enter student details:\n");

				// Collect student details
				logger.BlueLog("Name: ");
				betterCin(&name, "Sorry, you can't do that!\n");

				bool validID = false;
				while (!validID) {
					logger.BlueLog("\nID: ");
					betterCin(&id, "Sorry you can't do that!\n");

					validID = student_management_system.students_.find(id) == student_management_system.students_.end(); // Ensure ID is unique
					if (!validID) {
						logger.RedLog("ID already exists! Please choose a different ID.\n");
					}
				}

				logger.BlueLog("\nDate of Birth: ");
				betterCin(&date_of_birth, "Sorry, you can't do that!\n");

				logger.BlueLog("\nStatus: ");
				betterCin(&status, "Sorry, you can't do that!\n");

				// Collect grades
				logger.MagentaLog("Enter grades (subject:grade, command 'done' or wrong value stops it):");
				std::string line;
				while (true) {
					logger.CyanLog("\nEnter subject and grade: ");
					std::getline(std::cin, line);

					if (line == "done") break;

					size_t colon_pos = line.find(':');
					if (colon_pos == std::string::npos) {
						logger.RedLog("Invalid format! Please enter 'subject:grade' or 'done'.\n");
						continue;
					}

					std::string subject = line.substr(0, colon_pos);
					std::string grade_str = line.substr(colon_pos + 1);

					if (subject.empty() || subject.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 ") != std::string::npos) {
						logger.RedLog("Invalid subject! Only letters, numbers, and spaces are allowed.\n");
						continue;
					}

					bool valid_grade = true;
					for (char c : grade_str) {
						if (!isdigit(c)) {
							valid_grade = false;
							break;
						}
					}

					if (!valid_grade) {
						logger.RedLog("Invalid grade! Please enter a number between 0 and 100.\n");
						continue;
					}

					int grade = std::stoi(grade_str);
					if (grade < 0 || grade > 100) {
						logger.RedLog("Grade must be between 0 and 100.\n");
						continue;
					}

					grades[subject] = grade; // Add valid grade to map
				}

				student_management_system.add_student(name, id, date_of_birth, status, grades);
				break;
			}

			case 2: {
				long student_id;
				logger.BlueLog("Enter student ID to remove: ");
				betterCin(&student_id, "Sorry, you can't do that!\n");
				student_management_system.remove_student(student_id);
				logger.GreenLog("Successfully removed student!");
				break;
			}

			case 3: {
				long student_id;
				logger.BlueLog("Enter student ID to edit: ");
				betterCin(&student_id, "Sorry, you can't do that!\n");
				student_management_system.edit_student(student_id);
				break;
			}

			case 4: {
				long student_id;
				logger.BlueLog("Enter student ID to view: ");
				betterCin(&student_id, "Sorry, you can't do that!\n");
				student_management_system.view_student_data(student_id);
				break;
			}

			case 5: {
				// Create a vector to hold the student information so that we can sort it.
				std::vector<std::pair<long, student_data>> students_vec(student_management_system.students_.begin(), student_management_system.students_.end());

				// Sort the vector based on the student name.
				std::sort(students_vec.begin(), students_vec.end(),
					[](const std::pair<long, student_data>& a, const std::pair<long, student_data>& b) {
						return a.second.name < b.second.name; // Compare names alphabetically
				});

				// Display all students in an ordered form.
				logger.YellowLog("INFO: Please use the student ID to get information about them!");

				for (const auto& student : students_vec)
				{
					logger.GreenLog("\n--- Student Information ---\n");
					logger.CyanLog("Name: " + student.second.name + "\n");
					logger.MagentaLog("ID: " + std::to_string(student.first) + "\n");
				}
				break;
			}

			case 6: {
				std::exit(0);
			}

			default: {
				logger.RedLog("Sorry, that doesn't work!");
				break;
			}
			}
			});
	}
}