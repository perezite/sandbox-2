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

	string tabs(size_t num) {
		ostringstream os;
		for (size_t i = 0; i < num; i++)
			os << "  ";
		return os.str();
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

	struct ReflectionState {
		string reflectorName;
		ostream& os;
		size_t depth = 0;
		ReflectionState(const string& reflectorName_, ostream& os_) :
			reflectorName(reflectorName_), os(os_)
		{ }
	};

	template <class TType>
	void write(TType& object, ReflectionState state);

	class BaseField {
	public:
		virtual ~BaseField() { }
		virtual void write(ReflectionState state) = 0;
	};

	template <class T>
	class Field : public BaseField {
		T& _object;
	public:
		Field(T& object) : _object(object) { }

		virtual void write(ReflectionState state) {
			myDemo2::write<T>(_object, state);
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

	class MyInnerClass : public Reflectable {
		double _myInnerDouble;
	public:
		void setMyInnerDouble(double myInnerDouble) { _myInnerDouble = myInnerDouble; }
		MyInnerClass() {
			registerField(_myInnerDouble);
		}
	};

	class MyClass : public Reflectable {
		int _myInt;
		float _myFloat;
		MyInnerClass _myInnerClass;
	public:
		MyInnerClass& getMyInnerClass() { return _myInnerClass; }
		void setMyInt(int myInt) { _myInt = myInt; }
		void setMyFloat(float myFloat) { _myFloat = myFloat; }
		MyClass() {
			registerField(_myInt);
			registerField(_myFloat);
			registerField(_myInnerClass);
		}
	};

	class XmlWriter {
	public:
		static void writeReflectable( Reflectable& reflectable, ReflectionState state) {
			auto fields = reflectable.getFields();
			state.depth += 1;
			for (size_t i = 0; i < fields.size(); i++) 
				fields[i]->write(state);
		}

		template <class T>
		static void write(T& object, ReflectionState state) {
			state.os << tabs(state.depth) << object << endl;
		}
	};

	template <>
	void XmlWriter::write<float>(float& object, ReflectionState state) {
		auto precision = cout.precision();
		state.os << setprecision(3) << tabs(state.depth) << object << setprecision(precision) << endl;
	}



	template <class TReflector, class TType, bool>
	struct Writer;

	template <class TReflector, class TType>
	struct Writer<TReflector, TType, false> {
		static void write(TType& object, ReflectionState state) {
			TReflector::template write<TType>(object, state);
		}
	};

	template <class TReflector, class TType>
	struct Writer<TReflector, TType, true> {
		static void write(TType& reflectable, ReflectionState state) {
			TReflector::writeReflectable(reflectable, state);
		}
	};

	template <class TReflector, class TType>
	void writeField(TType& object, ReflectionState state) {
		const bool isReflectable = is_base_of<Reflectable, TType>::value;
		Writer<TReflector, TType, isReflectable>::write(object, state);
	}

	template <class TType>
	void write(TType& object, ReflectionState state) {
		if (state.reflectorName == SB_NAMEOF(XmlWriter)) {
			writeField<XmlWriter, TType>(object, state);
		}
	}

	template <class TType>
	void write(TType& object, const string& reflectorName, ostringstream& os) {
		write(object, ReflectionState(reflectorName, os));
	}

	void demo3() {
		MyClass myClass;
		myClass.setMyInt(42);
		myClass.setMyFloat(1.2345f);
		myClass.getMyInnerClass().setMyInnerDouble(9.87654);
		ostringstream os;
		write(myClass, SB_NAMEOF(XmlWriter), os);
		cout << os.str() << endl;
	}

	void demo2() {
		float myFloat = 3.1415f;
		ostringstream os;
		write(myFloat, SB_NAMEOF(XmlWriter), os);
		cout << os.str() << endl;
	}

	void demo1() {
		int myInt = 42;
		ostringstream os;
		write(myInt, SB_NAMEOF(XmlWriter), os);
		cout << os.str() << endl;
	}

	void run() {
		demo3();
		//demo2();
		//demo1();

		cin.get();
	}
}