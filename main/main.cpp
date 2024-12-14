#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <chrono>
#include <thread>

#include "../include/helper.h"

using namespace ezc;

struct
	grades_str
{
	std::string subject;
	int grade;
};

// Structure to hold student data
struct student_data
{
	std::string name;
	long id;
	std::string date_of_birth;
	std::string status;
	std::vector<grades_str> grades;
	float average_grade = 0;
};


std::string tolower(const std::string& input) {
	std::string result = input;
	for (char& c : result) {
		c = (c >= 'A' && c <= 'Z') ? c + ('a' - 'A') : c;
	}
	return result;
}

class student
{
private:
	int count_ = 0; // amount of students
public:
	std::vector<student_data> students_ = {};
	void add_student(const std::string& name, const long& id, const std::string& date_of_birth,
		const std::string& status, const std::vector<grades_str>& grades)
	{
		static student_data data;

		data.name = name;
		data.date_of_birth = date_of_birth;
		data.status = status;
		data.grades = grades;
		data.id = id;

		// Calculate average grade properly
		int total_grade = 0;
		for (const auto& grade : data.grades)
		{
			total_grade += grade.grade;
		}
		data.average_grade = (data.grades.empty()) ? 0 : static_cast<float>(total_grade) / data.grades.size();

		students_.push_back(data); // push data

		count_++; // update count
	}


	void remove_student(const int& student_id)
	{
		for (auto it = students_.begin(); it != students_.end(); ++it)
		{
			if (it->id == student_id)
			{
				students_.erase(it);
				break;
			}
		}
		count_--; // upgrade count
	}

	void edit_student(const int& student_id) {
		for (auto& item : students_) {
			if (item.id == student_id) {
				logger.BlueLog("1. Name\n");
				logger.BlueLog("2. Date of Birth\n");
				logger.BlueLog("3. Status\n");
				logger.BlueLog("4. Grades\n");

				int choice;
				betterCin(&choice, "\nInvalid choice! Please select a valid option.\n", true, [&]() {
					return choice >= 1 && choice <= 4;
					});

				switch (choice) {
				case 1: {
					logger.BlueLog("\nEnter new name: ");
					std::string input;
					betterCin(&input, "Sorry, you can't do that!\n", true, [&]() {
						item.name = input;
						});
					break;
				}
				case 2: {
					logger.BlueLog("\nEnter new date of birth: ");
					std::string input;
					betterCin(&input, "Invalid date format!\n", true, [&]() {
						item.date_of_birth = input;
						});
					break;
				}
				case 3: {
					logger.BlueLog("\nEnter new status: ");
					std::string input;
					betterCin(&input, "Invalid status!\n", true, [&]() {
						item.status = input;
						});
					break;
				}
				case 4: {
					logger.BlueLog("\nEnter new grades (subject:grade). Type 'done' to finish: ");
					std::vector<grades_str> new_grades = item.grades; // Start with existing grades
					std::string line;

					while (true) {
						logger.CyanLog("\nEnter subject and grade: ");
						betterCin(&line, "Invalid format! Please enter 'subject:grade' or 'done'.\n", true, [&]() {
							if (line == "done") return false;

							size_t colon_pos = line.find(':');
							if (colon_pos == std::string::npos) return false;

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

							bool found = false;
							for (auto& grade_entry : new_grades) {
								if (grade_entry.subject == subject) {
									if (grade == 0) {
										logger.GreenLog("Grade is 0. Deleting grade for subject: " + subject);
										// Delete the grade if it is 0
										new_grades.erase(std::remove_if(new_grades.begin(), new_grades.end(), [&subject](const grades_str& grade_entry) {
											return grade_entry.subject == subject;
											}), new_grades.end());
									}
									else {
										grade_entry.grade = grade; // Update the grade if subject is found
									}
									found = true;
									break;
								}
							}

							if (!found && grade != 0) {
								new_grades.push_back({ subject, grade }); // Add new grade if subject doesn't exist
							}

							return true;
							});

						if (line == "done") break;
					}

					item.grades = new_grades; // Update the grades list after adding/updating
					break;
				}
				}

				break;
			}
		}
	}





