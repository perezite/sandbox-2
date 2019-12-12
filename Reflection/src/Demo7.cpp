#include "Demo5.h"
#include "Macro.h"
#include "Logger.h"
#include <vector>
#include <istream>
#include <ostream>

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
		static bool value() {
			return sizeof(Test(static_cast<D*>(0))) == sizeof(Yes);
		}
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

	//class BaseReflectable1000;
	//class InnerProperty1000 : public Property1000<BaseReflectable1000> {
	//public:
	//	InnerProperty1000(BaseReflectable1000& reference, const std::string& name)
	//		: Property1000<BaseReflectable1000>(reference, name)
	//	{ }
	//};

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

		template <class U> void addInnerProperty(U& value, const std::string& name) {
			_properties.push_back(new Property1000<BaseReflectable1000>((BaseReflectable1000&)value, name));
		}

		template <class U> void addProperty(U& value, const std::string& name) { 
			_properties.push_back(new Property1000<U>(value, name)); 
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
		void addPropert_myFloat() {
			addProperty(_myFloat, SB_NAMEOF(_myFloat));
		}
		static void register_myFloat() {
			addRegistration(&MyReflectable1000::addPropert_myFloat, SB_NAMEOF(_myFloat));
		}
		Invocation1000<register_myFloat> invoke_register_myFloat;
		MyInnerReflectable1000 _myInnerReflectable;
		void addProperty_myInnerReflectable() {
			addInnerProperty(_myInnerReflectable, SB_NAMEOF(_myInnerReflectable));
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
			getStream() << std::string(depth, ' ') << t << endl;
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

	std::ostream* TextWriter1000::Stream;

	void demo1000() {
		// simplify/abstract reader
		MyReflectable1000 myReflectable;
		myReflectable.setMyInt(42);
		myReflectable.setMyFloat(3.1415f);
		myReflectable.getMyInnerReflectable().setMyDouble(9.876);
		std::ostringstream os;
		TextWriter1000::write(myReflectable, cout);
	}

	void run() {
		// demo7: abstracting reader/writer/editor
		demo1000();
		//demo300();
		//demo200();
		//demo100();
		//demo10();
	}

}
