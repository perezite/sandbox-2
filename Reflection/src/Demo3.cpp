#include "Demo3.h"
#include "Macro.h"
#include "Logger.h"
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <map>
#include <stdlib.h>

namespace reflectionDemo3 {

	template <class T>
	std::string toString(const T& t) {
		static std::ostringstream os;
		os.str(""); os.clear();
		os << t;
		return os.str();
	}

	template <class T>
	T fromString(const std::string& str) {
		static std::istringstream is;
		is.str(str);
		T t;
		is >> t;
		return t;
	}

	template <class T>
	void release(std::vector<T*>& v) {
		for (size_t i = 0; i < v.size(); i++)
			delete v[i];
	}

	bool endsWith(std::string& input, const std::string& token) {
		if (input.empty())
			return false;

		return input.substr(input.length() - token.length(), token.length()) == token;
	}

	void trimEnd(std::string& input, const std::string& token) {
		if (endsWith(input, token)) {
			input.erase(input.length() - token.length());
			trimEnd(input, token);
		}
	}

	static std::string InspectorName = "Undefined";

	template <class T>
	void inspect100(const std::string& name, T& t, std::string& str);

	struct BaseReflectablePointer100 {
		size_t id;
		virtual ~BaseReflectablePointer100() { }
		BaseReflectablePointer100(size_t id_) : id(id_)
		{ }
		virtual void inspect(std::string& str) = 0;
	};

	template <class T>
	struct ReflectablePointer100 : public BaseReflectablePointer100 {
		T* pointer;
		ReflectablePointer100(size_t id_, T* pointer_) : BaseReflectablePointer100(id_), pointer(pointer_)
		{ }
		virtual void inspect(std::string& str) {
			inspect100(toString(id), *pointer, str);
		}
	};

	class TextWriter100 {
		static size_t _currentId;
		static std::vector<BaseReflectablePointer100*> _reflectablePointers;
	protected:
		static void writePointers(std::string& result) {
			for (size_t i = 0; i < _reflectablePointers.size(); i++)
				_reflectablePointers[i]->inspect(result);
		}
		template <class T>
		static void writeValue(const std::string& name, const T& value, std::string& result) {
			std::ostringstream os;
			os << name << " " << value << " " << std::endl;
			result += os.str();
		}
	public:
		virtual ~TextWriter100() { release(_reflectablePointers); }
		template <class T> static void write(const std::string& name, const T& value, std::string& result) {
			writeValue(name, value, result);
		}
		template <class T> static void write(const std::string& name, T* pointer, std::string& result) {
			InspectorName = SB_NAMEOF(TextWriter100);
			BaseReflectablePointer100* reflectablePointer = new ReflectablePointer100<T>(_currentId, pointer);
			_reflectablePointers.push_back(reflectablePointer);
			writeValue(name, _currentId, result);
			_currentId += 1;
			writePointers(result);
		}
	};

	std::vector<BaseReflectablePointer100*> TextWriter100::_reflectablePointers;
	size_t TextWriter100::_currentId = 0;

	void demo100() {
		// primitive content pointer write
		int* test = new int;
		*test = 42;
		std::string result;
		TextWriter100::write(SB_NAMEOF(test), test, result);
		std::cout << result;

		int test2 = 43;
		result = "";
		TextWriter100::write(SB_NAMEOF(test2), test2, result);
		std::cout << result;
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

	void myAssert(bool expression, const std::string& message) {
		if (expression) {
			std::cout << message << std::endl;
			std::cin.get();
			exit(0);
		}
	}

	void popLine(std::string& input, std::string& output) {		
		trimEnd(input, "\n"); trimEnd(input, " ");
		auto pos = input.find('\n');
		
		if (pos == std::string::npos) {
			output = input;
			input = "";
			return;
		}

		output = input.substr(0, pos);
		input = input.substr(pos + 1);
	}

	class TextReader200 {	
		static size_t _currentId;
		static std::map<size_t, BaseReflectablePointer100*> _reflectablePointers;
	protected:
		static void readPointer(std::string& line) {
			std::vector<std::string> parts; split(line, " ", parts);
			SB_ERROR_IF(parts.size() != 2, "bad format");
			auto reflectablePointer = _reflectablePointers[fromString<size_t>(parts[0])];
			reflectablePointer->inspect(line);
		}
		static void readPointers(std::string& input) {
			if (input.length() == 0)
				return;

			std::string line;
			do {
				popLine(input, line);
				readPointer(line);
			} while (input.length() > 0);
		}
	public:
		template <class T> static void read(const std::string& input, T& result) {
			std::vector<std::string> parts; split(input, " ", parts);
			SB_ERROR_IF(parts.size() != 2, "bad format");
			std::istringstream is(parts[1]);
			is >> result;
		}
		template <class T> static void read(std::string& input, T*& result) {
			InspectorName = SB_NAMEOF(TextReader200);
			std::string line; popLine(input, line);
			std::vector<std::string> parts; split(line, " ", parts);
			SB_ERROR_IF(result != NULL, "target to write must be null");
			SB_ERROR_IF(parts.size() != 2, "bad format");
			result = new T();
			auto reflectablePointer = new ReflectablePointer100<T>(fromString<int>(parts[0]), result);
			_reflectablePointers[_currentId] = reflectablePointer;
			readPointers(input);
			_currentId += 1;
		}
	};

	size_t TextReader200::_currentId = 0;
	std::map<size_t, BaseReflectablePointer100*> TextReader200::_reflectablePointers;

	template <class T>
	void inspect100(const std::string& name, T& t, std::string& str) {
		if (InspectorName == SB_NAMEOF(TextWriter100))
			TextWriter100::write(name, t, str);
		else if (InspectorName == SB_NAMEOF(TextReader200))
			TextReader200::read(str, t);
		else
			SB_ERROR(InspectorName << " is not a valid inspector");
	}

	void demo200() {
		// primitive content pointer read
		std::string testString = "myInt 0 \n 0 42";
		int* result = NULL;
		TextReader200::read(testString, result);
		std::cout << *result << std::endl;

		std::string testString2 = "myInt 43";
		int result2;
		TextReader200::read(testString2, result2);
		std::cout << result2 << std::endl;
	}

	class ConsoleEditor300 {
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
		template <class T> static void doEdit(T* value) {
			doEdit(*value);
		}
	public:
		template <class T> static void edit(T& value) {
			_done = false;
			while (!_done)
				doEdit(value);
		}
	};

	bool ConsoleEditor300::_done = false;

	void demo300() {
		// demo3: primitive content pointer edit
		int* test2 = new int;
		*test2 = 42;
		ConsoleEditor300::edit(test2); 
	}

	void demo1000() {
		// primitive write, read and edit
		int* test = new int();
		*test = 42;
		std::string testString;
		TextWriter100::write(SB_NAMEOF(test), test, testString);
		std::cout << testString << std::endl;

		int* result = NULL;
		TextReader200::read(testString, result);
		std::cout << *result << std::endl;

		ConsoleEditor300::edit(result);
	}

	void run() {
		demo1000();
		//demo300();
		// demo200();
		// demo100();
	}
}
