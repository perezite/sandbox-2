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

	template <class T>
	string stringify(T& t) {
		ostringstream os;
		os << t;
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
		SB_FIELD(MyInnerClass, _myInnerObject)
		SB_FIELD(vector<char>, _myCharVector)
		float _myFloat;
	public:
		MyInnerClass& getMyInnerObject() { return _myInnerObject; }
		void setMyInt(int myInt) { _myInt = myInt; }
		void setMyFloat(float myFloat) { _myFloat = myFloat; }
		vector<char>& getMyCharVector() { return _myCharVector; }
		
		MyClass() {
			registerField(_myFloat, SB_NAMEOF(_myFloat));
		}
	};

	class XmlWriter {
	protected:
		static string openTag(const string& tagName, const string& name, size_t depth) {
			ostringstream os; os << tabs(depth) << "<" << tagName << " name=\"" << name << "\">"; 
			return os.str();
		}
		static string closeTag(const string& tagName, const string& name, size_t depth) {
			ostringstream os; os << tabs(depth) << "</" << tagName << ">"; 
			return os.str();
		}
		template <class T> static string tag(const string& tagName, const string& name, T& elem, ReflectionState& state) {
			ostringstream os; os << tabs(state.depth) << "<" << tagName << " name=\"" << name << "\">" << elem << "</" << tagName << ">"; 
			return os.str();
		}
	public:
		static void writeReflectable(BaseReflectable& reflectable, const string& name, ReflectionState state) {
			auto fields = reflectable.getFields();
			auto currentDepth = state.depth;
			state.os << openTag("object", name, currentDepth) << endl;
			state.depth += 1;
			for (size_t i = 0; i < fields.size(); i++) 
				fields[i]->write(state);
			state.os << closeTag("object", name, currentDepth) << endl;
		}

		template <class T>
		static void write(T& object, const string& name, ReflectionState state) {
			state.os << tag("property", name, object, state) << endl;
		}

		template <class T>
		static void write(vector<T>& vec, const string& name, ReflectionState state) {
			auto currentDepth = state.depth;
			state.depth = state.depth + 1;
			state.os << openTag("vector", name, currentDepth) << endl;
			for (size_t i = 0; i < vec.size(); i++)
				state.os << tag("item", stringify(i), vec[i], state) << endl;
			state.os << closeTag("vector", name, currentDepth) << endl;
		}
	};

	template <>
	void XmlWriter::write<float>(float& object, const string& name, ReflectionState state) {
		auto originalPrecision = cout.precision();
		state.os << setprecision(3) << tag("property", name, object, state) << setprecision(originalPrecision) << endl;
	}

	template <class TReflector, class TType, bool> struct Writer;

	template <class TReflector, class TType>
	struct Writer<TReflector, TType, false> {
		static void write(TType& object, const string& name, ReflectionState state) {
			TReflector::write(object, name, state);
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
	void write(TType& object, const string& name, const string& reflectorName, ostringstream& os) {
		write(object, name, ReflectionState(reflectorName, os));
	}

	void demo3() {
		MyClass myClass;
		myClass.setMyInt(42);
		myClass.setMyFloat(1.2345f);
		myClass.getMyCharVector() = vector<char>{ 'a', 'b', 'c' };
		myClass.getMyInnerObject().setMyInnerDouble(9.87654);
		MyInnerClass myInnerObject;
		myInnerObject.setMyInnerDouble(-1.234567);
		ostringstream os;
		write(myClass, SB_NAMEOF(myClass), SB_NAMEOF(XmlWriter), os);
		write(myInnerObject, SB_NAMEOF(myInnerObject), SB_NAMEOF(XmlWriter), os);
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