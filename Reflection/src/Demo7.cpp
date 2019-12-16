#include "Demo5.h"
#include "Macro.h"
#include "Logger.h"
#include <vector>
#include <istream>
#include <ostream>
#include <list>

using namespace std;

namespace reflectionDemo7 {
	// https://stackoverflow.com/questions/37031844/logic-of-stdis-base-of-in-c-11
	template<typename D, typename B>
	class IsDerivedFrom10
	{
		class No { };
		class Yes { No no[2]; };

		static Yes Test(B*) {};
		static No Test(...) {};
	public:
		static const bool value() {
			return sizeof(Test(static_cast<D*>(0))) == sizeof(Yes);
		}
	};

	// https://stackoverflow.com/questions/2631585/c-how-to-require-that-one-template-type-is-derived-from-the-other
	template <typename B, typename D>
	struct is_base_of // check if B is a base of D
	{
		typedef char yes[1];
		typedef char no[2];

		static yes& test(B*) {};
		static no& test(...) {};

		static D* get(void) {};

		static const bool value = (sizeof(test(get())) == sizeof(yes));
	};

	class BaseReflectable10 {

	};

	class DerivedReflectable10 : public BaseReflectable10 {

	};

	template <class TDerived>
	class MyBase10 {
	public:
		template <class T> static void baseInspect() {
			if (IsDerivedFrom10<T, BaseReflectable10>::value())
				TDerived::inspectReflectable();
			else
				TDerived::template inspect<T>();
		}
	};

	class MyDerived10 : public MyBase10<MyDerived10> {
	public:
		static void inspectReflectable() {
			cout << "BaseReflectable100" << endl;
		}

		template <class T> static void inspect() {
			cout << "type not supported" << endl;
		}
	};

	template <> void MyDerived10::inspect<BaseReflectable10>() { cout << "must not be called!!" << endl; }

	template <> void MyDerived10::inspect<int>() { cout << "int" << endl; }

	template <> void MyDerived10::inspect<float>() { cout << "float" << endl; }

	class RandomType10 { };

	void demo10() {
		MyDerived10::baseInspect<int>();
		MyDerived10::baseInspect<float>();
		MyDerived10::baseInspect<RandomType10>();
		MyDerived10::baseInspect<DerivedReflectable10>();
	}

	class MySprite100 {
	};

	template <class T>
	class MyReflectable100 {
		T* _reference;
	};

	template <class T>
	class MyNestedClass100 {
	};

	template <class T, template <class Inner> class V>
	void myMethod100(V<T> &con) {
	}

	template <class T>
	void myMethod100(MyReflectable100<T> &con) {
	}

	template <class T>
	void myMethod100(T &con) {
	}

	void demo100() {
		MyNestedClass100<int> myNestedClass;
		myMethod100(myNestedClass);
		MyReflectable100<MySprite100> mySpriteNode;
		myMethod100(mySpriteNode);
		int myInt;
		myMethod100(myInt);
	}

	template <class T>
	class MyReflectable200 {
		T* _reference;
	};

	template <class T> void myMethod200(MyReflectable200<T>& reflectable) {
	}

	template <class T> void myMethod200(T& t) {
	}

	void demo200() {
		int myInt;
		MyReflectable200<int> myReflectable;
		myMethod200(myInt);
		myMethod200(myReflectable);
	}

	class Inspector300 {
	public:
		template <class T> static void inspect(T& t) {
			cout << t << endl;
		}

		template <class T> static void inspect(MyReflectable200<T>& reflectable) {
			cout << "Inspector300::inspect<Reflectable>()" << endl;
		}
	};

	void demo300() {
		int myInt = 42;
		float myFloat = 3.1415f;
		MyReflectable200<float> myReflectable;
		Inspector300::inspect(myInt);
		Inspector300::inspect(myFloat);
		Inspector300::inspect(myReflectable);
	}

	namespace reflection {
		static void setInspector(const std::string& inspectorName);
		template <class T> static void inspect(T& t, const std::string& name, size_t depth);
	}

	class BaseReflectable600 {
	public:
		string getString() { return "BaseReflectable600::getString()"; }
	};

	template <class T>
	class Reflectable600 : public BaseReflectable600 {

	};

	class MyReflectable600 : public Reflectable600<MyReflectable600> {

	};

	void myFunc600(BaseReflectable600& t) {
		cout << "myFunc<BaseReflectable600>" << endl;
	}

	template <class T>
	void myFunc600(T& t) {
		cout << "myFunc<T>" << endl;
	}