	void view_student_data(const long& student_id)
	{
		bool student_found = false;

		for (const auto& student : students_)
		{
			if (student.id == student_id)
			{
				student_found = true;
				// Display student details
				std::cout << "\n\n--- Student Information ---\n";
				std::cout << "Name: " << student.name << "\n";
				std::cout << "ID: " << student.id << "\n";
				std::cout << "Date of Birth: " << student.date_of_birth << "\n";
				std::cout << "Status: " << student.status << "\n";
				std::cout << "Average Grade: " << student.average_grade << "\n";

				// Display grades
				if (!student.grades.empty())
				{
					std::cout << "Grades:\n";
					for (const auto& grade : student.grades)
					{
						std::cout << " - " << grade.subject << ": " << grade.grade << "\n";
					}
				}
				else
				{
					std::cout << "No grades available.\n";
				}

				break; // Exit the loop once the student is found
			}
		}

		// If student ID is not found
		if (!student_found)
		{
			std::cout << "Student with ID " << student_id << " not found.\n";
		}
	}





};

int main()
{
	student student_management_system;

	while (true)
	{
		logger.CyanLog("\nHello, USER!\n");

		logger.CyanLog("Here are the operations:\n");

		logger.BlueLog("1. Add student\n");
		logger.BlueLog("2. Remove student\n");
		logger.BlueLog("3. Edit student\n");
		logger.BlueLog("4. View student data\n");
		logger.BlueLog("5. Search student by name\n");
		logger.BlueLog("6. Exit\n");

		int input;
		betterCin(&input,
			"\nSorry, something unexpected happened!\n",
			true,
			[&] {
				switch (input)
				{
				case 1:
				{
					std::string name;
					long id;
					std::string date_of_birth;
					std::string status;
					std::vector<grades_str> grades;

					logger.MagentaLog("Enter student details:\n");

					logger.BlueLog("Name: ");
					betterCin(&name, "Sorry, you can't do that!\n");

					bool validID = false;
					while (true) {
						logger.BlueLog("\nID: ");
						betterCin(&id, "Sorry, you can't do that!\n");

						validID = true; // Assume ID is valid unless proven otherwise
						for (int i = 0; i < student_management_system.students_.size(); i++) {
							if (student_management_system.students_[i].id == id) {
								logger.RedLog("Student with this ID already exists! Please choose a different ID.\n");
								validID = false; // ID is not valid if it already exists
								break; // Exit the loop to prompt for a new ID
							}
						}

						if (validID) {
							break; // If no duplicate is found, break out of the while loop
						}
					}


					logger.BlueLog("\nDate of Birth: ");
					betterCin(&date_of_birth, "Sorry, you can't do that!\n");

					logger.BlueLog("\nStatus: ");
					betterCin(&status, "Sorry, you can't do that!\n");

					logger.MagentaLog("Enter grades (subject:grade, command 'done' or wrong value stops it):");
					std::string line;
					// don't touch, i don't fucking know why my code wont work without these
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

						grades.push_back({ subject, grade });
					}

					student_management_system.add_student(name, id, date_of_birth, status, grades);
					break;
				}

				case 2:
				{
					long student_id;
					logger.BlueLog("Enter student ID to remove: ");
					betterCin(&student_id, "Sorry, you can't do that!\n");
					student_management_system.remove_student(student_id);
					logger.GreenLog("Successfully removed student!");
					break;
				}

				case 3:
				{
					long student_id;
					logger.BlueLog("Enter student ID to edit: ");
					betterCin(&student_id, "Sorry, you can't do that!\n");
					student_management_system.edit_student(student_id);
					break;
				}

				case 4:
				{
					long student_id;
					logger.BlueLog("Enter student ID to view: ");
					betterCin(&student_id, "Sorry, you can't do that!\n");
					student_management_system.view_student_data(student_id);
					break;
				}

				case 5:
				{
					std::string name_to_search;
					logger.BlueLog("Enter student name to search: ");
					betterCin(&name_to_search, "Sorry, you can't do that!\n");
					bool found = false;
					for (int i = 0; i < student_management_system.students_.size(); i++)
					{
						if (tolower(student_management_system.students_[i].name) == tolower(name_to_search))
						{
							logger.GreenLog("Student found:\n");
							student_management_system.view_student_data(student_management_system.students_[i].id);
							found = true;
							break;
						}
						else
						{
							continue;
						}


					}

					if (!found) logger.RedLog("Student not found.\n");
					break;
				}

				case 6:
				{
					std::exit(0);
				}

				default:
				{
					logger.RedLog("Sorry, that doesn't work!");
					break;
				}
				}
			});
	}
}

