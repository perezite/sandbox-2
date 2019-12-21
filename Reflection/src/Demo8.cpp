#include "Demo8.h"

namespace reflectionDemo8 {
	using namespace std;

	/*
	class MyClass {
	public:
		SB_PROPERTY(int, myInt)
		SB_PROPERTY(int* myIntPointer)
	public:
		MyClass() : myIntPointer(&myInt) {
		
		}
	}

	int myExternalInt;

	class MyInvalidClass {
	public:
		SB_PROPERTY(int*, myExternalIntPointer);
	public:
		MyInvalidClass() : myExternalIntPointer(&myExternalInt) {
		}
	}

	void demo1000() {
		// write
		MyClass myClass;
		myClass.myInt = 42;
		std::ostringstream os;
		TextWriter::write(myClass, os);
		cout << os.str() << endl;

		// read
		MyClass myClass2;
		std::istringstream is(os.str());
		TextReader::read(myClass2, is);
		cout << myClass2.myInt << endl;
		cout << *myClass2.myIntPointer << endl;

		// edit
		string index; string value;
		while (value != "exit") {
			TextWriter::write(myClass2, cout, true);
			cout << "Enter the index of the property to edit: ";
			getline(cin, index);
			cout << "Enter the new value for the property (or exit to leave): ";
			getline(cin, value);
			ConsoleEditor::edit(parse<int>(index), value);
		}

		// prevent external pointers

	}
	*/

	void run() {
		// demo8: link pointers (write, raed, edit)
		//demo1000();
	}
}
