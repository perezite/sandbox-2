#include "Demo1.h"
#include <vector>
#include <sstream>
#include <iostream>
using namespace std;

namespace myDemo1 {

	class BaseWriter {
	public:
		virtual void write(ostringstream& os) = 0;
	};

	template <class T>
	class Writer : public BaseWriter {
		T& _reference;

	public:
		Writer(T& reference) : _reference(reference)
		{ }

		void write(ostringstream& os) {
			os << _reference << endl;
		}
	};

	void demo1() {
		vector<BaseWriter*> writers;

		float myFloat = 1.2345f;
		int myInt = 1234;
		Writer<float> floatWriter(myFloat);
		Writer<int> intWriter(myInt);

		writers.push_back(&floatWriter);
		writers.push_back(&intWriter);

		ostringstream os;
		for (size_t i = 0; i < writers.size(); i++)
			writers[i]->write(os);

		cout << os.str() << endl;
		cin.get();
	}
	
	void run() {
		demo1();
	}
}
