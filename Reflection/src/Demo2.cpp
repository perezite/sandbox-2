#include "Demo2.h"
#include "Logger.h"
#include <iostream>
#include <string>
#include <vector>

namespace reflectionDemo2 {
	#define SB_NAMEOF(name) #name

	void demo1000() {
		/* 
		int test = 42;
		std::string testString;
		TextWriter1000::write(test, nameof(test), testString);
		int result;
		TextReader1000::read(result, testString);
		int test2 = 43;
		TextEditor::edit(test2);
		*/
	}

	class TextWriter100 {
	public:
		template <class T> static void write(const T& value, const std::string& name, std::string& result) {
			std::ostringstream os;
			os << name << " " << value << " ";
			result = os.str();
		}
	};

	void demo100() {
		// primtive write
		int test = 42;
		std::string result;
		TextWriter100::write(test, SB_NAMEOF(test), result);
		std::cout << result << std::endl;
	}

	void split(const std::string& s, const std::string& delimiter, std::vector<std::string>& result) {
		size_t start = 0;
		size_t pos = 0;
		size_t delimiterLen = delimiter.length();
		while (true) {
			pos = s.find(delimiter, start);
			if (pos != std::string::npos) {
				size_t len = pos - start;
				if (len > 0)
					result.emplace_back(s.substr(start, len));
				start = pos + delimiterLen;
			}
			else {
				auto strLen = s.length();
				size_t len = s.length() - start;
				if (len > 0)
					result.emplace_back(s.substr(start, len));
				break;
			}
		}
	}

	void demo150() {
		std::string testString = "int   test42";
		std::vector<std::string> result;
		split(testString, " ", result);
		for (size_t i = 0; i < result.size(); i++)
			std::cout << result[i] << std::endl;
	}

	class TextReader200 {
	public:
		template <class T> static void read(const std::string& input, T& result) {
			std::vector<std::string> splitInput;
			split(input, " ", splitInput);
			SB_ERROR_IF(splitInput.size() != 2, "Bad format");
			std::istringstream(splitInput[0]) >> result;
		}
	};

	void demo200() {
		// primitive read
		auto testString = "42 myInt";
		int result;
		TextReader200::read(testString, result);
		std::cout << result;
	}

	void run() {
		//demo1000();
		demo200();
		//demo150();
		//demo100();
	}
	
	// primitive edit
}