	void demo600() {
		MyReflectable600 myReflectable;
		myFunc600(myReflectable);
	}

	template <bool isList>
	struct A;

	template<>
	struct A<true>
	{
		static void result() {
			cout << "true" << endl;
		}
	};

	template<>
	struct A<false>
	{
		static void result() {
			cout << "false" << endl;
		}
	};

	template <class T>
	T* convert600(T* t){
		// static_cast<BaseReflectable600*>(t);
	}

	template <class T>
	T* convert600(...) {
		return NULL;
		// return reflectable;
	}

	template <>
	BaseReflectable600* convert600<BaseReflectable600>(BaseReflectable600* reflectable) {
		return NULL;
	}

	const bool myResult() {
		return false;
	}

	void demo700() {
		//A<IsDerivedFrom10<int, float>::value()>::result();
		// const bool bla = true;
		// const bool bla = IsDerivedFrom10<int, float>::value();
		// A<bla>::result();
		//A<myResult()>::result();
		//MyReflectable600 myReflectable;
		//int myInt;
		//auto result = convert600(&myReflectable);
		//auto result2 = convert600(&myInt);

		// myRefelectable600;
		// auto result = tryConvertToBaseClass<BaseReflectable600, >()
	}

	// really basic
	template <bool>
	struct my_static_assert800;

	template <>
	struct my_static_assert800<true> {}; // only true is defined

	#define MY_STATIC_ASSERT(x) my_static_assert800<(x)>()

	template <class T> void myFunc800(T& t) {
		//auto result = is_base_of<BaseReflectable600, T>::value;
		my_static_assert800<is_base_of<BaseReflectable600, T>::value>();
		//MY_STATIC_ASSERT(!is_base_of<BaseReflectable1000, T>::value);
		cout << "basic" << endl;
	}

	void myFunc800(BaseReflectable600& reflectable) {
		cout << "specialized" << endl;
	}

	template <bool, class TBase, class TCurrent> 
	struct returnBaseIf800;

	template < class TBase, class TCurrent>
	struct returnBaseIf800<false, TBase, TCurrent> {
		static TCurrent& value(TCurrent& t) {
			return t;
		}
	};
 
	template <class TBase, class TCurrent>
	struct returnBaseIf800<true, TBase, TCurrent> {
		static TBase& value(TCurrent& t) {
			return (TBase&)t;
		}
	};	

	// converts the object to the base type and returns it, if possible. Otherwise, just returns the input object unchanged
	#define TRY_CONVERT_TO_BASE800(potentialBaseType, currentType, object) \
		returnBaseIf800<is_base_of<potentialBaseType, currentType>::value, potentialBaseType, currentType>::value(object)

	void demo800() {
		MyReflectable600 myReflectable;
		int myInt = 42;
		auto result = TRY_CONVERT_TO_BASE800(BaseReflectable600, MyReflectable600, myReflectable);
		auto result2 = TRY_CONVERT_TO_BASE800(BaseReflectable600, int, myInt);
		cout << result.getString() << endl;
		cout << result2 << endl;
	}

	class BaseProperty1000 {
		std::string _name;
	public:
		BaseProperty1000(const std::string name) : _name(name)
		{ }
		const std::string& getName() { return _name; }
		virtual void inspect(size_t depth) = 0;
	};

	template <class T>
	class Property1000 : public BaseProperty1000 {
		T& _reference;
	public:
		Property1000(T& reference, const std::string& name)
			: BaseProperty1000(name), _reference(reference)
		{ }
		virtual void inspect(size_t depth) {
			reflection::inspect(_reference, getName(), depth);
		}
	};

	class BaseReflectable1000 {
	public:
		virtual std::vector<BaseProperty1000*>& getProperties() = 0;
	};

	template <class T>
	class Reflectable1000 : public BaseReflectable1000 {
		typedef void(T::*Registration)();
		static std::vector<Registration> PropertyRegistrations;
		static std::vector<std::string> PropertyRegistrationNames;
		static bool RegistrationsInitialized;
		std::vector<BaseProperty1000*> _properties;
		bool _propertiesInitialized;
	protected:
		virtual void initProperties() {
			T* instance = (T*)this;
			for (size_t i = 0; i < PropertyRegistrations.size(); i++)
				(instance->*PropertyRegistrations[i])();
			_propertiesInitialized = true;
		}
		static bool findRegistrationName(std::string name) {
			return std::find(PropertyRegistrationNames.begin(), PropertyRegistrationNames.end(), name)
				!= PropertyRegistrationNames.end();
		}
		template <class TConverted> void addConvertedProperty(TConverted& value, const std::string& name) {
			_properties.push_back(new Property1000<TConverted>(value, name));
		}
	public:
		Reflectable1000() : _propertiesInitialized(false)
		{ }
		static void addRegistration(Registration registration, const std::string name) {
			if (findRegistrationName(name))
				RegistrationsInitialized = true;
			else
				PropertyRegistrationNames.push_back(name);
			if (!RegistrationsInitialized) {
				PropertyRegistrations.push_back(registration);
			}
		}
		virtual std::vector<BaseProperty1000*>& getProperties() {
			if (!_propertiesInitialized) {
				initProperties();
				_propertiesInitialized = true;
			}
			return _properties;
		}

