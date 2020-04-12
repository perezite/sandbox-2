#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
using namespace std;

#define SB_NAMEOF(name) #name

#ifndef __INTELLISENSE__
#define SB_CLASS(className) \
	typedef className CurrentClass; \
	inline virtual const std::string getClassName() { return #className; }
#else
	#define SB_CLASS(className)
#endif

#ifndef __INTELLISENSE__
	#define SB_FIELD(type, name)													\
		type name;																	\
		static void addField_##name(void* instance) {								\
			auto parent = (CurrentClass*)instance;									\
			parent->registerField(parent->name, SB_NAMEOF(name));					\
		}																			\
		static void register_##name() {												\
			addRegistration(addField_##name, SB_NAMEOF(name));						\
		}																			\
		Invocation<register_##name> invokeRegistration_##name;
#else
	#define SB_FIELD(type, name) type name;
#endif

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
	void write(TType& object, const string& name, ReflectionState state);

	class BaseField {
	public:
		virtual ~BaseField() { }
		virtual void write(ReflectionState state) = 0;
	};

	template <class T>
	class Field : public BaseField {
		T& _object;
		string _name;
	public:
		Field(T& object, const string& name) : _object(object), _name(name) { }

		virtual void write(ReflectionState state) {
			myDemo2::write<T>(_object, _name, state);
		}
	};

	class BaseReflectable {
	public:
		virtual vector<BaseField*>& getFields() = 0;
	};

	template <class T>
	class Reflectable : public BaseReflectable { 
		typedef void(*FieldRegistration)(void*);
		static map<string, FieldRegistration> FieldRegistrations;
		vector<BaseField*> _fields;
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
		template <class U> void registerField(U& object, const string& name) {
			_fields.push_back(new Field<U>(object, name));
		}
		virtual vector<BaseField*>& getFields() {
			if (!_fieldsInitialized)
				initializeFields();
			return _fields;
		}
	};

	template <class T> map<string, void(*)(void*)> Reflectable<T>::FieldRegistrations;

	class MyInnerClass : public Reflectable<MyInnerClass> {
		SB_CLASS(MyInnerClass)
		SB_FIELD(double, _myInnerDouble)
	public:
		void setMyInnerDouble(double myInnerDouble) { _myInnerDouble = myInnerDouble; }
	};

	class MyClass : public Reflectable<MyClass> {
		SB_CLASS(MyClass)
		SB_FIELD(int, _myInt)
		SB_FIELD(MyInnerClass, _myInnerClass)
		float _myFloat;
	public:
		MyInnerClass& getMyInnerClass() { return _myInnerClass; }
		void setMyInt(int myInt) { _myInt = myInt; }
		void setMyFloat(float myFloat) { _myFloat = myFloat; }
		
		MyClass() {
			registerField(_myFloat, SB_NAMEOF(_myFloat));
		}
	};

	class XmlWriter {
	protected:
		static string objectStart(const string& name, size_t depth) {
			ostringstream os; os << tabs(depth) << "<object name=\"" << name << "\">"; 
			return os.str();
		}
		static string objectEnd(const string& name, size_t depth) {
			ostringstream os; os << tabs(depth) << "</object>"; 
			return os.str();
		}
		template <class T> static string element(T& elem, const string& name, ReflectionState& state) {
			ostringstream os; os << tabs(state.depth) << "<element name=\"" << name << "\">" << elem << "</element>"; 
			return os.str();
		}
	public:
		static void writeReflectable(BaseReflectable& reflectable, const string& name, ReflectionState state) {
			auto fields = reflectable.getFields();
			auto currentDepth = state.depth;
			state.os << objectStart(name, currentDepth) << endl;
			state.depth += 1;
			for (size_t i = 0; i < fields.size(); i++) 
				fields[i]->write(state);
			state.os << objectEnd(name, currentDepth) << endl;
		}

		template <class T>
		static void write(T& object, const string& name, ReflectionState state) {
			state.os << element(object, name, state) << endl;
		}
	};

	template <>
	void XmlWriter::write<float>(float& object, const string& name, ReflectionState state) {
		auto precision = cout.precision();
		state.os << setprecision(3) << element(object, name, state) << setprecision(precision) << endl;
	}

	template <class TReflector, class TType, bool> struct Writer;

	template <class TReflector, class TType>
	struct Writer<TReflector, TType, false> {
		static void write(TType& object, const string& name, ReflectionState state) {
			TReflector::template write<TType>(object, name, state);
		}
	};

	template <class TReflector, class TType>
	struct Writer<TReflector, TType, true> {
		static void write(TType& reflectable, const string& name, ReflectionState state) {
			TReflector::writeReflectable(reflectable, name, state);
		}
	};
	
	template <class TReflector, class TType>
	void writeField(TType& object, const string& name, ReflectionState state) {
		const bool isReflectable = is_base_of<BaseReflectable, TType>::value;
		Writer<TReflector, TType, isReflectable>::write(object, name, state);
	}

	template <class TType>
	void write(TType& object, const string& name, ReflectionState state) {
		if (state.reflectorName == SB_NAMEOF(XmlWriter)) {
			writeField<XmlWriter, TType>(object, name, state);
		}
	}

	template <class TType>
	void write(TType& object,const string& name, const string& reflectorName, ostringstream& os) {
		write(object, name, ReflectionState(reflectorName, os));
	}

	void demo3() {
		MyClass myClass;
		myClass.setMyInt(42);
		myClass.setMyFloat(1.2345f);
		myClass.getMyInnerClass().setMyInnerDouble(9.87654);
		MyInnerClass myInnerClass;
		myInnerClass.setMyInnerDouble(-1.234567);
		ostringstream os;
		write(myClass, SB_NAMEOF(myClass), SB_NAMEOF(XmlWriter), os);
		write(myInnerClass, SB_NAMEOF(myInnerClass), SB_NAMEOF(XmlWriter), os);
		auto test = os.str();
		cout << test << endl;
	}

	void demo2() {
		float myFloat = 3.1415f;
		ostringstream os;
		write(myFloat, SB_NAMEOF(myFloat), SB_NAMEOF(XmlWriter), os);
		cout << os.str() << endl;
	}

	void demo1() {
		int myInt = 42;
		ostringstream os;
		write(myInt, SB_NAMEOF(myInt), SB_NAMEOF(XmlWriter), os);
		cout << os.str() << endl;
	}

	void run() {
		demo3();
		//demo2();
		//demo1();

		cin.get();
	}
}