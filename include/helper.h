#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <limits>
#include <functional>
#include <typeinfo>

// Colors for the ColorLogger class
#define RESET   "\033[0m"        // Reset to default
#define RED     "\033[31m"       // Red text
#define GREEN   "\033[32m"       // Green text
#define YELLOW  "\033[33m"       // Yellow text
#define BLUE    "\033[34m"       // Blue text
#define MAGENTA "\033[35m"       // Magenta text
#define CYAN    "\033[36m"       // Cyan text

namespace ezc {
	// ColorLogger class
	class ColorLogger {
	public:
		void BlueLog(std::string msg) {
			std::cout << BLUE << msg << RESET;
		}

		void CyanLog(std::string msg) {
			std::cout << CYAN << msg << RESET;
		}

		void GreenLog(std::string msg) {
			std::cout << GREEN << msg << RESET;
		}

		void MagentaLog(std::string msg) {
			std::cout << MAGENTA << msg << RESET;
		}

		void RedLog(std::string msg) {
			std::cout << RED << msg << RESET;
		}

		void YellowLog(std::string msg) {
			std::cout << YELLOW << msg << RESET;
		}
	};

	inline ColorLogger logger;

	// Math class
	class math {
	public:
		template <typename T>
		T randomNumber(T x, T y) {
			// Check if x or y is not a number
			if (x % 1 != 0 || y % 1 != 0) {
				logger.RedLog("Unable to generate random number!");
				logger.RedLog("randomNumber(x ,y)");
				return 0;
			}

			if (x > y) {
				std::swap(x, y); // Ensure x is the smaller number
			}

			return rand() % (y - x + 1) + x; // Generate random number
		}
	};

	inline ezc::math MathUtils;

	// Vector or array utilities
	class vector_or_array {
	public:
		template <typename T>
		int getArrayLength(T& array) {
			if (!array[0]) {
				logger.RedLog("Your array is empty or not an array!");
				logger.RedLog("ezc::vector_array::getArrayLength() error");
				return 0;
			}

			return sizeof(array) / sizeof(array[0]);
		}

		template <typename T>
		std::vector<T> rearrangeOrder(std::vector<T>& vector, bool ascending = false) {
			if (ascending) {
				std::sort(vector.begin(), vector.end());
			}
			else {
				std::sort(vector.rbegin(), vector.rend());
			}

			return vector;
		}

		template <typename T>
		std::vector<T> shuffle(std::vector<T>& vector, unsigned int shuffleTimes = 1) {
			for (unsigned int i = 0; i < shuffleTimes; i++) {
				std::shuffle(vector.begin(), vector.end());
			}

			return vector;
		}
	};

	template <typename T, typename Lambda = std::function<void()>>
	void betterCin(
		T* mainValue,
		const std::string& errorMessage,
		const bool redColour = true,
		Lambda lambda_function = []() {}
	) {
		bool validInput = false;
		while (!validInput) {
			// Clear the error state of the stream
			std::cin.clear();

			if constexpr (std::is_same_v<T, std::string>) {
				std::getline(std::cin, *mainValue); // Read the entire line for strings

				if (mainValue->empty()) { // Handle empty string input
					if (redColour) {
						logger.RedLog(errorMessage);
						std::cout << std::endl;
					}
					else {
						std::cout << errorMessage << std::endl;
					}
					continue; // Retry input
				}
			}
			else {
				std::cin >> *mainValue; // Read for other data types

				if (std::cin.fail()) { // Handle input failure
					std::cin.clear(); // Clear the error flag
					std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input
					if (redColour) {
						logger.RedLog(errorMessage);
						std::cout << std::endl;
					}
					else {
						std::cout << errorMessage << std::endl;
					}
					continue; // Retry input
				}

				// Discard any extra input after the valid input (e.g., leftover newline)
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			}

			// Call the lambda function for custom validation or actions
			lambda_function();

			// Input is considered valid
			validInput = true;
		}
	}




} // namespace ezc
