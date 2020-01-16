#include "Demo9.h"
#include "Logger.h"
#include "Macro.h"

namespace reflectionDemo9 {
	class MyBase {
		int _myInt;
	public:
		int getMyInt() const { return _myInt; }
		void setMyInt(int myInt) { _myInt = myInt; }
	};

	class MyDerived : public MyBase {
		float _myFloat;
	public:
		float getMyFloat() const { return _myFloat; }
		void setMyInt(float myFloat) { _myFloat = myFloat; }
	};

	/*
	void demo() {
		// write
		MyDerived myDerived;
		myDerived.setMyInt(42);
		myDerived.setMyFloat(1.2345f);
		ostringstream os;
		TextWriter::write(myDerived, os);

		// read
		MyDerived myDerived2;
		istringstream is(os);
		TextReader::read(myDerived2, is);
		cout << myDerived2.getMyInt() << endl;
		cout << myDerived2.getMyFloat() << endl;

		// edit
		string value; size_t index;
		while(value != "exit") {
			TextWriter::write(myDerived2, cout, true);
			cout << "Enter the index of the property to edit: ";
			getline(cin, index);
			cout << "Enter the new value for the property (or exit to leave): ";
			getline(cin, value);
			ConsoleEditor::edit(parse<int>(index), value);
		}
	}
	*/

	void run() {
		// demo9: inheritance (write, read, edit)
	}
}

