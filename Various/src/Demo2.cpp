#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
using namespace std;

#define SB_NAMEOF(name) #name

namespace myDemo2 {

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

	class Reflectable { };

	class MyClass : public Reflectable {
		int _myInt;
		float _myFloat;
	public:
		void setMyInt(int myInt) { _myInt = myInt; }
		void setMyFloat(float myFloat) { _myFloat = myFloat; }
	};

	class XmlWriter {
	public:
		static void writeReflectable(Reflectable& reflectable, ostringstream& os) {
			os << "TODO: Write reflectable" << endl;
		}

		template <class T>
		static void write(T& object, ostringstream& os) {
			os << object;
		}
	};

	template <>
	void XmlWriter::write<float>(float& object, ostringstream& os) {
		os << setprecision(3) << object << endl;
	}

	template <class TReflector, class TType, bool>
	struct Writer;

	template <class TReflector, class TType>
	struct Writer<TReflector, TType, false> {
		static void write(TType& object, ostringstream& os) {
			TReflector::template write<TType>(object, os);
		}
	};

	template <class TReflector, class TType>
	struct Writer<TReflector, TType, true> {
		static void write(TType& reflectable, ostringstream& os) {
			TReflector::writeReflectable(reflectable, os);
		}
	};

	template <class TReflector, class TType>
	void write(TType& object, ostringstream& os) {
		const bool isReflectable = is_base_of<Reflectable, TType>::value;
		Writer<TReflector, TType, isReflectable>::write(object, os);
	}

	template <class TType>
	void write(const string& reflectorName, TType& object, ostringstream& os) {
		if (reflectorName == SB_NAMEOF(XmlWriter)) {
			write<XmlWriter, TType>(object, os);
		}
	}

	void demo3() {
		MyClass myClass;
		myClass.setMyInt(42);
		myClass.setMyFloat(1.2345f);
		ostringstream os;
		write(SB_NAMEOF(XmlWriter), myClass, os);
		cout << os.str() << endl;
	}

	void demo2() {
		float myFloat = 3.1415f;
		ostringstream os;
		write(SB_NAMEOF(XmlWriter), myFloat, os);
		cout << os.str() << endl;
	}

	void demo1() {
		int myInt = 42;
		ostringstream os;
		write(SB_NAMEOF(XmlWriter), myInt, os);
		cout << os.str() << endl;
	}

	void run() {
		demo3();
		//demo2();
		//demo1();

		cin.get();
	}
}