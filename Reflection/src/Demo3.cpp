#include "Demo3.h"
#include "Macro.h"
#include <string>
#include <sstream>
#include <iostream>
#include <vector>

namespace reflectionDemo3 {

	void demo1000() {
		// primitive write, read and edit
		/*int* test = new int();
		std::string testString;
		TextWriter100::write(test, SB_NAMEOF(test), testString);
		std::cout << testString << std::endl;

		int* result = NULL;
		TextReader200::read(testString, result);
		std::cout << result << std::endl;

		int test2 = 43;
		ConsoleEditor500::edit(test2);*/
	}

	template <class T>
	std::string toString(const T& t) {
		static std::ostringstream os;
		os.str(""); os.clear();
		os << t;
		return os.str();
	}

	template <class T>
	void release(std::vector<T*>& v) {
		for (size_t i = 0; i < v.size(); i++)
			delete v[i];
	}

	template <class T>
	void inspect100(const std::string& name, const T& t, std::string& result);

	struct BaseReflectablePointer100 {
		size_t id;
		BaseReflectablePointer100(size_t id_) : id(id_)
		{ }
		virtual void write(std::string& result) = 0;
	};

	template <class T>
	struct ReflectablePointer100 : public BaseReflectablePointer100 {
		T* pointer;
		ReflectablePointer100(size_t id_, T* pointer_) : BaseReflectablePointer100(id_), pointer(pointer_)
		{ }
		virtual void write(std::string& result) {
			inspect100(toString(id), *pointer, result);
		}
	};

	class TextWriter100 {
		static size_t _currentId;
		static std::vector<BaseReflectablePointer100*> _reflectablePointers;
	protected:
		static void writePointers(std::string& result) {
			for (size_t i = 0; i < _reflectablePointers.size(); i++)
				_reflectablePointers[i]->write(result);
 			
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
			BaseReflectablePointer100* reflectablePointer = new ReflectablePointer100<T>(_currentId, pointer);
			_reflectablePointers.push_back(reflectablePointer);
			writeValue(name, _currentId, result);
			_currentId += 1;
			writePointers(result);
		}
	};

	std::vector<BaseReflectablePointer100*> TextWriter100::_reflectablePointers;
	size_t TextWriter100::_currentId = 0;

	template <class T>
	void inspect100(const std::string& name, const T& t, std::string& result) {
		TextWriter100::write(name, t, result);
	}

	void demo100() {
		// primitive content pointer read
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

	void run() {
		demo100();
	}

	// demo3: primtive content pointer read
	// demo3: primtive content pointer write
	// demo3: primtive content pointer edit
}
