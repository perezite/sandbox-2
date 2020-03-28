#include "Demo1.h"
#include <vector>
#include <sstream>
#include <iostream>
using namespace std;

namespace myDemo1 {

	template <typename TBase, typename TDerived>
	struct is_base_of // check if TBase is a base of TDerived
	{
		typedef char yes[1];
		typedef char no[2];

		static yes& test(TBase*) {};
		static no& test(...) {};

		static TDerived* get(void) {};

		static const bool value = (sizeof(test(get())) == sizeof(yes));
	};

	class Reflectable {
	public:
		void write(ostringstream& os) {

		}
	};

	template <class T> void stringify(T& value, ostringstream& os) {
		os << value;
	}

	template <> void stringify<int>(int& value, ostringstream& os) {
		os << "(int)" << value;
	}

	class BaseWriter {
	public:
		virtual void write(ostringstream& os) = 0;
	};

	template <class T>
	class Writer : public BaseWriter {
		T& _reference;

	protected:
		template <class U, bool>
		struct MyWrite;

		template <class U> struct MyWrite<U, false> {
			static void write(U& value, ostringstream& os) {
				stringify(value, os);
				os << endl;
			}
		};

		template <class U> struct MyWrite<U, true> {
			static void write(U& value, ostringstream& os) {
				os << "TODO: Write reflectable" << endl;
			}
		};

	public:
		Writer(T& reference) : _reference(reference)
		{ }

		void write(ostringstream& os) {
			const bool isReflectable = is_base_of<Reflectable, T>::value;
			MyWrite<T, isReflectable>::write(_reference, os);
		}
	};

	template <class T>
	class Writer<vector<T>> : public BaseWriter {
		vector<T> _values;
	public:
		Writer(vector<T>& values) : _values(values)
		{ }
		void write(ostringstream& os) {
			os << "[";
			for (size_t i = 0; i < _values.size(); i++) {
				stringify(_values[i], os);
				os << (i < _values.size() - 1 ? ", " : "");
			}
			os << "]" << endl;
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
		float myFloat = 1.2345f;
		vector<int> myInts{ 1, 2, 3 };

		MyClass myClass;
		myClass.setFloat(1.2345f);
		myClass.setInt(1234);

		Writer<int> myIntWriter(myInt);
		Writer<float> myFloatWriter(myFloat);
		Writer<vector<int>> myIntsWriter(myInts);
		Writer<MyClass> myClassWriter(myClass);

		ostringstream os;
		myIntWriter.write(os);
		myFloatWriter.write(os);
		myIntsWriter.write(os);
		myClassWriter.write(os);

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
