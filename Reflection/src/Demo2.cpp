#include "Demo2.h"
#include <iostream>
#include <string>

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
		int test = 42;
		std::string result;
		TextWriter100::write(test, SB_NAMEOF(test), result);
		std::cout << result << std::endl;
	}


	void run() {
		demo100();
		//void demo1000();
	}
	
	// primitive write
	// primitive read
	// primitive edit
}