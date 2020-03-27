#include "Demo2.h"
#include "Logger.h"
#include "Reflectable.h"
#include "TextSerializer.h"
#include <iostream>
#include <string>
#include <vector>
#include <map>

namespace reflectionDemo2 {

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
			std::istringstream(splitInput[1]) >> result;
		}
	};

	void demo200() {
		// primitive read
		auto testString = "myInt 42";
		int result;
		TextReader200::read(testString, result);
		std::cout << result;
	}

	class MyReflectable : public sb::Reflectable<MyReflectable> {
		SB_CLASS(MyReflectable)
		SB_PROPERTY(int, _myInt)
		SB_PROPERTY(float, _myFloat)
	public:
		void setMyInt(int myInt) { _myInt = myInt; }
		void setMyFloat(float myFloat) { _myFloat = myFloat; }
	};

	void demo300() {
		MyReflectable reflectable;
		reflectable.setMyInt(9876);
		reflectable.setMyFloat(1.2345f);
		auto result = sb::TextSerializer::serialize(reflectable);
		std::cout << result;
	}

	template <class T>
	class MyTest {
	public:
		void print() {
			std::cout << "standard template class" << std::endl;
		}
	};

	template <class T>
	class MyTest<T*> {
	public:
		void print() {
			std::cout << "pointer template class" << std::endl;
		}
	};

	void demo400() {
		MyTest<int>().print();
		MyTest<int*>().print();
	}

	class ConsoleEditor500 {
		static bool _done;

	protected:
		static bool isDone(const std::string& input) {
			_done = input == "exit";
			return _done;			
		}

		template <class T> static void doEdit(T& value) {
			std::cout << "current: " << value << std::endl;
			std::cout << "new: "; 

			std::string input;
			std::cin >> input;
			if (isDone(input)) 
				return;

			std::istringstream is(input);
			is >> value;
		}

	public:
		template <class T> static void edit(T& value) {
			_done = false;
			while (!_done) 
				doEdit(value);
		}
	};

	bool ConsoleEditor500::_done = false;

	void demo500() {
		// primitive edit
		int val = 42;
		float val2 = 3.1415f;
		ConsoleEditor500::edit(val);
		ConsoleEditor500::edit(val2);
	}

	void demo1000() {
		// primitive write, read and edit
		int test = 42;
		std::string testString;
		TextWriter100::write(test, SB_NAMEOF(test), testString);
		std::cout << testString << std::endl;

		int result;
		TextReader200::read(testString, result);
		std::cout << result << std::endl;

		int test2 = 43;
		ConsoleEditor500::edit(test2);
	}
	void run() {
		demo1000();
		//demo500();
		//demo400();
		//demo300();
		//demo200();
		//demo150();
		//demo100();
	}
}