		template <class U> void addProperty(U& value, const std::string& name) {
			addConvertedProperty(TRY_CONVERT_TO_BASE800(BaseReflectable1000, U, value), name);
		}
	};

	template <class T>
	std::vector<void(T::*)()> Reflectable1000<T>::PropertyRegistrations;

	template <class T>
	std::vector<std::string> Reflectable1000<T>::PropertyRegistrationNames;

	template <class T> bool Reflectable1000<T>::RegistrationsInitialized = false;

	template <void(*Action)()>
	class Invocation1000 {
	public:
		Invocation1000() {
			Action();
		}
	};

	class MyInnerReflectable1000 : public Reflectable1000<MyInnerReflectable1000> {
		double _myDouble;
		void addProperty_myDouble() {
			addProperty(_myDouble, SB_NAMEOF(_myDouble));
		}
		static void register_myDouble() {
			addRegistration(&MyInnerReflectable1000::addProperty_myDouble, SB_NAMEOF(_myDouble));
		}
		Invocation1000<register_myDouble> invoke_register_myInt;
	public:
		void setMyDouble(double myDouble) { _myDouble = myDouble; }
		double getMyDouble() const { return _myDouble; }
	};

	class MyReflectable1000 : public Reflectable1000<MyReflectable1000> {
		int _myInt;
		void addProperty_myInt() {
			addProperty(_myInt, SB_NAMEOF(_myInt));
		}
		static void register_myInt() {
			addRegistration(&MyReflectable1000::addProperty_myInt, SB_NAMEOF(_myInt));
		}
		Invocation1000<register_myInt> invoke_register_myInt;
		float _myFloat;
		void addProperty_myFloat() {
			addProperty(_myFloat, SB_NAMEOF(_myFloat));
		}
		static void register_myFloat() {
			addRegistration(&MyReflectable1000::addProperty_myFloat, SB_NAMEOF(_myFloat));
		}
		Invocation1000<register_myFloat> invoke_register_myFloat;
		MyInnerReflectable1000 _myInnerReflectable;
		void addProperty_myInnerReflectable() {
			addProperty((BaseReflectable1000&)_myInnerReflectable, SB_NAMEOF(_myInnerReflectable));
		}
		static void register_myInnerReflectable() {
			addRegistration(&MyReflectable1000::addProperty_myInnerReflectable, SB_NAMEOF(_myInnerReflectable));
		}
		Invocation1000<register_myInnerReflectable> invoke_register_myInnerReflectable;
	public:
		int getMyInt() const { return _myInt; }
		float getMyFloat() const { return _myFloat; }
		const MyInnerReflectable1000& getMyInnerRefletable() const { return _myInnerReflectable; }
		void setMyInt(int myInt) { _myInt = myInt; }
		void setMyFloat(float myFloat) { _myFloat = myFloat; }
		MyInnerReflectable1000& getMyInnerReflectable() { return _myInnerReflectable; }
	};

	class TextWriter1000 {
		static std::ostream* Stream;
	public:
		static std::ostream& getStream() { return *Stream; }
		template <class T> static void write(T& t, const string& name, size_t depth) {
			getStream() << std::string(depth, ' ') << name << " " << t << endl;
		}

		static void write(BaseReflectable1000& myReflectable, const std::string& name, size_t depth) {
			getStream() << std::string(depth, ' ') << name << endl;
			auto properties = myReflectable.getProperties();
			for (size_t i = 0; i < properties.size(); i++)
				properties[i]->inspect(depth + 1);
		}

		static void write(BaseReflectable1000& t, std::ostream& os) {
			reflection::setInspector(SB_NAMEOF(TextWriter1000));
			Stream = &os;
			write(t, "root", 0);
		}
	};

	std::ostream* TextWriter1000::Stream;

