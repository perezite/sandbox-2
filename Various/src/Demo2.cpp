#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
using namespace std;

#define SB_NAMEOF(name) #name

//#ifndef __INTELLISENSE__
//#define SB_FIELD(type, name)														\
//		type name;																	\
//		void create_property_##name() {												\
//			createProperty<type>(#name, name, #type);								\
//		}																			\
//		static void add_property_creator_##name() {									\
//			addPropertyCreator(&CurrentClass::create_property_##name, #name);		\
//		}																			\
//		Invocation<add_property_creator_##name> invocation_##name;						
//#else
//	#define SB_PROPERTY(type, name) type name;
//#endif

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

	template <void(*Action)()> 
	class Invocation {
	public:
		Invocation() {
			Action();
		}
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

	class BaseReflectable {
	public:
		virtual vector<BaseField*>& getFields() = 0;
	};

	template <class T>
	class Reflectable : public BaseReflectable { 
		typedef void(*FieldRegistration)(void*);
		vector<BaseField*> _fields;
		static map<string, FieldRegistration> FieldRegistrations;
		bool _fieldsInitialized = false;
	protected:
		void initializeFields() {
			auto self = (void*)this;
			for (auto it = FieldRegistrations.begin(); it != FieldRegistrations.end(); ++it) 
				it->second(self);
			_fieldsInitialized = true;
		}
	public:
		virtual ~Reflectable() { release(_fields); }
		static void addRegistration(FieldRegistration registration, const std::string name) { 
			FieldRegistrations[name] = registration;
		}
		template <class U> void registerField(U& object) {
			_fields.push_back(new Field<U>(object));
		}
		virtual vector<BaseField*>& getFields() {
			if (!_fieldsInitialized)
				initializeFields();
			return _fields;
		}
	};

	template <class T> map<string, void(*)(void*)> Reflectable<T>::FieldRegistrations;


	class MyInnerClass : public Reflectable<MyInnerClass> {
		double _myInnerDouble;
	public:
		void setMyInnerDouble(double myInnerDouble) { _myInnerDouble = myInnerDouble; }
		MyInnerClass() {
			registerField(_myInnerDouble);
		}
	};

	class MyClass : public Reflectable<MyClass> {
		int _myInt;
		static void addField_myInt(void* instance) {
			auto parent = (MyClass*)instance;
			parent->registerField(parent->_myInt);
		}
		static void register_myInt() { addRegistration(addField_myInt, SB_NAMEOF(_myInt)); }
		Invocation<register_myInt> invoke_register_myInt;

		float _myFloat;
		MyInnerClass _myInnerClass;
	public:
		MyInnerClass& getMyInnerClass() { return _myInnerClass; }
		void setMyInt(int myInt) { _myInt = myInt; }
		void setMyFloat(float myFloat) { _myFloat = myFloat; }
		
		MyClass() {
			//registerField(_myInt);
			registerField(_myFloat);
			registerField(_myInnerClass);
		}
	};

	class XmlWriter {
	protected:
		template <class T> static string element(T& elem) {
			ostringstream os;
			os << "<element>" << elem << "</element>";
			return os.str();
		}
	public:
		static void writeReflectable(BaseReflectable& reflectable, ReflectionState state) {
			auto fields = reflectable.getFields();
			auto currentDepth = state.depth;
			state.os << tabs(currentDepth) << "<object>" << endl;
			state.depth += 1;
			for (size_t i = 0; i < fields.size(); i++) 
				fields[i]->write(state);
			state.os << tabs(currentDepth) << "</object>" << endl;
		}

		template <class T>
		static void write(T& object, ReflectionState state) {
			state.os << tabs(state.depth) << element(object) << endl;
		}
	};

	template <>
	void XmlWriter::write<float>(float& object, ReflectionState state) {
		auto precision = cout.precision();
		state.os << setprecision(3) << tabs(state.depth) << element(object) << setprecision(precision) << endl;
	}

	template <class TReflector, class TType, bool> struct Writer;

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
		const bool isReflectable = is_base_of<BaseReflectable, TType>::value;
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
		auto test = os.str();
		cout << test << endl;
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