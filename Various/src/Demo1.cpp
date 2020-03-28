#include "Demo1.h"
#include <vector>
#include <sstream>
#include <iostream>
using namespace std;

namespace myDemo1 {

	template <typename TBase, typename TDerived>
	struct is_base_of // check if B is a base of D
	{
		typedef char yes[1];
		typedef char no[2];

		static yes& test(TBase*) {};
		static no& test(...) {};

		static TDerived* get(void) {};

		static const bool value = (sizeof(test(get())) == sizeof(yes));
	};

	class BaseWriter {
	public:
		virtual void write(ostringstream& os) = 0;
	};

	class Reflectable {
	public:
		void write(ostringstream& os) {

		}
	};

	template <class T>
	class Writer : public BaseWriter {
		T& _reference;

	protected:

		void writeReflectable(ostringstream& os) {
			Reflectable *reflectable = (Reflectable*)(&_reference);
			reflectable->write(os);
		}

		template <bool> void write(ostringstream& os);

		template <> void write<false>(ostringstream& os) {
			os << _reference;
		}

		template <> void write<true>(ostringstream& os) {
			os << "TODO: Write reflectable" << endl;
		}

	public:
		Writer(T& reference) : _reference(reference)
		{ }

		void write(ostringstream& os) {
			write<is_base_of<Reflectable, T>::value>(os);
		}
	};


	class MyClass : public Reflectable {
		float _myFloat;
		int _myInt;

	public:
		void setFloat(float myFloat) { _myFloat = myFloat; }
		void setInt(int myInt) { _myInt = myInt; }
	};

	void demo2() {
		int myInt = 42;

		MyClass myClass;
		myClass.setFloat(1.2345f);
		myClass.setInt(1234);

		Writer<MyClass> myClassWriter(myClass);
		Writer<int> myIntWriter(myInt);

		ostringstream os;
		myClassWriter.write(os);
		myIntWriter.write(os);

		cout << os.str() << endl;

		cin.get();
	}

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
		demo2();
		//demo1();
	}
}
