#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <vector>
using namespace std;

#define SB_NAMEOF(name) #name

namespace myDemo2 {

	template <class T>
	void release(vector<T>& v) {
		for (size_t i = 0; i < v.size(); i++)
			delete v[i];
	}

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

	template <class TType>
	void write(const string& reflectorName, TType& object, ostringstream& os);

	class BaseField {
	public:
		virtual ~BaseField() { }
		virtual void write(const string& reflectorName, ostringstream& os) = 0;
	};

	template <class T>
	class Field : public BaseField {
		T& _object;
	public:
		Field(T& object) : _object(object) { }

		virtual void write(const string& reflectorName, ostringstream& os) {
			myDemo2::write<T>(reflectorName, _object, os);
		}
	};

	class Reflectable { 
		vector<BaseField*> _fields;
	public:
		virtual ~Reflectable() {
			release(_fields);
		}

		template <class T>
		void registerField(T& object) {
			_fields.push_back(new Field<T>(object));
		}

		vector<BaseField*>& getFields() {
			return _fields;
		}
	};

	class MyClass : public Reflectable {
		int _myInt;
		float _myFloat;
	public:
		void setMyInt(int myInt) { _myInt = myInt; }
		void setMyFloat(float myFloat) { _myFloat = myFloat; }
		MyClass() {
			registerField(_myInt);
			registerField(_myFloat);
		}
	};

	class XmlWriter {
	public:
		static void writeReflectable(const string& reflectorName, Reflectable& reflectable, ostringstream& os) {
			auto fields = reflectable.getFields();
			for (size_t i = 0; i < fields.size(); i++) 
				fields[i]->write(reflectorName, os);
		}

		template <class T>
		static void write(T& object, ostringstream& os) {
			os << object << endl;
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
		static void write(const string& reflectorName, TType& object, ostringstream& os) {
			TReflector::template write<TType>(object, os);
		}
	};

	template <class TReflector, class TType>
	struct Writer<TReflector, TType, true> {
		static void write(const string& reflectorName, TType& reflectable, ostringstream& os) {
			TReflector::writeReflectable(reflectorName, reflectable, os);
		}
	};

	template <class TReflector, class TType>
	void writeField(const string& reflectorName, TType& object, ostringstream& os) {
		const bool isReflectable = is_base_of<Reflectable, TType>::value;
		Writer<TReflector, TType, isReflectable>::write(reflectorName, object, os);
	}

	template <class TType>
	void write(const string& reflectorName, TType& object, ostringstream& os) {
		if (reflectorName == SB_NAMEOF(XmlWriter)) {
			writeField<XmlWriter, TType>(reflectorName, object, os);
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