	void demo1000() {
		// abstract write
		MyReflectable1000 myReflectable;
		myReflectable.setMyInt(42);
		myReflectable.setMyFloat(3.1415f);
		myReflectable.getMyInnerReflectable().setMyDouble(9.876);
		std::ostringstream os;
		TextWriter1000::write(myReflectable, cout);
	}

	int countStart(const std::string& str, char token) {
		size_t counter = 0;
		for (size_t i = 0; i < str.length(); i++) {
			if (str[i] == token)
				counter++;
			else
				break;
		}

		return counter;
	}

	void split(const std::string& s, const std::string& delimiter, std::vector<std::string>& result) {
		size_t start = 0;
		size_t pos = 0;
		size_t delimiterLen = delimiter.length();
		while (true) {
			pos = s.find(delimiter, start);
			if (pos != std::string::npos) {
				size_t len = pos - start;
				if (len > 0)
					result.emplace_back(s.substr(start, len));
				start = pos + delimiterLen;
			}
			else {
				size_t len = s.length() - start;
				if (len > 0)
					result.emplace_back(s.substr(start, len));
				break;
			}
		}
	}

	BaseProperty1000* findProperty(BaseReflectable1000& reflectable, const std::string& propertyName) {
		BaseProperty1000* result = NULL;
		auto properties = reflectable.getProperties();
		for (size_t i = 0; i < properties.size(); i++) {
			if (properties[i]->getName() == propertyName)
				result = properties[i];
		}
		return result;
	}

	class Reader2000 {
	public:
		static void skipLine(std::istream& is) {
			std::string line; std::getline(is, line);
		}
		static bool extractLine(std::string& name, std::string& value, int depth, std::istream& is) {
			std::string line;
			if (std::getline(is, line)) {
				size_t currentDepth = countStart(line, ' ');
				if (depth == currentDepth) {
					std::vector<std::string> result;
					split(line, " ", result);
					name = result[0];
					value = result.size() == 2 ? result[1] : "";
					return true;
				}
			}
			return false;
		}
	};

	class TextReader2000 : public Reader2000 {
		static std::istream* Stream;
		static std::string CurrentValue;
	protected:
		static std::istream& getStream() { return *Stream; }
		static void readProperties(BaseReflectable1000& reflectable, size_t depth) {
			std::string name;
			while (extractLine(name, CurrentValue, depth, getStream())) {
				BaseProperty1000* property = findProperty(reflectable, name);
				property->inspect(depth);
			}
		}
	public:
		static void readProperty(BaseReflectable1000& reflectable, const std::string& name, size_t depth) {
			readProperties(reflectable, depth + 1);
		}
		template <class T>
		static void readProperty(T& t, const std::string& name, size_t depth) {
			if (IsDerivedFrom10<T, BaseReflectable1000>::value() == false)
				SB_ERROR("the type of " << name << " is not supported by TextWriter");
			readProperty((BaseReflectable1000&)t, name, depth);
		}
		static void read(std::istream& is, BaseReflectable1000& reflectable) {
			reflection::setInspector(SB_NAMEOF(TextReader20000));
			Stream = &is;
			skipLine(getStream());
			//read(reflectable, 0);
		}
	};

	std::istream* TextReader2000::Stream;
	std::string TextReader2000::CurrentValue;

	#define SB_REGISTER_INSPECTOR(str, inspectorMethod) \
		if (CurrentInspectorName == str) \
			inspectorMethod(t, name, depth);

	namespace reflection {
		static std::string CurrentInspectorName;
		static void setInspector(const std::string& inspectorName) { CurrentInspectorName = inspectorName; }
		template <class T> static void inspect(T& t, const std::string& name, size_t depth) {
			if (CurrentInspectorName == SB_NAMEOF(TextWriter1000))
				TextWriter1000::write(t, name, depth);
			else
				SB_ERROR("Inspector " << CurrentInspectorName << " not found");
		}
	}

	void demo2000() {
		// abstract read
		std::ostringstream os;
		os << "root";
		os << " _myInt 42";
		os << " _myFloat 3.1415";
		os << " _myInnerReflectable";
		os << "  _myDouble 9.876";
		std::istringstream is(os.str());

		MyReflectable1000 myReflectable;
		// TextReader1000::read(myReflectable, is);
	}

	void run() {
		// next: insert the stuff from demo800 into the reflectables (no more SB_COMPLEX_PROPERTY anymore!)

		// demo7: abstract reader/writer/editor
		//demo2000();
		demo1000();
		//demo800();
		//demo700();
		//demo600();
		//demo300();
		//demo200();
		//demo100();
		//demo10();
	}

}
